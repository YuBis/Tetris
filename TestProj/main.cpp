#include <iostream>
#include <conio.h>

#include "World.h"

using namespace std;

int main()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);

	auto world_instance = World::getInstance();
	if( world_instance != nullptr )
	{
		world_instance->CreateMap();
	}

	return 0;
}