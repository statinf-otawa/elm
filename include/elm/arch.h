/*
 *	information about the architecture
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2007-16, IRIT UPS.
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

#ifndef ELM_ARCH_H
#define ELM_ARCH_H

#define ELM_LITTLE_ENDIAN

namespace elm { namespace t {

	typedef signed char int8;
	typedef unsigned char uint8;
	typedef short int16;
	typedef unsigned short uint16;
	typedef int int32;
	typedef unsigned int uint32;
	typedef long int64;
	typedef unsigned long uint64;

	typedef uint64 size;
	typedef uint64 offset;
	typedef uint64 uint;
	typedef uint64 intptr;

} }	// elm::t

#endif	// ELM_ARCH_H
