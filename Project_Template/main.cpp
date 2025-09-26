#include "Game.h"


int main(int argc,char* args[])
{
	
	auto pGame = std::make_unique<Game>(Window{"ProjectName - Marek Lens", 1400.f , 900.f});
	pGame->Run();

	return 0;

}

