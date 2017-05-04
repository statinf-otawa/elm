/*
 *	Data Manager classes
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
#ifndef ELM_DATA_MANAGER_H_
#define ELM_DATA_MANAGER_H_

#include <elm/alloc/DefaultAllocator.h>
#include <elm/compare.h>
#include <elm/util/Equiv.h>
#include <elm/util/HashKey.h>

namespace elm {

// EquivManager class
template <class T, class E = Equiv<T>, class A = DefaultAllocator>
class EquivManager {
public:
	inline EquivManager(E& e = Single<E>::_, A& a = A::DEFAULT): eq(e), alloc(a) { }
	E& eq;
	A& alloc;
};

// CompareManager class
template <class T, class C = Comparator<T>, class E = Equiv<T>, class A = DefaultAllocator>
class CompareManager {
public:
	inline CompareManager(const C& c = Single<C>::_, const E& e = Single<E>::_, A& a = DefaultAllocator::DEFAULT)
		: cmp(c), eq(e), alloc(a) { }

	inline int compare(const T& v1, const T& v2) const { return cmp.doCompare(v1, v2); }
	inline bool equals(const T& v1, const T& v2) const { return eq.isEqual(v1, v2); }
	inline void *allocate(t::size size) const { return alloc.allocate(size); }

	const C& cmp;
	const E& eq;
	A& alloc;
};


// HashManager class
template <class K, class H = HashKey<K>, class A = DefaultAllocator>
class HashManager {
public:
	inline HashManager(void): alloc(DefaultAllocator::DEFAULT) { }
	inline HashManager(const H& h, A& a = DefaultAllocator::DEFAULT): hash(h), alloc(a) { }
	H hash;
	A& alloc;
};


// CompareManager class
#if 0
template <class T, class A = IdAdapter<T>, class C = Comparator<T>, class TC = Comparator<T>, class A = DefaultAllocator>
class KeyCompareManager {
public:
	/*inline CompareMapManager(void): alloc(DefaultAllocator::DEFAULT) { }
	inline CompareMapManager(const KC& kc, const TC& tc, A& a = DefaultAllocator::DEFAULT): kcmp(kc), tcmp(tc), alloc(a) { }
	KC kcmp;
	TC tcmp;
	A& alloc;*/
};
#endif

}	// elm

#endif /* ELM_DATA_MANAGER_H_ */
