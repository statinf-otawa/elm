/*
 * $Id$
 * Copyright (c) 2004, Alfheim Corporation.
 *
 * src/InStream.cpp -- implementation for InStream class.
 */

#include <elm/io/InStream.h>

namespace elm { namespace io {

/**
 * @class InStream
 * This class must be shared by all classes implementing input streams. It provides common
 * facilities provided by input streams.
 */


/**
 * @fn InStream::~InStream(void);
 * Common virtual destructor.
 */


/**
 * @fn int InStream::read(char *buffer, int size);
 * Read some bytes from the stream and store them in the given buffer.
 * This method does not ever read as many bytes as available in the buffer even if the stream
 * is not ended. Insteadn a read returning 0 bytes means the end of stream is reached.
 * @param buffer	Buffer to write bytes in.
 * @param size	Size of the buffer.
 * @return		Number of read bytes, 0 for end of stream, -1 for an error.
 */


/**
 * Read one byte from the stream.
 * @return	Read byte, ERROR for an error, END for end of stream.
  */
int InStream::read(void) {
	char buffer[1];
	int result = read(buffer, 1);
	if(result < 0)
		return FAILED;
	else if(!result)
		return ENDED;
	else
		return (unsigned char)buffer[0];
}

} } // elm::io
