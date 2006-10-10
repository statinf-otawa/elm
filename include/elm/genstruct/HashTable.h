/*
 * $Id$
 * Copyright (c) 2004, IRIT-UPS.
 *
 * elm/genstruct/HashTable.h -- HashTable class interface.
 */
#ifndef ELM_GENSTRUCT_HASHTABLE_H
#define ELM_GENSTRUCT_HASHTABLE_H

#include <elm/Iterator.h>
#include <elm/utility.h>

namespace elm { namespace genstruct {

// InHashTable class
template <class K, class T>
class HashTable {
	
	typedef struct node_t {
		node_t *next;
		K key;
		T value;
	} node_t;
	
	HashKey<K>& hkey;
	int size;
	node_t **tab;
	node_t *find(const K& key);
	
	// Iterator
	class Iterator {
		const HashTable<K, T>& htab;
		int i;
	protected:
		node_t *node;
	public:
		inline Iterator(const HashTable<K, T>& htab);
		inline bool ended(void) const;
		inline void next(void);
	};
	
public:
	HashTable(HashKey<K>& _hkey = HashKey<K>::def, int _size = 211);
	inline ~HashTable(void);
	
	bool isEmpty(void) const;
	int count(void) const;
	
	inline Option<T> get(const K& key);
	const T get(const K& key, const T& def_value);
	bool hasKey(const K& key);
	inline bool exists(const K& key) { return hasKey(key); };
	
	void put(const K& key, const T& value);
	void add(const K& key, const T& value);
	void putAll(const HashTable<K, T>& htab);
	void remove(const K& key);
	void clear(void);

	// KeyIterator
	class KeyIterator: public Iterator, public PreIterator<KeyIterator, K> {
	public:
		inline KeyIterator(const HashTable<K, T>& htab): Iterator(htab) { };
		inline K item(void) const { return this->node->key; }
	};

	// ItemIterator
	class ItemIterator: public Iterator, public PreIterator<ItemIterator, T> {
	public:
		inline ItemIterator(const HashTable<K, T>& htab): Iterator(htab) { };
		inline T item(void) const { return this->node->value; }
		inline const K& key(void) const { return this->node->key; };
	};
};


// HashTable methods
template <class K, class T>
HashTable<K, T>::HashTable(HashKey<K>& _hkey, int _size)
: hkey(_hkey), size(_size), tab(new node_t *[_size]) {
	for(int i = 0; i < size; i++) tab[i] = 0;
}
template <class K, class T>
inline HashTable<K, T>::~HashTable(void) {
	clear();
	delete [] tab;
}
template <class K, class T>
void HashTable<K, T>::clear(void) {
	for(int i = 0; i < size; i++) {
		for(node_t *cur = tab[i], *next; cur; cur = next) {
			next = cur->next;
			delete cur;
		}
		tab[i] = 0;
	}
}
template <class K, class T>
bool HashTable<K, T>::isEmpty(void) const {
	for(int i = 0; i <size; i++)
		if(tab[i])
			return false;
	return true;
}
template <class K, class T>
struct HashTable<K, T>::node_t *HashTable<K, T>::find(const K& key) {
	int i = hkey.hash(key) % size;
	for(node_t *node = tab[i], *prev = 0; node; prev = node, node = node->next)
		if(node->key == key) {
			if(prev) {
				prev->next = node->next;
				node->next = tab[i];
				tab[i] = node;
			}
			return node;
		}
	return 0;
}

template <class K, class T>
void HashTable<K, T>::add(const K& key, const T& value) {
	int i = hkey.hash(key) % size;
	node_t *node = new node_t;
	node->next = tab[i];
	tab[i] = node;
	node->key = key;
	node->value = value;
}

template <class K, class T>
void HashTable<K, T>::put(const K& key, const T& value) {
	node_t *node = find(key);
	if(node)
		node->value = value;
	else
		add(key, value);
}

template <class K, class T>
void HashTable<K, T>::remove(const K& key) {
	int i = hkey.hash(key) % size;
	for(node_t *node = tab[i], *prev = 0; node; prev = node, node = node->next)
		if(node->key == key) {
			if(prev)
				prev->next = node->next;
			else
				tab[i] = node->next;
			delete node;
			break;
		}
}
template <class K, class T>
int HashTable<K, T>::count(void) const {
	int cnt = 0;
	for(int i = 0; i < size; i++)
		for(node_t *cur = tab[i]; cur; cur = cur->next)
			cnt++;
	return cnt;
}

template <class K, class T>
inline Option<T> HashTable<K,T>::get(const K& key)
		{ node_t *node = find(key); return node ? Option<T>(node->value) : Option<T>(); };

template <class K, class T>
const T HashTable<K,T>::get(const K& key, const T& def_value) {
	node_t *node = find(key);
	return node ? node->value : def_value;
}

template <class K, class T>
bool HashTable<K, T>::hasKey(const K& key) {
	node_t *node = find(key);
	return node != 0;
}

template <class K, class T>
void HashTable<K, T>::putAll(const HashTable<K, T>& htab) {
	for(int i = 0; i < htab.size; i++)
		for(node_t *node = htab.tab[i]; node; node = node->next) 
			put(node->key, node->value);
}


// Iterator methods
template <class K, class T>
HashTable<K, T>::Iterator::Iterator(const HashTable<K, T>& _htab):
htab(_htab) {
	for(i = 0; i < htab.size; i++)
		if(htab.tab[i]) {
			node = htab.tab[i];
			break;
		}
}

template <class K, class T> bool HashTable<K, T>::Iterator::ended(void) const {
	return i >= htab.size;
}

template <class K, class T> void HashTable<K, T>::Iterator::next(void) {
	node = node->next;
	if(!node)
		for(i++; i < htab.size; i++)
			if(htab.tab[i]) {
				node = htab.tab[i];
				break;
			}
}

	
} }	// elm::genstruct

#endif	// ELM_GENSTRUCT_H
