//written by Michael Benoit

#ifndef _HASH_CPP
#define _HASH_CPP

#include "hash.h"
#include <vector>
#include <string>


hashTable::hashTable(int size) {
	data.reserve(60000);
	capacity = getPrime(size);
	filled = 0;
	data.resize(capacity);
}


int hashTable::insert(const std::string &key, void *pv) {
	int hashval = hash(key);
	if (filled == capacity*3/4) {
		if(!rehash())
			 return 2;
	}
	while(data[hashval].isOccupied) { //linear probing
		if(data[hashval].key == key  && !data[hashval].isDeleted)
			return 1;
		hashval++;
		if(hashval == capacity)
			hashval = 0;
	}
	data[hashval].key = key;
	data[hashval].isOccupied = true;
	data[hashval].isDeleted = false;
	data[hashval].pv = pv;
	filled++;
	return 0;
}

bool hashTable::contains(const std::string &key) {
	int pos = findPos(key);
	if(pos == -1) {
		return false;
	}
	return true;
}

void *hashTable::getPointer(const std::string &key, bool *b) {
	int pos = findPos(key);
	if(pos == -1) {
		if(b)
			*b = false;
		return NULL;
	}
	if(b)
		*b = true;
	return data[pos].pv;
}

bool hashTable::setPointer(const std::string &key, void *pv) {
	int pos = findPos(key);
	if(pos == -1)
		return false;
	data[pos].pv = pv;
	return true;
}

bool hashTable::remove(const std::string &key) {
	int pos = findPos(key);
	if(pos == -1)
		return false;
	data[pos].isDeleted = true;
	//--filled;
	return true;
}

// The hash function.
int hashTable::hash(const std::string &key) {
	int retval = 0;
	int factor = 1;
	int ksize = key.size();
	char letter;
	for(int p = 0; p < ksize; p++) {
		letter = tolower(key[p]);
		if(isalpha(letter)) {
			retval += factor * (key[p] - 'a');
		}
		if(key[p] == '-') {
			retval += factor * 26;
		}
		if(key[p] == '\'') {
			retval += factor * 27;
		}
		else {
			retval += factor * (key[p] - '0' + 28);
		}
		factor *= 28;
		if( p == 20)
			break;
	}
	if(retval > 0)
		return retval % capacity;
	else
		return (retval * -1) % capacity;
}

// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const std::string &key) {
	int first, hashval = first = hash(key); 
	do {
		if(key == data[hashval].key && !(data[hashval].isDeleted)) {
			return hashval;
		}
		hashval++;
		if(hashval == capacity)
				hashval =0;
	}while(data[hashval].isOccupied && hashval != first);
	return -1;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash() {
	int oldcap = capacity;
	capacity = getPrime(capacity*2);
	try {
		std::vector<hashItem> temp = data;
		data.clear();
		filled = 0;
		data.resize(capacity);
		for(int i = 0; i < oldcap; i++) {
			if(temp[i].isOccupied && !temp[i].isDeleted) {
				insert(temp[i].key, temp[i].pv);
			}
		}
	}
	catch(int e) {
		return false;
	}
	return true;
}

// Return a prime number at least as large as size.
// Uses a precomputed sequence of selected prime numbers.
unsigned int hashTable::getPrime(int size) {
	static unsigned int t;
	unsigned int primes[] = {5,11,23,53,131,359,743,1531,3191,6317,13633,27011,55807,111119,222247,444487,1118113,}; //array holding prime numbers
	while(primes[t] < size) t++;
	return primes[t];
}

#endif // HASH_CPP
