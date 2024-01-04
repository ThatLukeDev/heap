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
		unsigned int pid = 0;

		chunk(unsigned int _size) : size(_size) { }

		void split(unsigned int _size) {
			chunk* fragment = (chunk*)((long)this + (long)sizeof(chunk) + (long)_size);
			*fragment = chunk(size - _size - sizeof(chunk));
			fragment->next = this->next;
			fragment->prev = this;
			fragment->pid = pid;
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
			if (prev->pid != pid) {
				return;
			}
			prev->size += sizeof(chunk) + size;
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
		start->pid = (unsigned long)start;
		return (char)0;
	}
	char p_m = _init();

	void expand() {
		chunk* newChunk = (chunk*)mmap(start, DEFAULT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		*newChunk = chunk(DEFAULT_SIZE - sizeof(chunk));
		chunk* head = start;
		for (; head->next; head = head->next) ;
		newChunk->prev = head;
		head->next = newChunk;
		newChunk->pid = (unsigned long)newChunk;
	}

public:

	void show() {
		std::clog << "[ ";
		for (chunk* v = start; v; v = v->next) {
			std::clog << v->size + sizeof(chunk) << " (" << v->size << " usable)";
			if (v->next) {
				if (v->pid != v->next->pid) {
					std::clog << " | ";
				}
				else {
					std::clog << " + ";
				}
			}
		}
		std::clog << " ]\n" << std::endl;
	}

	void* allocate(unsigned int _size) {
		for (chunk* v = start; v; v = v->next) {
			if (v->free && v->size >= _size + sizeof(chunk)) {
				v->split(_size);
				v->free = false;
				return (void*)(v + sizeof(chunk));
			}
		}
		expand();
		return allocate(_size);
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
