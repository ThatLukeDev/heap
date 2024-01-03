#ifndef HEAP_HEAP_HPP
#define HEAP_HEAP_HPP

#include <sys/mman.h>
#include <iostream>

static class {
	const unsigned int DEFAULT_SIZE = 4096;

	class chunk {
	public:
		unsigned int size = 0;
		bool free = true;
		chunk* next = nullptr;

		chunk(unsigned int _size) : size(_size) { }

		void split(unsigned int _size) {
			chunk* fragment = this + sizeof(*this) + _size;
			*fragment = chunk(size - _size - 2 * sizeof(*this));
			fragment->next = this->next;
			size = _size;
			next = fragment;
		}
	};

	chunk* start;

	char _init() {
		start = (chunk*)mmap(0, DEFAULT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		*start = chunk(DEFAULT_SIZE - sizeof(chunk));
		return (char)0;
	}
	char p_m = _init();
public:

	void* allocate(unsigned int _size) {
		for (chunk* v = start; v; v = v->next) {
			if (v->free && v->size >= _size) {
				v->split(_size);
				v->free = false;
				return (void*)v;
			}
		}
		return nullptr;
	}
	void free(void* ptr) {
		return;
	}
} heap;

#endif
