#ifndef KIRITO_H_
#define KIRITO_H_

#include <cstdint>
#include <vector>
#include <algorithm>

namespace kirito {

using std::random_access_iterator_tag;
using std::enable_if_t;
using std::is_integral;
using std::is_same;

struct EmptySlot {};

struct OpenSlot {};

#ifdef DEBUG
#include <iostream>
using std::ostream;
inline ostream& operator<<(ostream& os, const EmptySlot& e) {
	return os << "EmptySlot";
} 

inline ostream& operator<<(ostream& os, const OpenSlot& o) {
	return os << "OpenSlot";
} 
#endif  // DEBUG

template <typename A, typename B, typename C, bool V = false>
struct Index {
	const A a;
	const B b;
	const C c;

	constexpr Index(A a, B b, C c) : a(a), b(b), c(c) {}
};

template <typename A, typename B, bool V>
struct IndexPartial : public Index<A, B, OpenSlot, V> {
	constexpr IndexPartial(A a, B b) : Index<A, B, OpenSlot, V>(a, b, {}) {}

	template <typename C, typename = enable_if_t<is_integral<C>::value>>
	constexpr Index<A, B, C, V> operator()(C c) const {
		return {Index<A, B, OpenSlot, V>::a, Index<A, B, OpenSlot, V>::b, c};
	}
};

template <typename A, bool V>
struct IndexStart : public Index<A, OpenSlot, OpenSlot, V> {
	constexpr IndexStart(A a) : Index<A, OpenSlot, OpenSlot, V>(a, {}, {}) {}

	template <typename B, typename = enable_if_t<is_integral<B>::value>>
	constexpr IndexPartial<A, B, V> operator()(B b) const {
		return {Index<A, OpenSlot, OpenSlot, V>::a, b};
	}

	constexpr IndexPartial<A, EmptySlot, V> operator()() const {
		return {Index<A, OpenSlot, OpenSlot, V>::a, {}};
	}
};

template <bool V>
struct IndexBuilder {
	constexpr IndexStart<EmptySlot, V> operator()() const { return {{}}; }
	
	template <typename A, typename = enable_if_t<is_integral<A>::value>>
	constexpr IndexStart<A, V> operator()(A a) const { return {a}; }
};

constexpr static IndexBuilder<false> I;
constexpr static IndexBuilder<true> V;

template <
	typename BaseContainer,
	typename T = typename BaseContainer::value_type
>
class FancyContainer : public BaseContainer {
public:
	using typename BaseContainer::iterator;
	using BaseContainer::begin;
	using BaseContainer::end;
	using BaseContainer::operator[];

	constexpr FancyContainer(const BaseContainer &b) : BaseContainer(b) {}
	FancyContainer(BaseContainer &&b) : BaseContainer(std::move(b)) {}

	using BaseContainer::BaseContainer;

private:
	template <typename Step>
	FancyContainer copySlice(iterator start, iterator stop, Step step) {
		BaseContainer rv;
		if (step > 0) {
			for (auto iter = start; iter < stop; iter += step) {
				rv.push_back(*iter);
			}
		} else if (step < 0) {
			for (auto iter = start; iter > stop; iter += step) {
				rv.push_back(*iter);
			}
		}
		return rv;
	}

	template <typename Offset>
	iterator getStart(Offset o) {
		if constexpr (is_same<Offset, EmptySlot>::value) {
			return begin();
		} else {
			if (o < 0) {
				return end() + o;
			} else {
				return begin() + o;
			}
		}
	}

	template <typename Offset>
	iterator getStop(Offset o) {
		if constexpr (
			is_same<Offset, EmptySlot>::value ||
			is_same<Offset, OpenSlot>::value
		) {
			return end();
		} else {
			if (o < 0) {
				return end() + o;
			} else {
				return begin() + o;
			}
		}
	}

public:
	template <typename A, typename B, typename C>
	FancyContainer operator[](Index<A, B, C, false> ndx) {
		if constexpr (is_same<C, OpenSlot>::value) {
			return copySlice(getStart(ndx.a), getStop(ndx.b), 1);
		} else {
			iterator start;
			iterator stop;
			if constexpr (is_same<A, EmptySlot>::value) {
				if (ndx.c < 0) start = end() - 1;
				else           start = begin();
			} else {
				start = getStart(ndx.a);
			}
			if constexpr (is_same<B, EmptySlot>::value) {
				if (ndx.c < 0) stop = begin() - 1;
				else           stop = end();
			} else {
				stop = getStop(ndx.b);
			}
			return copySlice(start, stop, ndx.c);
		}
	}

	template <
		typename C,
		typename difference_type = typename iterator::difference_type
	>
	class view_iterator :
		public std::iterator<random_access_iterator_tag, T> {
		iterator iter;
		C step;
	public:
		view_iterator(const iterator &iter, C step) :
			iter(iter), step(step) {}

		view_iterator(iterator &&iter, C step) :
			iter(std::move(iter)), step(step) {}

		view_iterator() = default;
		view_iterator(const view_iterator &other) = default;
		view_iterator(view_iterator &&other) = default;

		view_iterator reverse() const {
			return {iter, -step};
		}

		view_iterator& operator=(const view_iterator &rhs) {
			iter = rhs.iter;
			step = rhs.step;
		}

		view_iterator& operator+=(difference_type rhs) {
			return {iter += (rhs * step), step};
		}

		view_iterator& operator-=(difference_type rhs) {
			return {iter -= (rhs * step), step};
		}

		T& operator[](difference_type rhs) const {
			return iter[rhs * step];
		}

		view_iterator& operator++() {
			iter += step;
			return *this;
		}

		view_iterator& operator++(int) {
			view_iterator temp(*this);
			iter += step;
			return temp;
		}

		view_iterator& operator--() {
			iter -= step;
			return *this;
		}

		view_iterator& operator--(int) {
			view_iterator temp(*this);
			iter -= step;
			return temp;
		}

		difference_type operator-(const view_iterator& rhs) const {
			return (iter - rhs.iter) / step;
		}

		view_iterator operator+(difference_type rhs) const {
			return {iter + (rhs * step), step};
		}

		view_iterator operator-(difference_type rhs) const {
			return {iter - (rhs * step), step};
		}

		bool operator<(const view_iterator &rhs) const {
			if (step < 0) return iter > rhs.iter;
			else          return iter < rhs.iter;
		}

		bool operator>(const view_iterator &rhs) const {
			if (step < 0) return iter < rhs.iter;
			else          return iter > rhs.iter;
		}
		
		bool operator<=(const view_iterator &rhs) const {
			if (step < 0) return iter >= rhs.iter;
			else          return iter <= rhs.iter;
		}

		bool operator>=(const view_iterator &rhs) const {
			if (step < 0) return iter <= rhs.iter;
			else          return iter >= rhs.iter;
		}

		bool operator==(const view_iterator &rhs) const {
			return iter == rhs.iter;
		}

		bool operator!=(const view_iterator &rhs) const {
			return iter != rhs.iter;
		}
		
		T& operator*() { return *iter; }
		const T& operator*() const { return *iter; }
		T* operator->() { return iter; }
		const T* operator->() const { return iter; }
	};

	template <typename C>
	class View {
		view_iterator<C> start;
		view_iterator<C> stop;
	public:
		view_iterator<C> begin() { return start; }
		view_iterator<C> end() { return stop; }
		view_iterator<C> rbegin() { return (stop).reverse() + 1; }
		view_iterator<C> rend() { return (start).reverse() + 1; }
		const view_iterator<C> cbegin() const { return start; }
		const view_iterator<C> cend() const { return stop; }
		const view_iterator<C> crbegin() const {
			return (stop).reverse() + 1;
		}
		const view_iterator<C> crend() const {
			return (start).reverse() + 1;
		}

		T& front() { return start[0]; }
		const T& front() const { return start[0]; }
		T& back() { return start[size() - 1]; }
		const T& back() const { return start[size() - 1]; }

		T& operator[](C at) { return start[at]; }
		const T &operator[](C at) const { return start[at]; }
		T& at(C at) { return start[at]; }
		const T at(C at) const { return start[at]; }

		size_t size() const { return cend() - cbegin(); }

		View(const view_iterator<C> &start, const view_iterator<C> &stop) :
			start(start), stop(stop) {}

		View(const iterator &start, const iterator &stop, C step) :
			start(start, step), stop(stop, step) {}

		void swap(View &other) {
			auto temp = start;
			start = other.start;
			other.start = temp;
			temp = stop;
			stop = other.stop;
			other.stop = temp;
		}

		operator BaseContainer() {
			BaseContainer rv;
			for (auto iter = begin(); iter < end(); iter++) {
				rv.push_back(*iter);
			}
			return rv;
		}

		operator FancyContainer() {
			FancyContainer rv;
			for (auto iter = begin(); iter < end(); iter++) {
				rv.push_back(*iter);
			}
			return rv;
		}
	};

	template <typename A, typename B, typename C,
		 typename = enable_if_t<!is_same<C, OpenSlot>::value>>
	View<C> operator[](Index<A, B, C, true> ndx) {
		iterator start;
		iterator stop;
		if constexpr (is_same<A, EmptySlot>::value) {
			if (ndx.c < 0) start = end() - 1;
			else           start = begin();
		} else {
			start = getStart(ndx.a);
		}
		if constexpr (is_same<B, EmptySlot>::value) {
			if (ndx.c < 0) stop = begin() - 1;
			else           stop = end();
		} else {
			stop = getStop(ndx.b);
		}
		auto comp = std::abs(stop - start) % ndx.c;
		return {start, stop + comp, ndx.c};
	}

	template <typename A, typename B, typename C,
		 typename = enable_if_t<is_same<C, OpenSlot>::value>>
	View<int> operator[](Index<A, B, C, true> ndx) {
		return {getStart(ndx.a), getStop(ndx.b), 1};
	}
};

}  // namespace kirito
#endif  // KIRITO_H_
