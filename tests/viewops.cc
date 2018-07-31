/**
 * To test that some basic STL container operations work on views
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

template <typename iter>
void print(iter start, iter stop) {
	for (auto i = start; i < stop; ++i) {
		cout << *i << " ";
	}
	cout << endl;
}

int main(int argc, char **argv) {
	FancyContainer<vector<int>> t = {0, 1, 2, 3, 4, 5};
	auto v = t[V()];

	assert(v.size() == 6);
	for (int i = 0; i < 6; ++i) assert(v[i] == i);
	print(v.begin(), v.end());
	print(v.rbegin(), v.rend());
	print(v.cbegin(), v.cend());
	print(v.crbegin(), v.crend());

	v[3] = 39;
	assert(t[3] == 39);
	*v.begin() = 42;
	assert(t[0] == 42);
	v.front() = 88;
	assert(t[0] == 88);
	v.back() = 11;
	assert(t[5] == 11);
}
