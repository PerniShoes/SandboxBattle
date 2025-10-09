#include "Game.h"


int main(int argc,char* args[])
{
	
	auto pGame = std::make_unique<Game>(Window{"Duelbox Demo - Marek Lens", 1500.f , 900.f});
	pGame->Run();

	return 0;

}

