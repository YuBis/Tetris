#include "World.h"
#include "Wall.h"

World::World()
{
	for( int y = 0 ; y < MAP_SIZE_Y ; y++ )
	{
		for( int x = 0 ; x < MAP_SIZE_X ; x++ )
		{
			gameboard_[x][y] = tBLANK;
		}
	}
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
	map_wall_["LEFT"]   = new Wall(Vec2(0, 0), Vec2(1, MAP_SIZE_Y));
	map_wall_["BOTTOM"] = new Wall(Vec2(0, MAP_SIZE_Y - 1), Vec2(MAP_SIZE_X, 1));
	map_wall_["RIGHT"]  = new Wall(Vec2(MAP_SIZE_X - 1, 0), Vec2(1, MAP_SIZE_Y));

	for( const auto kWall : map_wall_ )
	{
		const auto kWallData = kWall.second;

		FillMap(kWall.second->getWallPos(), kWall.second->getWallSize(), tWALL);
	}
}

void World::FillMap(const Vec2& start_pos, const Vec2& size, const eSpaceType& type)
{
	for( int y = start_pos.y_ ; y < start_pos.y_ + size.y_ ; ++y )
	{
		for( int x = start_pos.x_ ; x < start_pos.x_ + size.x_ ; ++x )
		{
			gameboard_[x][y] = type;
		}
	}
}

void World::DrawMap()
{
	auto getBlockShape = [=](eSpaceType type) -> char {
		switch (type)
		{
			case tBLANK : return ' '; break;
			case tBLOCK : return '*'; break;
			case tWALL : return '@'; break;
			default : return '?'; break;
		}
	};

	for( int y = 0 ; y < MAP_SIZE_Y ; y++ )
	{
		for( int x = 0 ; x < MAP_SIZE_X ; x++ )
		{
			gotoxy(x, y);
			putchar(getBlockShape(gameboard_[x][y]));
		}
	}
}