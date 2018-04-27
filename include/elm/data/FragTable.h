/*
 *	FragTable class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
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
#ifndef ELM_DATA_FRAGTABLE_H
#define ELM_DATA_FRAGTABLE_H

#include <elm/assert.h>
#include "Manager.h"
#include "Vector.h"

namespace elm {

// FragTable class
template <class T, class M = EquivManager<T> >
class FragTable {
	typedef EquivManager<T *, Equiv<T *>, typename M::alloc_t> vec_equiv_t;
public:
	typedef FragTable<T, M> self_t;

	inline FragTable(int size_pow = 8, M& man = Single<M>::_)
		: vec_equiv(Single<Equiv<T *> >::_, man.alloc), tab(8, vec_equiv), _man(man), size(1 << size_pow), msk(size - 1), shf(size_pow), used(size)
		{ ASSERTP(size_pow > 0, "size must be greater than 0"); }
	inline FragTable(const FragTable& t)
		: vec_equiv(Single<Equiv<T *> >::_, t._man.alloc), tab(8, vec_equiv), _man(t._man), size(t.size), msk(size - 1), shf(t.shf), used(0)
		{ addAll(tab); }
	inline ~FragTable(void) { clear(); }
	inline M& manager(void) const { return tab.manager(); }

	class Iter: public PreIterator<Iter, const T&> {
	public:
		inline Iter(void): arr(0), i(0), len(0) { }
		inline Iter(const FragTable<T, M>& array, int pos = 0): arr(&array), i(pos), len(array.count()) { }
		inline void next(void) { ASSERT(i < len); i++; }
		inline const T& item(void) const { return arr->get(i); }
		inline bool ended(void) const { return i >= len; }
		inline bool equals(const Iter& it) const { return arr == it.arr && i == it.i; }
	protected:
		friend class FragTable;
		const FragTable<T, M> *arr;
		int i, len;
	};
 	
	// Collection concept
	inline int count (void) const { return length(); }
	inline bool contains(const T &v) const
		{ for(int i = 0; i < length(); i++) if(get(i) == v) return true; return false; }
	template <class C> bool containsAll(const C& c) const
		{ for(typename C::Iter i = c.items(); i; i++) if(!contains(*i)) return false; return false; }
	inline bool isEmpty(void) const { return tab.count() == 0; }
 	inline operator bool (void) const { return !isEmpty(); }
 	inline Iter items(void) const { return Iter(*this); }
 	inline Iter operator*(void) const { return items(); }
 	inline operator Iter(void) const { return items(); }
 	inline Iter begin(void) const { return items(); }
 	inline Iter end(void) const { return Iter(*this, count()); }
 	inline bool equals(const self_t& t) const
 		{ Iter i = begin(), j = t.begin(); for(; i && j; i++, j++) if(*i != *j) return false; return !i && !j; }
 	inline bool operator==(const self_t& t) const { return equals(t); }
 	inline bool operator!=(const self_t& t) const { return !equals(t); }
 	inline bool contains(const self_t& t) const
 		{ for(Iter i = t.begin(); i; i++) if(!contains(*i)) return false; return true; }
 	inline bool operator<=(const self_t& t) { return t.contains(*this); }
 	inline bool operator<(const self_t& t) { return t.contains(*this) && !equals(t); }
 	inline bool operator>=(const self_t& t) { return contains(t); }
 	inline bool operator>(const self_t& t) { return contains(t) && !equals(t); }

	// MutableCollection concept
	inline void clear(void)
		{ for(int i = 0; i < tab.count(); i++) delete [] tab[i]; tab.clear(); used = size; }
	inline void add(const T &value)
		{	if(used >= size) { tab.add(new T[size]); used = 0; }
			tab[tab.length() - 1][used++] = value; }
	template <template <class _> class C >
	void addAll(const C<T> &items)
		{ for(typename C<T>::Iterator i(items); i; i++) add(i); }
	inline void remove(const T &item)
		{ for(Iter i(*this); i; i++) if(i == item) { remove(i); break; } }
	template <template <class _> class C >
	void removeAll(const C<T> &items)
		{ for(typename C<T>::Iterator i(items); i; i++) remove(i); }
	inline void remove(const Iter &iter) { removeAt(iter.i); }
	inline self_t& operator+=(const T& v) { add(v); return *this; }
	inline self_t& operator-=(const T& v) { remove(v); return *this; }

	// Array concept
	inline int length(void) const { return ((tab.count() - 1) << shf) + used; }
	inline const T& get(int index) const
		{ ASSERTP(index >= 0 && index < length(), "index out of bounds"); return tab[index >> shf][index & msk]; }
	inline int indexOf(const T &value, int start = 0) const
		{ for(Iter i(*this, start); i; i++)
			if(i == value) return i.i; return -1; }
	inline int lastIndexOf(const T &value, int start = -1) const
		{ for(int i = (start < 0 ? length() : start) - 1; i >= 0; i--)
			if(get(i) == value) return i; return -1; }
	inline const T& operator[](int index) const { return get(index); }
	
	// MutableArray concept
	void shrink(int length)
		{	ASSERTP(length < this->length(), "length too big"); int nl = (length + msk) >> shf;
			for(int i = nl; i < tab.count(); i++) delete [] tab[i];
			tab.setLength(nl); used = length & msk; if(!used) used = size;  }
	inline void set(int index, const T &value)
		{ ASSERTP(index >= 0 && index < length(), "index out of bounds"); tab[index >> shf][index & msk] = value; }
	void set(const Iter &iter, const T &item) { set(iter.i, item); }
	inline T &get(int index)
		{ ASSERTP(index >= 0 && index < length(), "index out of bounds"); return tab[index >> shf][index & msk]; }
	inline T &operator[](int index) { return get(index); }
	void insert(int index, const T &item)
		{ 	ASSERTP(index >= 0 && index <= length(), "index out of bounds");
			int len = length(); alloc(1); for(int i = len - 1; i >= index; i--) set(i + 1, get(i)); set(index, item); }
	inline void insert(const Iter &iter, const T &item)
		{ insert(iter.i, item); }
	void removeAt(int index)  {
		int len = length(); for(int i = index + 1; i < len; i++) set(i - 1, get(i));
		used--; if(!used) { delete [] tab[tab.count() - 1]; tab.setLength(tab.count() - 1); used = size; }
	}
	inline void removeAt(const Iter &iter) { removeAt(iter.i); }

	// other methods
	int alloc(int count)
		{	int res = length(); while(count >= size - used) { count -= size - used; tab.add(new T[size]); used = 0; }
			used += count; return res; }

private:
	vec_equiv_t vec_equiv;
	Vector<T *> tab;
	M& _man;
	int size, msk, shf, used;
};

template <class T, class M>
inline bool operator<=(const T& v, const FragTable<T, M>& t) { return t.contains(v); }

} // elm

#endif	// ELM_DATA_FRAGTABLE_H
