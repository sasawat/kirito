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
ostream& operator<<(ostream& os, const EmptySlot& e) {
	return os << "EmptySlot";
} 

ostream& operator<<(ostream& os, const OpenSlot& o) {
	return os << "OpenSlot";
} 
#endif  // DEBUG

template <typename A, typename B, typename C>
struct Index {
	const A a;
	const B b;
	const C c;

	constexpr Index(A a, B b, C c) : a(a), b(b), c(c) {}
};

template <typename A, typename B>
struct IndexPartial : public Index<A, B, OpenSlot> {
	constexpr IndexPartial(A a, B b) : Index<A, B, OpenSlot>(a, b, {}) {}

	template <typename C, typename = enable_if_t<is_integral<C>::value>>
	constexpr Index<A, B, C> operator()(C c) const {
		return {Index<A, B, OpenSlot>::a, Index<A, B, OpenSlot>::b, c};
	}
};

template <typename A>
struct IndexStart : public Index<A, OpenSlot, OpenSlot> {
	constexpr IndexStart(A a) : Index<A, OpenSlot, OpenSlot>(a, {}, {}) {}

	template <typename B, typename = enable_if_t<is_integral<B>::value>>
	constexpr IndexPartial<A, B> operator()(B b) const {
		return {Index<A, OpenSlot, OpenSlot>::a, b};
	}

	constexpr IndexPartial<A, EmptySlot> operator()() const {
		return {Index<A, OpenSlot, OpenSlot>::a, {}};
	}
};

constexpr IndexStart<EmptySlot> IndexBuilder() { return {{}}; }

template <typename A, typename = enable_if_t<is_integral<A>::value>>
constexpr IndexStart<A> IndexBuilder(A a) { return {a}; }

template <
	typename BaseContainer,
	typename T = typename BaseContainer::value_type
>
class FancyContainer : public BaseContainer {
	using typename BaseContainer::iterator;
	using BaseContainer::begin;
	using BaseContainer::end;

	FancyContainer(const BaseContainer &b) : BaseContainer(b) {}

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
	FancyContainer operator[](Index<A, B, C> ndx) {
		if constexpr (is_same<C, OpenSlot>::value) {
			return copySlice(getStart(ndx.a), getStop(ndx.b), 1);
		} else {
			return copySlice(getStart(ndx.a), getStop(ndx.b), ndx.c);
		}
	}
};

}  // namespace kirito
#endif  // KIRITO_H_
