/**
 * To conform with behavior as outlined in: 
 * stackoverflow.com/questions/509211/understanding-pythons-slice-notation
 */
#include <cassert>
#include <iostream>
#include <vector>

#include <kirito.h>

using kirito::I;
using kirito::V;
using kirito::FancyContainer;
using std::vector;
using std::cout;
using std::endl;

bool match(vector<int> a, vector<int> b) {
	if (a.size() != b.size()) return false;
	for (size_t i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

int main(int argc, char **argv) {
	// the thing we are cutting and connecting today
	FancyContainer<vector<int>> t = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	// With slice indexing I
	// a[start:end] # items start through end-1
	assert(match(t[I(1)(6)], {1, 2, 3, 4, 5}));
	// a[start:]    # items start through the rest of the array
	assert(match(t[I(7)], {7, 8, 9}));
	// a[:end]      # items from the beginning through end-1
	assert(match(t[I()(3)], {0, 1, 2}));
	// a[:]         # a copy of the whole array
	assert(match(t[I()], t));
	// a[start:end:step] # start through not past end, by step
	assert(match(t[I(1)(8)(2)], {1, 3, 5, 7}));
	// a[-1]    # last item in the array
	// not supported. we always return a container instead of an item
	// we could overload the normal integral operator[] to support negative
	// indexing though. but we don't. 
	// a[-2:]   # last two items in the array
	assert(match(t[I(-2)()], {8, 9}));
	// a[:-2]   # everything except the last two items
	assert(match(t[I()(-2)], {0, 1, 2, 3, 4, 5, 6, 7}));
	// a[::-1]    # all items in the array, reversed
	assert(match(t[I()()(-1)], {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
	// a[1::-1]   # the first two items, reversed
	assert(match(t[I(1)()(-1)], {1, 0}));
	// a[:-3:-1]  # the last two items, reversed
	assert(match(t[I()(-3)(-1)], {9, 8}));
	// a[-3::-1]  # everything except the last two items, reversed
	assert(match(t[I(-3)()(-1)], {7, 6, 5, 4, 3, 2, 1, 0}));

	// With view indexing V
	// a[start:end] # items start through end-1
	assert(match(t[V(1)(6)], {1, 2, 3, 4, 5}));
	// a[start:]    # items start through the rest of the array
	assert(match(t[V(7)], {7, 8, 9}));
	// a[:end]      # items from the beginning through end-1
	assert(match(t[V()(3)], {0, 1, 2}));
	// a[:]         # a copy of the whole array
	assert(match(t[V()], t));
	// a[start:end:step] # start through not past end, by step
	assert(match(t[V(1)(8)(2)], {1, 3, 5, 7}));
	// a[-1]    # last item in the array
	// not supported. we always return a container instead of an item
	// we could overload the normal integral operator[] to support negative
	// indexing though. but we don't. 
	// a[-2:]   # last two items in the array
	assert(match(t[V(-2)()], {8, 9}));
	// a[:-2]   # everything except the last two items
	assert(match(t[V()(-2)], {0, 1, 2, 3, 4, 5, 6, 7}));
	// a[::-1]    # all items in the array, reversed
	assert(match(t[V()()(-1)], {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
	// a[1::-1]   # the first two items, reversed
	assert(match(t[V(1)()(-1)], {1, 0}));
	// a[:-3:-1]  # the last two items, reversed
	assert(match(t[V()(-3)(-1)], {9, 8}));
	// a[-3::-1]  # everything except the last two items, reversed
	assert(match(t[V(-3)()(-1)], {7, 6, 5, 4, 3, 2, 1, 0}));

	// Python is kind to the programmer if there are fewer items than you
	// ask for. For example, if you ask for a[:-2] and a only contains one
	// element, you get an empty list instead of an error. Sometimes you
	// would prefer the error, so you have to be aware that this may happen.
	assert(match(t[I()(-44)], {}));
	assert(match(t[I(44)()], {}));
}
