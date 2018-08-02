/**
 * To test that some basic STL container operations work on views
 */
#include <algorithm>
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
using std::swap;

template <typename iter>
void print(iter start, iter stop) {
	for (auto i = start; i < stop; ++i) {
		cout << *i << " ";
	}
	cout << endl;
}

int main(int argc, char **argv) {
	FancyContainer<vector<int>> t = {5, 4, 3, 2, 1, 0};
	auto v = t[V()];

	std::sort(v.begin(), v.end());

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
	v.at(2) = 9001;
	assert(t[2] == 9001);

	FancyContainer<vector<int>> u = {999, 999, 999};
	auto w = u[V()];
	swap(w, v);
	assert(v[0] == 999);
	assert(w[0] == 88);

	t = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	v = t[V()()(2)];
	print(v.begin(), v.end());
	print(v.rbegin(), v.rend());
	print(v.cbegin(), v.cend());
	print(v.crbegin(), v.crend());

	v = t[V(1)()(2)];
	print(v.begin(), v.end());
	print(v.rbegin(), v.rend());
	print(v.cbegin(), v.cend());
	print(v.crbegin(), v.crend());

	v = t[V()()(-2)];
	print(v.begin(), v.end());
	print(v.rbegin(), v.rend());
	print(v.cbegin(), v.cend());
	print(v.crbegin(), v.crend());

	v = t[V(-2)()(-2)];
	print(v.begin(), v.end());
	print(v.rbegin(), v.rend());
	print(v.cbegin(), v.cend());
	print(v.crbegin(), v.crend());

	v = t[V()()(-3)];
	print(v.begin(), v.end());
	print(v.rbegin(), v.rend());
	print(v.cbegin(), v.cend());
	print(v.crbegin(), v.crend());
}
