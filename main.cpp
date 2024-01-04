#include "heap.hpp"

int main() {
	auto num1 = heap.allocate<int>();
	int* num2 = (int*)heap.allocate(sizeof(int));
	int* num3 = (int*)heap.allocate(sizeof(int));
	*num1 = 4;
	*num2 = 6;
	*num3 = 8;

	std::clog << "Nums: " << *num1 << ", " << *num2 << ", " << *num3 << "\n" << std::endl;
	std::clog << "Output (while nums are being stored):" << std::endl;
	heap.show();

	heap.free(num1);
	heap.free(num2);
	heap.free(num3);

	std::clog << "Output (after free) (can be defragged here because they are next to each other):" << std::endl;
	heap.show();

	void* big = heap.allocate(4000);
	void* small = heap.allocate(1000);

	std::clog << "Output (before free)" << std::endl;
	heap.show();

	heap.free(big);
	heap.free(small);

	std::clog << "Output (after free) (cannot be defragged further here because memory locations are seperate):" << std::endl;
	heap.show();

	return 0;
}
