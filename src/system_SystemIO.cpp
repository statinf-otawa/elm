/*
 *	$Id$
 *	System stream classes implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-07, IRIT UPS.
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

#include <elm/system/SystemIO.h>
#if defined(__WIN32)
#include <windows.h>
#endif

namespace elm { namespace system {

/**
 * @class SystemInStream
 * This kind of stream is tied to a system input stream, that is, it may be
 * shared by processes or support system feature. @ref io::stdin is an example
 * of @ref SystemInStream.
 *  
 * @warning	Do not rely on the current inheritance of this class: it may change
 * according systems. The only right inheritance relation is with @ref io::InStream.
 */


/**
 * @class SystemOutStream
 * This kind of stream is tied to a system output stream, that is, it may be
 * shared by processes or support system feature. @ref io::stdout and
 * @ref io::stdout are examples of @ref SystemOutStream.
 *  
 * @warning	Do not rely on the current inheritance of this class: it may change
 * according systems. The only right inheritance relation is with @ref io::OutStream.
 */


// Standard Input
class StandardInStream: public SystemInStream {
public:
#if defined(__LINUX)
	inline StandardInStream(int fd): SystemInStream(fd) { };
	static StandardInStream stdin_object(0);
#elif defined(__WIN32) || defined(WIN64)
	inline StandardInStream(void* fd): SystemInStream(fd) { };
#endif
};
#if defined(__WIN32)
static StandardInStream stdin_object(GetStdHandle(STD_INPUT_HANDLE));
#endif

// Standard Output
class StandardOutStream: public SystemOutStream {
public:
#if defined(__LINUX)
	inline StandardOutStream(int fd): SystemOutStream(fd) { };
	static StandardOutStream stdout_object(1);
	static StandardOutStream stderr_object(2);
#elif defined(__WIN32) || defined(__WIN64)
	inline StandardOutStream(void* fd): SystemOutStream(fd) { };
#endif
};
#if defined(__WIN32)
static StandardOutStream stdout_object(GetStdHandle(STD_OUTPUT_HANDLE));
static StandardOutStream stderr_object(GetStdHandle(STD_ERROR_HANDLE));
#endif

} // system

namespace io {

/**
 * Standard input stream.
 * @ingroup ios
 */
//system::SystemInStream& stdin = system::stdin_object;

/**
 * Standard output stream.
 * @ingroup ios
 */
//system::SystemOutStream& stdout = system::stdout_object;

/**
 * Standard error stream.
 * @ingroup ios
 */
//system::SystemOutStream& stderr = system::stderr_object;

} } // elm::io
