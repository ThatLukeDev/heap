#include "heap.hpp"

int main() {
	int* num1 = (int*)heap.allocate(sizeof(int));
	int* num2 = (int*)heap.allocate(sizeof(int));
	int* num3 = (int*)heap.allocate(sizeof(int));
	*num1 = 4;
	*num2 = 6;
	*num3 = 8;

	heap.free(num1);
	heap.free(num2);
	heap.free(num3);

	heap.show();

	return 0;
}
