#include "World.h"
#include "Wall.h"

World::World() :
map_size_(MAP_SIZE_X, MAP_SIZE_Y)
{
	std::fill(map_wall_.begin(), map_wall_.end(), nullptr);
}

World::~World()
{

}


void World::CreateMap()
{
	CreateWall();
	DrawMap();
}

void World::CreateWall()
{
	
	map_wall_["LEFT"]   = new Wall(Vec2(0, 0), Vec2(1, map_size_.y_));
	map_wall_["BOTTOM"] = new Wall(Vec2(0, map_size_.y_ - 1), Vec2(map_size_.x_, 1));
	map_wall_["RIGHT"]  = new Wall(Vec2(map_size_.x_ - 1, 0), Vec2(1, map_size_.y_));

	for( const auto kWall : map_wall_ )
	{
		const auto kWallData = kWall.second;

		FillMap(kWall.second->getWallPos(), kWall.second->getWallSize(), tWALL);
	}
}

void World::FillMap(Vec2 start_pos, Vec2 size, eSpaceType type)
{
	auto getBlockShape = [=](eSpaceType type) -> std::string {
		switch (type)
		{
			case tBLANK : return "в╞"; break;
			case tBLOCK : return "бс"; break;
			case tWALL : return "бр"; break;
			default : return "?"; break;
		}
	};

	const std::string kBlock = getBlockShape(type);

	
}

void World::DrawMap()
{

}