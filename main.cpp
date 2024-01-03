#include "heap.hpp"

int main() {
	int* num1 = (int*)heap.allocate(sizeof(int));
	int* num2 = (int*)heap.allocate(sizeof(int));
	int* num3 = (int*)heap.allocate(sizeof(int));
	*num1 = 4;
	*num2 = 6;
	*num3 = 8;
	std::clog << *num1 << std::endl;
	std::clog << *num2 << std::endl;
	std::clog << *num3 << std::endl;

	std::clog << std::endl;

	return 0;
}
