//Eaton simple demo
//Petr Buben
//monitor smyčka, Github, makefile demo

#include <iostream>
#include "monitor.hpp"

int main() {

	int c = monitor();

	std::cout << "\nCelkove provedeno " << c-1 << " mereni \n";

	return 0;
}


