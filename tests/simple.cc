#include <iostream>
#include <string>
#include <vector>

#define DEBUG
#include <kirito.h>

using kirito::I;

using std::cout;
using std::endl;
using std::string;

constexpr auto N = I()()(3);

template <typename A, typename B, typename C>
void fn(kirito::Index<A, B, C> i) {
	cout << i.a << i.b << i.c << endl;
}

void printhelper(std::vector<int> &v) {
	std::cout << "YO ";
	for (auto a : v) {
		std::cout << a << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {
	fn(I(1)(2)(3));
	fn(I()()(1));
	fn(I(1)()(3));
	fn(I()(2)(3));
	fn(I(1)());
	fn(I()(2));
	for (int i = -3; i < 3; ++i) {
		fn(I(i)());
	}

	kirito::FancyContainer<std::vector<int>> a = {1, 2, 3, 4, 5, 6, 7};

	auto b = a[I()];
	fn(I());
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
