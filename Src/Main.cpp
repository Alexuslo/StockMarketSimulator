#include "Application.h"

int main() 
{
	auto &Application = CApplication::GetInstance();

	Application.Init();
	Application.Run();

	return 0;
}