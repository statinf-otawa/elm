/*
 * $Id$
 * Copyright (c) 2005, IRIT-UPS.
 *
 * src/system_Directory.cpp -- Directory class implementation.
 */

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <elm/system/Directory.h>
#include <elm/system/SystemException.h>

namespace elm { namespace system {

/**
 * @class Directory
 * This class represents a directory file item.
 */


/**
 * Build a new directory at the given path.
 * @param path	Path of the directory.
 * @return		Built directory.
 * @Throws		SystemException		When creation files.
 */
Directory *Directory::make(Path path) {
	
	// Build the directory
	if(mkdir(&path.toString(), 0777) < 0)
		throw new SystemException(errno, "file");
	
	// Get the file
	FileItem *item = FileItem::get(path);
	assert(item);
	Directory *dir = item->toDirectory();
	assert(dir);
	return dir;
}


/**
 */
Directory::Directory(Path path, ino_t inode): FileItem(path, inode) {
	assert(path);
}


/**
 */
Directory *Directory::toDirectory(void) {
	return this;
}


/**
 * @class Directory::Iterator
 * This iterator is used for visiting files in a directory.
 */


/**
 */
void Directory::Iterator::go(void) {
	struct dirent *dirent = readdir(dir);
	if(dirent)
		file = FileItem::get(path / Path(dirent->d_name));
	else if(errno)
		throw new SystemException(errno, "file");
	else
		file = 0;
}


/**
 */
Directory::Iterator::Iterator(Directory *directory)
: dir(0), file(0), path(directory->path()) {
	dir = opendir(&directory->path().toString());
	if(!dir)
		throw new SystemException(errno, "file");
	for(int i = 0; i < 3; i++)
		go();	// Skip "." and ".."
}


/**
 */
Directory::Iterator::~Iterator(void) {
	if(dir)
		closedir(dir);
	if(file)
		file->release();
}

/**
 */
bool Directory::Iterator::ended(void) const {
	return !file;
}


/**
 */
FileItem *Directory::Iterator::item(void) const {
	assert(file);
	return file;
}


/**
 */
void Directory::Iterator::next(void) {
	file->release();
	go();
}

} } // elm::system
