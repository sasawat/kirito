/**
 * To test that returned items can be turned into a container as needed
 */
#include <cassert>
#include <iostream>
#include <vector>

#include <kirito.h>

using kirito::I;
using kirito::V;
using kirito::FancyContainer;
using std::vector;

template <typename T>
bool match(T a, T b) {
	if (a.size() != b.size()) return false;
	for (size_t i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

int main(int argc, char **argv) {
	FancyContainer<vector<int>> t = {0, 1, 2, 3};

	// FancyContainer is implicitly converted to BaseContainer
	assert(match<vector<int>>(t[I()], t));
	// FancyContainer actually works as expected
	assert(match<FancyContainer<vector<int>>>(t[I()], t));
	// View is implicitly converted to BaseContainer
	assert(match<vector<int>>(t[V()], t));
	// View is implicity converted to FancyContainer
	assert(match<FancyContainer<vector<int>>>(t[V()], t));

	// But now with const references
	// FancyContainer is implicitly converted to BaseContainer
	assert(match<const vector<int>&>(t[I()], t));
	// FancyContainer actually works as expected
	assert(match<const FancyContainer<vector<int>>&>(t[I()], t));
	// View is implicitly converted to BaseContainer
	assert(match<const vector<int>&>(t[V()], t));
	// View is implicity converted to FancyContainer
	assert(match<const FancyContainer<vector<int>>&>(t[V()], t));
}
