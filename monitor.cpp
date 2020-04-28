#include <iostream>
//using namespace std;

int monitor(){

	static char in = '1';
	int c = 1;

	std::cout << "K zastaveni, zmacknete 0\n";
	//cin;

	while (1){

		std::cin >> in;

		if (in == '0')
		break;

		std::cout<< "Zaznam c. " << c-1 << "  hodnota " <<in << std::endl;
		c++;
	}

	return c;
}

