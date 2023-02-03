#include <iostream>
#include <string>

unsigned int fact(unsigned int n) {
	unsigned int result = 1;
	if (n == 0) {
		return result;
	}
	else {
		for (unsigned i = 1; i <= n; i++) {
			result *= i;
		}
	}
	return result;
}

int main9() {
	unsigned int shto;
	std::cin >> shto;
	std::cout << fact(shto);
	return 0;
}