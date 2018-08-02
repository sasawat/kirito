/**
 * To test that Views and Views of Views have the expected data
 */
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#define DEBUG
#include <kirito.h>

using kirito::I;
using kirito::V;
using kirito::FancyContainer;
using std::vector;
using std::cout;
using std::endl;

template <typename V>
bool match(const V &v, const vector<int> &c) {
	if (v.size() != c.size()) return false;
	for (decltype(v.size()) i = 0; i < v.size(); ++i)
		if (v[i] != c[i]) return false;
	return true;
}

template <typename V>
void print(const V &v) {
	for (auto i = v.cbegin(); i < v.cend(); ++i) {
		cout << *i << " ";
	}
	cout << endl;
}

int main(int argc, char **argv) {
	FancyContainer<vector<int>> t;
	for (int i = 0; i < 100; ++i) t.push_back(i);

	assert(match(t[V()], t));
	assert(match(t[V()][V()], t));
	assert(match(t[V(-1)()], {99}));
	assert(match(t[V(-2)()][V(-1)()], {99}));
	assert(match(t[V()()(25)], {0, 25, 50, 75}));
	assert(match(t[V()()(25)][V(-2)(-1)], {50}));
	assert(match(t[V()()(25)][V(-1)()], {75}));
	assert(match(t[V()()(-25)], {99, 74, 49, 24}));
	assert(match(t[V()()(-25)][V()()(-2)], {24, 74}));
	assert(match(t[V()()(-25)][V()()(-2)][V(-1)()], {74}));
	assert(match(t[V(1)()(25)], {1, 26, 51, 76}));
	assert(match(t[V(-50)()(25)], {50, 75}));
	print(t[V(-49)()(-25)]);
	assert(match(t[V(-49)()(-25)], {51, 26, 1}));
}
