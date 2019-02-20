#include <iostream>
#include <conio.h>

#include "World.h"

using namespace std;

int main()
{
	//cout << "hello world!" << endl;

	auto _instance = new World();
	_instance->CreateMap();

	return 0;
}