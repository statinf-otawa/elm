/*
 *	Thread class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2011-16, IRIT UPS.
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
#ifndef ELM_SYSTEM_THREAD_H_
#define ELM_SYSTEM_THREAD_H_

#include <elm/sys/SystemException.h>
#include <elm/util/GenValue.h>
#include <elm/util/MessageException.h>

namespace elm { namespace sys {

// pre-declaration
class Thread;

// ThreadException class
class ThreadException: public MessageException {
public:
	inline ThreadException(const string& message): MessageException(message) { }
};


// Runnable class
class Runnable {
public:
	virtual ~Runnable(void);
	virtual void run(void) = 0;
protected:
	void stop(void);
	inline Thread *current(void) const { return thr; }
private:
	Thread *thr;
};


// Thread class
class Thread {
	friend class Runnable;
	typedef void *key_t;
public:

	template <class T>
	class Key {
		friend class Thread;
	public:
		inline ~Key(void) { delKey(k); }
		inline void set(const T& val)  throw(ThreadException)
			{ Thread::set(k, new GenValue<T>(val)); }
		Option<T> get(void) const
			{ AbstractValue *v = Thread::get(k); if(!v) return none;
			else return some(static_cast<GenValue<T> *>(v)->value()); }
		inline void clean(void) const
			{ AbstractValue *v = Thread::get(k); if(v) { Thread::set(k, 0); delete v; } }
	private:
		inline Key(void): k(newKey()) { }
		key_t k;
	};

	virtual ~Thread(void);
	static Thread *make(Runnable& runnable);
	template <class T>
	inline static Key<T> *key(void) throw(ThreadException)
		{ return new Key<T>(); }

	virtual void start(void) throw(ThreadException) = 0;
	virtual void join(void) throw(ThreadException) = 0;
	virtual void kill(void) throw(ThreadException) = 0;
	virtual bool isRunning(void) = 0;


protected:
	Runnable& _runnable;
	Thread(Runnable& runnable);
	virtual void stop(void) = 0;

private:
	static key_t newKey(void) throw(ThreadException);
	static void delKey(key_t k);
	static AbstractValue *get(key_t k);
	static void set(key_t k, AbstractValue *val) throw(ThreadException);
};


class Mutex {
public:
	static Mutex *make(void) throw(SystemException);
	virtual ~Mutex(void);
	virtual void lock(void) throw(SystemException) = 0;
	virtual void unlock(void) throw(SystemException) = 0;
	virtual bool tryLock(void) throw(SystemException) = 0;
};

} }	// elm::sys

#endif /* ELM_SYSTEM_THREAD_H_ */
