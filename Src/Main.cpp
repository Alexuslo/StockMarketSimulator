#include "Application.h"
#include <iostream>

int main() 
{
	try
	{
		auto &Application = CApplication::GetInstance();

		Application.Init();
		Application.Run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}