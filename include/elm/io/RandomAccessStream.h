/*
 *	$Id$
 *	RandomAccessStream class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2008, IRIT UPS.
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
#ifndef ELM_IO_RANDOMACCESSSTREAM_H_
#define ELM_IO_RANDOMACCESSSTREAM_H_

#include <elm/io/InStream.h>
#include <elm/io/OutStream.h>
#include <elm/system/Path.h>

namespace elm { namespace io {

// RandomAccessStream class
class RandomAccessStream: public InStream, public OutStream  {
public:
	typedef unsigned long long pos_t;
	typedef enum access_t {
		READ,
		WRITE,
		READ_WRITE
	} access_t;
	
	virtual pos_t pos(void) = 0;
	virtual bool moveTo(pos_t pos) = 0;
	virtual bool moveForward(pos_t pos) = 0;
	virtual bool moveBackward(pos_t pos) = 0;
	virtual void resetPos(void) { moveTo(0); }
	static RandomAccessStream *openFile(const system::Path& path,
		access_t access = READ);
	static RandomAccessStream *createFile(const system::Path& path,
		access_t access = WRITE);
};

} } // elm::io

#endif /*ELM_IO_RANDOMACCESSSTREAM_H_*/
