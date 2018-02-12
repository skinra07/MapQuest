#include <iostream>
#include <stdexcept>
#include <conio.h>


#include "POI.h"

int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		std::cout << "Error: Please pass input csv filename as argument." << "\n";

		std::cout << "Press any key to continue. . .\n";

		_getch();

		return(EXIT_FAILURE);
	}

	try
	{
		std::string input(argv[1]);

		POI poi;
		poi.load(input);
		poi.calculate();
		poi.display();

		std::cout << "\n" << "----- DONE ----" << "\n";
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";

		std::cout << "Press any key to continue. . .\n";

		_getch();

		return(EXIT_FAILURE);
	}

	std::cout << "Press any key to continue. . .\n";

	_getch();

	return EXIT_SUCCESS;
}