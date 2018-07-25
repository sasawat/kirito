#include <cassert>
#include <iostream>

#include "kirito.h"

using kirito::vector;
using I = kirito::Index;

void printhelper(vector<int> &v) {
	for (auto a : v) {
		std::cout << a << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {
	vector<int> a = {1, 2, 3, 4, 5, 6, 7};
	auto b = a[I()];
	printhelper(b);
	auto c = a[I(1)()];
	printhelper(c);
	auto d = a[I()(1)];
	printhelper(d);
	auto e = a[I(1)(3)];
	printhelper(e);
	auto f = a[I()()(2)];
	printhelper(f);
	auto g = a[I(1)()(2)];
	printhelper(g);
	auto h = a[I()(4)(2)];
	printhelper(h);
	auto i = a[I(1)(5)(2)];
	printhelper(i);
}
