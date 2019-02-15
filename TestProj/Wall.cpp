#include "Wall.h"

Wall::Wall() :
	wall_size_(-1, -1),
	wall_pos_(-1, -1)
{

}

Wall::~Wall()
{

}

Wall* Wall::create(Vec2 pos, Vec2 size)
{
	if( size.x_ != 1 && size.y_ != 1) // Wall must be 1*n size
		return nullptr;

	instance = new Wall();
	instance->wall_pos_ = pos;
	instance->wall_size_ = size;
	return instance;
}

void Wall::RemoveInstance()
{
	delete instance;
	instance = nullptr;
}