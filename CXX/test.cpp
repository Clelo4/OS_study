/**
 * @file test.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-25
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>

namespace X {
	int x;
	int n = 1;
}
int n = 2;

class A {
	public:
		A() {
			std::cout << "A()" << std::endl;
		}
	~A() {
		std::cout << "~A()" << std::endl;
	}
};

void fn() {
	A a;
	// throw 1; // logic_error("a");
}

int main() {
	fn();
	return  0;
}

