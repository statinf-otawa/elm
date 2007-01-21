/*
 * $Id$
 * Copyright (c) 2007 IRIT-UPS <casse@irit.fr>
 * 
 * checksum::Fletcher class interface
 */ 
#ifndef ELM_CHECKSUM_FLETCHER_H
#define ELM_CHECKSUM_FLETCHER_H

#include <elm/io/InStream.h>

namespace elm { namespace checksum {

// Fletcher class
class Fletcher {
	unsigned long sum1, sum2;
	unsigned long len;
	char half[2];
	unsigned short size;
	inline void shuffle(void);
	inline void add(void);
public:
	Fletcher(void);
	unsigned long sum(void);
	void put(io::InStream& in);
	void put(io::InStream& in, int length);
	void put(const void *buffer, int length);
	void put(const CString& string);
	void put(const String& string);
	inline Fletcher& operator<<(const char *string) { put(CString(string)); }
	inline Fletcher& operator<<(const CString& string) { put(string); }
	inline Fletcher& operator<<(const String& string) { put(string); }
	template <class T> inline Fletcher& operator<<(const T& value)
		{ put(&value, sizeof(T)); }
};

} } // elm::checksum

#endif // ELM_CHECKSUM_FLETCHER_H
 