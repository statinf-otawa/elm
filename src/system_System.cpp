/*
 *	$Id$
 *	System class implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-08, IRIT UPS.
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

#if defined(__LINUX)
#include <dlfcn.h>
#elif defined(__WIN32)
#include <windows.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <elm/assert.h>
#include <elm/deprecated.h>
#include <elm/io/RandomAccessStream.h>
#include <elm/system/System.h>
#include <elm/system/SystemException.h>

namespace elm {
namespace system {

/**
 * @defgroup system_inter System Interface
 *
 * These classes provides a unified interface to the OS services.
 *
 * @section file_man File Management
 *
 * The first file management class is @ref Path that is used to represent file
 * paths and provides many operations:
 * @li appending path,
 * @li extracting / setting file and directory part,
 * @li getting absolute form,
 * @li getting and setting extension.
 *
 * It provides also system information about paths:
 * @li getting / setting current path,
 * @li getting home directory,
 * @li getting path item separator,
 * @li getting path separator in path lists.
 *
 * Other classes represent files themselves. They are derivated from the
 * @ref FileItem class and may be either @ref Directory or @ref File. A file
 * item is obtained by the @ref FileItem::get() method taking the path of
 * looked file. The file item gives the information as name, path, type and
 * access rights. The files provides also information about their size while
 * directories content may be retrieved using the @ref Directory::Iterator or
 * made using the @ref Directory::make method. Once their use is completed,
 * this objects must be released with the method @ref FileItem::release().
 *
 * @section proc_man Process Management
 *
 * The @ref Process objects provides control on the process of the OS:
 * @li getting liveness,
 * @li getting return code after death,
 * @li killing,
 * @li waiting until end of the process.
 *
 * A process may be created using the @ref ProcessBuilder class. It allows
 * to set process configuration like arguments and standard IO. Once the
 * method ProcessBuilder::run() is called, the process is created, either
 * a duplication of the current one, or a new command,
 * with the set configuration and a process object is returned.
 *
 * @section misc_ser Miscalleneous Classes
 *
 * First, ELM provides a class to measure times as precisely as possible on
 * the current OS: the class @ref StopWatch.
 *
 * Then, the static class @ref System provides some services like:
 * @li @ref System::pipe() -- to create piped streams.
 */

/**
 * @class  PipeInStream
 * A system stream implementing the input end of a pipe.
 */

/**
 * Create a pipe stream.
 * @param fd	Unix file descriptor.
 */
#if defined(__LINUX)
PipeInStream::PipeInStream(int fd): SystemInStream(fd) {
}
#elif defined(_WIN32_WINNT)
PipeInStream::PipeInStream(HANDLE fd): SystemInStream(fd) {
}
#endif

/**
 * Delete the pipe stream.
 */
PipeInStream::~PipeInStream(void) {
#if defined(__LINUX)
	close(fd());
#elif defined(_WIN32_WINNT)
	TerminateProcess(fd(),1);
#else
#error "System Unsupported"
#endif
}

/**
 * @class  PipeOutStream
 * A system stream implementing the output end of a pipe.
 */

/**
 * Build a pipe output stream.
 * @param fd	Unix file descriptor.
 */
#if defined(__LINUX)
PipeOutStream::PipeOutStream(int fd): SystemOutStream(fd) {
}
#elif defined(__WIN32) || defined(__WIN64)
PipeOutStream::PipeOutStream(HANDLE fd): SystemOutStream(fd) {
}
#endif

/**
 * Delete the pipe stream.
 */
PipeOutStream::~PipeOutStream(void) {
#if defined(__LINUX)
	close(fd());
#elif defined(__WIN32)
	TerminateProcess(fd(),1);
#else
#error "System Unsupported"
#endif
}

/**
 * @class System
 * Non-instatiable object giving access to system facilities.
 * @ingroup system_inter
 */

/**
 * Create a  pipe with input / output end streams.
 * @return	Linked streams.
 * @throws	System exception.
 */
Pair<PipeInStream *, PipeOutStream *> System::pipe(void)
throw (SystemException) {
#if defined(__LINUX)
	int fds[2];
	// Create the pair
	if(::pipe(fds) < 0) {
		ASSERT(errno != EFAULT);
		throw SystemException(errno, "pipe creation");
	}

	/* Configure the close-on-exec flag
	 * !!WARNING!! added to implement a working ProcessBuilder: I'm not sure
	 * if it is the best way to do this (yet it is POSIX compliant). */
	fcntl(fds[0], F_SETFD, FD_CLOEXEC);
	fcntl(fds[1], F_SETFD, FD_CLOEXEC);

	// Return result
	return pair(
			new PipeInStream(fds[0]),
			new PipeOutStream(fds[1]));
#elif defined(__WIN32) || defined(__WIN64)
	HANDLE fdsread = NULL;
	HANDLE fdswrite = NULL;

	if(CreatePipe(&fdsread,&fdswrite,NULL,0) == 0)
		throw SystemException(errno, "pipe creation");

	/* Apparently under windows the FD_CLOXEXC is useless */

	return pair(
			new PipeInStream(fdsread),
			new PipeOutStream(fdswrite));
#else
#error "Unsupported System"
#endif
}

/**
 * Generate an integer random number in interval [0, top[.
 * @param top	Maximum exclusive value.
 * @return		Random number.
 */
unsigned int System::random(unsigned int top) {
	static bool init = false;
	if (!init) {
		srand( time(NULL));
		init = true;
	}
	int op = rand();
	return int(double(op) * top / RAND_MAX);
}

/**
 * Create a new file and open it to write.
 * The created file must be fried by the caller (causing the file closure).
 * @param path	Path of the file to open.
 * @return		Opened file.
 * @throws		SystemException	Thrown if there is an error.
 */
io::OutStream *System::createFile(const Path& path) throw (SystemException) {
#if defined(__LINUX)
	int fd = ::open(&path.toString(), O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if(fd == -1)
		throw SystemException(errno, "file creation");
	return new SystemOutStream(fd);
#elif defined(__WIN32) || defined(__WIN64)
	HANDLE fd;
	fd=CreateFile(&path.toString(),
			GENERIC_READ,
			FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if(fd == INVALID_HANDLE_VALUE)
		throw SystemException(errno, "file creation");
	return new SystemOutStream(fd);
#else
#error "System Unsupported"
#endif
}

/**
 * Open a file for reading.
 * The opened file must be fried by the caller (causing the closure).
 * @param path	Path of the file to open.
 * @return		Opened file.
 * @throws		SystemException	Thrown if there is an error.
 */
io::InStream *System::readFile(const Path& path) throw(SystemException) {
#if defined(__UNIX)
	int fd = ::open(&path.toString(), O_RDONLY);
	if(fd == -1)
		throw SystemException(errno, "file reading");
	return new SystemInStream(fd);
#elif defined(__WIN32) || defined(__WIN64)
	HANDLE fd;
	fd=CreateFile(&path.toString(),
			GENERIC_READ,
			FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if(fd == INVALID_HANDLE_VALUE)
		throw SystemException(errno, "file creation");
	return new SystemInStream(fd);
#else
#error "System Unsupported"
#endif
}

/**
 * Open a file for appending write.
 * The opened file must be fried by the caller (causing the closure).
 * @param path	Path of the file to open.
 * @return		Opened file.
 * @throws		SystemException	Thrown if there is an error.
 */
io::OutStream *System::appendFile(const Path& path) throw(SystemException) {
#if defined(__UNIX)
	int fd = ::open(&path.toString(), O_APPEND | O_CREAT | O_WRONLY, 0777);
	if(fd == -1)
		throw SystemException(errno, "file appending");
	return new SystemOutStream(fd);
#elif defined(__WIN32) || defined(__WIN64)
	HANDLE fd;
	fd=CreateFile(&path.toString(),
			GENERIC_READ,
			FILE_APPEND_DATA,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if(fd == INVALID_HANDLE_VALUE)
		throw SystemException(errno, "file creation");
	return new SystemOutStream(fd);
#else
#error "System Unsupported"
#endif
}

#if defined(__UNIX)
// UnixRandomAccessStream class
class UnixRandomAccessStream: public io::RandomAccessStream {
public:
	inline UnixRandomAccessStream(int _fd): fd(_fd) {}
	virtual ~UnixRandomAccessStream(void)
	{	close(fd);}

	virtual int read(void *buffer, int size)
	{	return ::read(fd, buffer, size);}
	virtual int write(const char *buffer, int size)
	{	return ::write(fd, buffer, size);}
	cstring lastErrorMessage(void)
	{	DEPRECATED; return strerror(errno);}
	int flush(void)
	{	return 0;}

	virtual pos_t pos(void) const
	{	return lseek(fd, 0, SEEK_CUR);}
	virtual size_t size(void) const
	{	struct stat s; fstat(fd, &s); return s.st_size;}
	virtual bool moveTo(pos_t pos)
	{	return (pos_t)lseek(fd, pos, SEEK_SET) == pos;}
	virtual bool moveForward(pos_t pos)
	{	return (pos_t)lseek(fd, pos, SEEK_CUR) == pos;}
	virtual bool moveBackward(pos_t _pos)
	{	return (pos_t)lseek(fd, pos() - _pos, SEEK_SET) == _pos;}

private:
	int fd;
};

// Build the flags
static inline int makeFlags(System::access_t access) {
	switch(access) {
	case System::READ: return O_RDONLY;
	case System::WRITE: return O_WRONLY;
	case System::READ_WRITE: return O_RDWR;
	default: ASSERT(false); return 0;
	}
}
#elif defined(__WIN32)
class WinRandomAccessStream: public io::RandomAccessStream {
public:
	inline WinRandomAccessStream(int _fd) : fd(_fd){
		//this->fd=_open_osfhandle((long)_fd,O_RDWR);
	}
	virtual ~WinRandomAccessStream(void)
	{	close(fd);}

	virtual int read(void *buffer, int size)
	{	return ::read(fd, buffer, size);}
	virtual int write(const char *buffer, int size)
	{	return ::write(fd, buffer, size);}
	cstring lastErrorMessage(void)
	{	DEPRECATED; return strerror(errno);}
	int flush(void)
	{	return 0;}

	virtual pos_t pos(void) const
	{	return lseek(fd, 0, SEEK_CUR);}
	virtual size_t size(void) const
	{	struct stat s;
	fstat(fd, &s); return s.st_size;}
	virtual bool moveTo(pos_t pos)
	{	return (pos_t)lseek(fd, pos, SEEK_SET) == pos;}
	virtual bool moveForward(pos_t pos)
	{	return (pos_t)lseek(fd, pos, SEEK_CUR) == pos;}
	virtual bool moveBackward(pos_t _pos)
	{	return (pos_t)lseek(fd, pos() - _pos, SEEK_SET) == _pos;}

private:
	int fd;
};

// Build the flags
static inline int makeFlags(System::access_t access) {
	switch(access) {
	case System::READ: return O_RDONLY;
	case System::WRITE: return O_WRONLY;
	case System::READ_WRITE: return O_RDWR;
	default: ASSERT(false); return 0;
	}
}
#else
#error "System Unsupported"
#endif

/**
 * Open a random access stream from a file.
 * @param path			Path of the file to open.
 * @param access		Type of access (one of READ, WRITE, READ_WRITE).
 * @return				Opened file.
 * @throws IOException	Thrown if there is an error.
 */
io::RandomAccessStream *System::openRandomFile(
		const system::Path& path,
		access_t access )
throw(SystemException)
{
	int fd = ::open(&path.toString(), makeFlags(access));
	if(fd < 0)
		throw SystemException(errno, _ << "cannot open \"" << path << "\"");
	else
		return new WinRandomAccessStream(fd);
};

/**
 * Create a random access stream from a file, removing it if it already exists.
 * @param path			Path of the file to open.
 * @param access		Type of access (one of READ, WRITE, READ_WRITE).
 * @return				Opened file.
 * @throws IOException	Thrown if there is an error.
 */
io::RandomAccessStream *System::createRandomFile(
		const system::Path& path,
		access_t access)
throw(SystemException)
{
	ASSERTP(access != READ, "file creation requires at least a write mode");
	int fd = ::open(&path.toString(), makeFlags(access) | O_CREAT | O_TRUNC, 0666);
	if(fd < 0)
		throw SystemException(errno, _ << "cannot create \"" << path << "\"");
	else
		return new WinRandomAccessStream(fd);
	return 0;
}

/**
 * Get the path of the object item (library, program) containing the symbol
 * whose address is given.
 * @param address	Address of the looked symbol.
 * @return			Path to the object containing the symbol or an empty path
 * 					if it can not be found.
 * @notice			This method gives a usable result only when the GLIBC is used.
 * 					Be careful: it may be hard to get object path of an external symbol
 * 					due to relocation function stub and due to duplication of some small data.
 */
Path System::getUnitPath(void *address) {
#	ifdef __USE_GNU
	Dl_info info;
	if(!dladdr(address, &info))
		return "";
	else {
		/*cerr << "dli_fname = " << info.dli_fname << io::endl;
	 cerr << "dli_fbase = " << info.dli_fbase << io::endl;
	 cerr << "dli_sname = " << info.dli_sname << io::endl;
	 cerr << "dli_saddr = " << info.dli_saddr << io::endl;*/
		return info.dli_fname;
	}
#	else
	return "";
#	endif
}

/**
 * Get an environment variable value.
 * @param key	Key name of the environment variable.
 * @return		Value of the environment variable or an empty string.
 */
cstring System::getEnv(cstring key) {
	char *res = getenv(&key);
	if(!res)
		return "";
	else
		return res;
}

/**
 * Test if an environment variable is defined.
 * @param key	Key name of the environment variable.
 * @return		True if the environment variable is defined, false else.
 */
bool System::hasEnv(cstring key) {
	return getenv(&key);
}

}} // elm::system
