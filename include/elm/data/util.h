/*
 *	utilities for data module
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2016, IRIT UPS.
 *
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_DATA_UTIL_H_
#define ELM_DATA_UTIL_H_

#include <elm/PreIterator.h>
#include <elm/meta.h>
#include <elm/type_info.h>
#include <elm/types.h>

#include "util.h"

namespace elm {

// simple functions
template <class T>
struct Add {
	typedef T x_t;
	typedef T y_t;
	inline T operator()(const T& a, const T& b) const { return a + b; }
	static T null;
};
template <class T> struct scalar_zero
	{ static inline T zero(void) { return 0; } };
template <class T> struct class_zero
	{ static inline T zero(void) { return T::zero; } };
template <class T> T Add<T>::null = _if<type_info<T>::is_scalar, scalar_zero<T>, class_zero<T> >::zero();

template <class T>
struct Mul {
	typedef T x_t;
	typedef T y_t;
	inline T operator()(const T& a, const T& b) const { return a * b; }
	static T null;
};
template <class T> struct scalar_one
	{ static inline T one(void) { return 1; } };
template <class T> struct class_one
	{ static inline T one(void) { return T::zero; } };
template <class T> T Mul<T>::null = _if<type_info<T>::is_scalar, scalar_one<T>, class_one<T> >::one();

template <class T> struct true_pred
	{ inline bool operator()(const T& v) { return true; } };


// count operations
template <class C, class P>
inline int count(const C& c, const P& p)
	{ int r = 0; for(const auto& i: c) if(p(i)) r++; return r; }


// forall operations
template <class C, class P>
inline bool forall(const C& c, const P& p)
	{ for(const auto& i: c) if(!p(i)) return false; return true; }


// exists operations
template <class C, class P>
inline bool exists(const C& c, const P& p)
	{ for(const auto& i: c) if(p(i)) return true; return false; }


// find operation
template <class C, class P>
inline typename C::Iter find(const C& c, const P& p)
	{ typename C::Iter i = c.begin(); for(; i != c.end(); i++) if(p(i)) break; return i; }


// map operation
template <class C, class F, class D>
inline void map(const C& c, const F& f, D& d)
	{ for(const auto& i: c) d.add(f(i)); }


// iter operation
template <class C, class F>
inline void iter(const C& c, const F& f)
	{ for(const auto& i: c) f(i); }


// fold operation
template <class C, class F, class T>
inline T fold(const C& c, const F& f, T t)
	{ for(const auto& i: c) t = f(i, t); return t; }


// useful function
template <class C>
inline typename C::t sum(const C& c)
	{ return fold(c, [](const typename C::t& i, const typename C::t& s) { return i + s; }, typename C::t(0)); }
template <class C>
inline typename C::t product(const C& c)
	{ return fold(c, [](const typename C::t& i, const typename C::t& p) { return i * p; }, typename C::t(1)); }


// construction operations
template <class C>
inline void fill(C& c, int n, const typename C::t v = type_info<typename C::t>::null)
	{ for(int i = 0; i < n; i++) c.add(v); }


// NumIter class
template <class T>
class NumIter: public PreIterator<NumIter<T>, T> {
public:
	inline NumIter(T i, T j, T s = 1): _i(i), _j(j), _s(s) { }
	inline bool ended() const { return _i <= _j; }
	inline T item() const { return _i; }
	inline void next() { _i += _s; }
	inline bool equals(const NumIter<T>& i) const { return _i == i._i; }
private:
	T _i, _j, _s;
};
template <class T>
class NumRange {
public:
	typedef T t;
	inline NumRange(T i, T j, T s = 1): _i(i), _j(j), _s(s) { }
	inline NumIter<T> begin() const { return NumIter<T>(_i, _j, _s); }
	inline NumIter<T> end() const { return NumIter<T>(_j + 1, _j, _s); }
private:
	int _i, _j, _s;
};
template <class T> NumRange<T> nrange(T i, T j, T s = 1) { return NumRange<T>(i, j, s); }


// range class
template <class I>
class Range {
public:
	typedef typename I::t t;
	typedef I Iter;
	inline Range(const I& b, const I& e): _b(b), _e(e) { }
	inline const I& begin() const { return _b; }
	inline const I& end() const { return _e; }
private:
	I _b, _e;
};
template <class I> Range<I> range(const I& b, const I& e) { return Range<I>(b, e); }


// select classes
template <class I, class P>
class SelectIter: public PreIterator<SelectIter<I, P>, typename I::t> {
public:
	inline SelectIter(const I &i, const P& p): _i(i), _p(p) { step(); }
	inline bool atEnd() const { return _i.atEnd(); }
	inline typename I::t item() const { return *_i; }
	inline void next() { _i++; step(); }
	inline bool equals(const SelectIter<I, P>& i) { return _i.equals(i._i); }
private:
	inline void step() { while(_i && !_p(*_i)) _i++; }
	I _i;
	P _p;
};
template <class I, class P>
inline SelectIter<I, P> select_iter(const I& i, const P& p) { return SelectIter<I, P>(i, p); }

template <class C, class P>
inline Range<SelectIter<typename C::Iter, P> > select(const C& c, const P& p)
	{ return range(select_iter(c.begin(), p), select_iter(c.end(), p)); }

} // elm

#endif /* ELM_DATA_UTIL_H_ */
