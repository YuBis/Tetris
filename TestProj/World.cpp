#include "World.h"
#include "Wall.h"

World::World() :
map_size_(-1, -1)
{
	std::fill(vec_wall_.begin(), vec_wall_.end(), nullptr);
}

World::~World()
{

}

World* World::create()
{
	instance = new World();
	return instance;
}

void World::RemoveInstance()
{
	delete instance;
	instance = nullptr;
}

void World::CreateMap(Vec2 mapsize)
{
	map_size_ = mapsize;

	CreateWall();
	DrawMap();
}

void World::CreateWall()
{
	
	map_wall_["LEFT"]   = Wall::create(Vec2(0, 0), Vec2(1, map_size_.y_));
	map_wall_["BOTTOM"] = Wall::create(Vec2(0, map_size_.y_ - 1), Vec2(map_size_.x_, 1));
	map_wall_["RIGHT"]  = Wall::create(Vec2(map_size_.x_ - 1, 0), Vec2(1, map_size_.y_));

	for( const auto kWall : map_wall_ )
	{
		const auto kWallData = kWall.second;

		FillMap(wall.second->wall_pos_, wall.second->wall_size_, 
	}
}

void World::FillMap(Vec2 start_pos, Vec2 size, eSpaceType type)
{

}

void World::DrawMap()
{

}