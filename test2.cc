#include <iostream>
#include <string>

#define DEBUG
#include "kirito.h"

#define I kirito::IndexBuilder

using std::cout;
using std::endl;
using std::string;

template <typename A, typename B, typename C>
int fn(kirito::Index<A, B, C> i) {
	cout << i.a << i.b << i.c << endl;
}

int main(int argc, char **argv) {
	return fn(I(1)(2)(3));
}
