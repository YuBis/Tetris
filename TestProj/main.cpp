#include <iostream>
#include <conio.h>

#include "World.h"

using namespace std;

int main()
{
	//cout << "hello world!" << endl;

	auto world_instance = World::getInstance();
	if( world_instance != nullptr )
	{
		world_instance->CreateMap();
	}

	return 0;
}