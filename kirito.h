#ifndef KIRITO_H_
#define KIRITO_H_

#include <cstdint>
#include <vector>
#include <exception>
#include <algorithm>

namespace kirito {

using std::size_t;
using std::runtime_error;
using std::back_inserter;

struct Index {
	enum Type {
		x, a,
		xx, ax, xb, ab,
		xxc, axc, xbc, abc
	};
	Index(Type t, size_t a, size_t b, size_t c) : t_(t), a_(a), b_(b), c_(c) {}
	const Type t_;
	const size_t a_, b_, c_;
	Index() : Index(Type::x, 0, 0, 0)  {}
	Index(size_t s) : Index(Type::a, s, 0, 0) {}
	Index operator()() const {
		switch (t_) {
		case x:
			return {Type::xx, 0, 0, 0};
		case a:
			return {Type::ax, a_, 0, 0};
		default:
			throw runtime_error("bad type");
		}
	}
	Index operator()(size_t s) const {
		switch (t_) {
		case x:
			return {Type::xb, 0, s, 0};
		case a:
			return {Type::ab, a_, s, 0};
		case xx:
			return {Type::xxc, 0, 0, s};
		case ax:
			return {Type::axc, a_, 0, s};
		case xb:
			return {Type::xbc, 0, b_, s};
		case ab:
			return {Type::abc, a_, b_, s};
		default:
			throw runtime_error("bad type");
		}
	}
};

template <typename T>
class vector : public std::vector<T> {
public:
	using typename std::vector<T>::iterator;
	using std::vector<T>::begin;
	using std::vector<T>::end;
	using std::vector<T>::vector;
	vector<T> copyabc(iterator a, iterator b, size_t c) {
		vector<T> rv;
		for (auto iter = a; iter < b; iter += c) {
			rv.push_back(*iter);
		}
		return rv;
	}
	vector<T> operator[](const Index &ndx) {
		switch (ndx.t_) {
		case Index::Type::x:
			return copyabc(begin(), end(), 1);
		case Index::Type::ax:
			return copyabc(begin() + ndx.a_, end(), 1);
		case Index::Type::xb:
			return copyabc(begin(), begin() + ndx.b_, 1);
		case Index::Type::ab:
			return copyabc(begin() + ndx.a_, begin() + ndx.b_, 1);
		case Index::Type::xxc:
			return copyabc(begin(), end(), ndx.c_);
		case Index::Type::axc:
			return copyabc(begin() + ndx.a_, end(), ndx.c_);
		case Index::Type::xbc:
			return copyabc(begin(), begin() + ndx.b_, ndx.c_);
		case Index::Type::abc:
			return copyabc(begin() + ndx.a_, begin() + ndx.b_, ndx.c_);
		default:
			throw runtime_error("bad type");
		}
	}
};
}  // namespace kirito
#endif  // KIRITO_H_
