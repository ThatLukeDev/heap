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
		chunk* prev = nullptr;

		chunk(unsigned int _size) : size(_size) { }

		void split(unsigned int _size) {
			chunk* fragment = this + sizeof(*this) + _size;
			*fragment = chunk(size - _size - sizeof(*this));
			fragment->next = this->next;
			fragment->prev = this;
			size = _size;
			next = fragment;
		}
		chunk* zipFree() {
			if (!prev) {
				return this;
			}
			if (prev->free) {
				return prev->zipFree();
			}
			return this;
		}
		void defragForward() {
			if (next) if (next->free) {
				next->defragForward();
			}
			if (!prev) {
				return;
			}
			if (!prev->free) {
				return;
			}
			if (next) {
				next->prev = prev;
			}
			prev->size += sizeof(*this) + size;
			prev->next = next;
		}
		void defrag() {
			zipFree()->defragForward();
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

	void show() {
		std::clog << "[ ";
		for (chunk* v = start; v; v = v->next) {
			std::clog << v->size + sizeof(chunk) << " (" << v->size << " usable)";
			if (v->next) {
				std::clog << " | ";
			}
		}
		std::clog << " ]" << std::endl;
	}

	void* allocate(unsigned int _size) {
		for (chunk* v = start; v; v = v->next) {
			if (v->free && v->size >= _size) {
				v->split(_size);
				v->free = false;
				return (void*)(v + sizeof(*v));
			}
		}
		return nullptr;
	}

	void free(void* ptr) {
		for (chunk* v = start; v; v = v->next) {
			if (ptr == (void*)(v + sizeof(chunk))) {
				v->free = true;
				v->defrag();
				break;
			}
		}
	}

	template <typename T>
	T* allocate() {
		return (T*)allocate(sizeof(T));
	}
	template <typename T>
	void free(T ptr) {
		free((void*)ptr);
	}
} heap;

#endif
