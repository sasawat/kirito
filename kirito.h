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

	Index(A a, B b, C c) : a(a), b(b), c(c) {}
};

template <typename A, typename B>
struct IndexPartial : public Index<A, B, OpenSlot> {
	IndexPartial(A a, B b) : Index<A, B, OpenSlot>(a, b, {}) {}

	template <typename C, typename = enable_if_t<is_integral<C>::value>>
	Index<A, B, C> operator()(C c) { return {Index<A, B, OpenSlot>::a, Index<A, B, OpenSlot>::b, c}; }
};

template <typename A>
struct IndexStart : public Index<A, OpenSlot, OpenSlot> {
	IndexStart(A a) : Index<A, OpenSlot, OpenSlot>(a, {}, {}) {}

	template <typename B, typename = enable_if_t<is_integral<B>::value>>
	IndexPartial<A, B> operator()(B b) { return {Index<A, OpenSlot, OpenSlot>::a, b}; }

	IndexPartial<A, EmptySlot> operator()() { return {Index<A, OpenSlot, OpenSlot>::a, {}}; }
};

IndexStart<EmptySlot> IndexBuilder() { return {{}}; }

template <typename A, typename = enable_if_t<is_integral<A>::value>>
IndexStart<A> IndexBuilder(A a) { return {a}; }
}  // namespace kirito
#endif  // KIRITO_H_
