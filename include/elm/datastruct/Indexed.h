/*
 * $Id$
 * Copyright (c) 2004, Alfheim Corporation.
 *
 * elm/datastruct/Indexed.h -- index access interface.
 */
#ifndef ELM_DATASTRUCT_INDEXED_H
#define ELM_DATASTRUCT_INDEXED_H

#include <elm/datastruct/Indexed.h>

namespace elm { namespace datastruct {

// Indexed interface
template <class T> class Indexed {
public:
	virtual int length(void) const = 0;
	virtual T get(int index) const = 0;
	virtual void set(int index, const T value) = 0;
	virtual T& item(int index) = 0;
	inline T& operator[](int index);
	inline T operator[](int index) const;
	int indexOf(const T value, int start = 0) const;
	int lastIndexOf(const T value, int start = -1) const;	
};

// Inlines
template <class T> inline T& Indexed<T>::operator[](int index) {
	return item(index);
}
template <class T> inline T Indexed<T>::operator[](int index) const {
	return get(index);
}
template <class T> int Indexed<T>::indexOf(const T value, int start) const {
	for(int i = start; i < length(); i++)
		if(get(i) == value)
			return i;
	return -1;
}
template <class T> int Indexed<T>::lastIndexOf(const T value, int start) const {
	for(int i = (start < 0 ? length() : start) - 1; i >= 0; i--)
		if(get(i) == value)
			return i;
	return -1;
}

} } // elm::datastruct

#endif // ELM_DATASTRUCT_ARRAY_H