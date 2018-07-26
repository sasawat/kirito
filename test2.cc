#include <iostream>
#include <string>

#define DEBUG
#include "kirito.h"

#define I kirito::IndexBuilder

using std::cout;
using std::endl;
using std::string;

template <typename A, typename B, typename C>
void fn(kirito::Index<A, B, C> i) {
	cout << i.a << i.b << i.c << endl;
}

int main(int argc, char **argv) {
	fn(I(1)(2)(3));
	fn(I()()(1));
	fn(I(1)()(3));
	fn(I()(2)(3));
	fn(I(1)());
	fn(I()(2));
}
