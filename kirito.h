#ifndef KIRITO_H_
#define KIRITO_H_

#include <cstdint>
#include <vector>
#include <algorithm>

namespace kirito {

using std::back_inserter;
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

	constexpr FancyContainer(const BaseContainer &b) : BaseContainer(b) {}
	FancyContainer(BaseContainer &&b) : BaseContainer(std::move(b)) {}

	using BaseContainer::BaseContainer;

private:
	template <typename Step>
	FancyContainer copySlice(iterator start, iterator stop, Step step) {
		BaseContainer rv;
		if (start < stop) {
			for (auto iter = start; iter < stop; iter += step) {
				rv.push_back(*iter);
			}
		} else if (start > stop) {
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
				return end() - o;
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
				return end() - o;
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
			return copySlice(getStart(ndx.a), getStop(ndx.b), ndx.c);
		}
	}

	template <typename C>
	class view_iterator : private iterator {
	public:
		using typename iterator::difference_type;
		const C step;

		view_iterator(const iterator &iter, C step) :
			iterator(iter), step(step) {}

		view_iterator(iterator &&iter, C step) :
			iterator(std::move(iter)), step(step) {}

		view_iterator& operator+=(difference_type rhs) {
			return iterator::operator+=(rhs * step);
		}

		view_iterator& operator-=(difference_type rhs) {
			return iterator::operator-=(rhs * step);
		}

		T& operator[](difference_type rhs) const {
			return iterator::operator[](rhs * step);
		}

		view_iterator& operator++() { return iterator::operator+=(step); }

		view_iterator& operator++(int) const {
			view_iterator temp(*this);
			iterator::operator+=(step);
			return temp;
		}

		view_iterator& operator--() { return iterator::operator-=(step); }

		view_iterator& operator--(int) const {
			view_iterator temp(*this);
			iterator::operator-=(step);
			return temp;
		}

		difference_type operator-(const view_iterator& rhs) const {
			return iterator::operator-(rhs) / step;
		}

		view_iterator operator+(difference_type rhs) const {
			return iterator::operator+(rhs * step);
		}

		view_iterator operator-(difference_type rhs) const {
			return iterator::operator-(rhs * step);
		}

		view_iterator operator<(const view_iterator &rhs) const {
			if (step < 0) return iterator::operator>(rhs);
			else          return iterator::operator<(rhs);
		}

		view_iterator operator>(const view_iterator &rhs) const {
			if (step < 0) return iterator::operator<(rhs);
			else          return iterator::operator>(rhs);
		}
		
		view_iterator operator<=(const view_iterator &rhs) const {
			if (step < 0) return iterator::operator>=(rhs);
			else          return iterator::operator<=(rhs);
		}

		view_iterator operator>=(const view_iterator &rhs) const {
			if (step < 0) return iterator::operator<=(rhs);
			else          return iterator::operator>=(rhs);
		}
		
		using iterator::operator*;
		using iterator::operator->;
		using iterator::operator==;
		using iterator::operator!=;
	};

	template <typename C>
	class View {
		view_iterator<C> start;
		view_iterator<C> stop;
	public:
		view_iterator<C> begin() { return start; }
		view_iterator<C> end() { return stop; }
		view_iterator<C> rbegin() { return {start, -start.step}; }
		view_iterator<C> rend() { return {stop, -stop.step}; }
		T &operator[](C at) { return start[at]; }

		View(const view_iterator<C> &start, const view_iterator<C> &stop) :
			start(start), stop(stop) {}

		View(const iterator &start, const iterator &stop, C step) :
			start(start, step), stop(stop, step) {}
	};

	template <typename A, typename B, typename C>
	View<C> operator[](Index<A, B, C, true> ndx) {
		if constexpr (is_same<C, OpenSlot>::value) {
			return {begin(), end(), 1};
		} else {
			return {begin(), end(), ndx.c};
		}
	}
};

}  // namespace kirito
#endif  // KIRITO_H_
