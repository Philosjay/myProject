#include "../Include/Game.hpp"

#include <stdexcept>
#include <iostream>

#include <math.h>

int main()
{
	

	try
	{
		Game game;
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}
