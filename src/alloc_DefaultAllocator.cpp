/*
 *	$Id$
 *	BadAlloc exception implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2009, IRIT UPS.
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

#include <new>
#include <elm/alloc/DefaultAllocator.h>

namespace elm {

/**
 * @defgroup alloc	Allocation Module
 *
 * This module binds together classes providing different ways to perform
 * memory allocation. This kind of class is usually passed to any container
 * class performing automatic memory allocation.
 *
 * The chosen allocator
 * is usually passed as a template parameter to the collection type and
 * as a parameter to the collection constructor.
 *
 * An allocator class must implement the concept @ref elm::concept::Allocator.
 *
 * A collection supporting allocator matches the concept below:
 * @code
 * template <class A>
 * class CollectionWithAllocation {
 * public:
 *		CollectionWithAllocation(A& allocator = A::DEFAULT);
 *	};
 * @endcode
 */


namespace concept {

/**
 * Allocator concept
 * Memory allocation.
 */
class Allocator {

	/**
	 * Allocate a block of the given size.
	 * @param size		Size in bytes of the allocated block.
	 * @throw BadAlloc	If the allocation fails.
	 */
	void *allocate(size_t size) throw(BadAlloc);

	/**
	 * Free a block previously allocated.
	 * @param block	Block to free.
	 */
	void free(void *block);
};
}


/**
 * @class BadAlloc
 * This exception is thrown when a memory allocation fails.
 * @ingroup alloc
 */


/**
 */
BadAlloc::~BadAlloc(void) { }


/**
 */
String 	BadAlloc::message(void) { return "allocation failed"; }


/**
 * @class DefaultAllocator
 * Default implementation of a memory allocator just calling new and delete.
 * @ingroup alloc
 */


/**
 * Default allocator.
 */
DefaultAllocator DefaultAllocator::DEFAULT;


/**
 * Allocate a memory block of the given size.
 * @param size	Size of the block to allocate.
 * @return		Allocated block.
 * @throw BadAlloc	Thrown if there is no more system memory.
 */
void *DefaultAllocator::allocate(t::size size) throw(BadAlloc) {
	try {
		return new char[size];
	}
	catch(std::bad_alloc& e) {
		throw BadAlloc();
	}
}


/**
 * @fn void DefaultAllocator::free(void *block);
 * Free the given block.
 * @param block	Block to free.
 */

} // elm
