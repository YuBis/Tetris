#include <iostream>
#include <conio.h>

#include "World.h"

using namespace std;

int main()
{
	cout << "hello world!" << endl;

	auto _instance = World::create();
	_instance->CreateMap();

	return 0;
}