#include "World.h"
#include "Wall.h"
#include "Block.h"

World* World::instance_ = nullptr;

World* World::getInstance()
{
	if( !instance_ )
	{
		instance_ = new(std::nothrow) World();
	}

	return instance_;
}

World::World() :
playing_block_(nullptr)
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

void World::GameLoop()
{
	//while (true) 
	{
		auto thistime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		//update(thistime - bef_time_);

	}
}

void World::update(time_t dt)
{
}


void World::CreateMap()
{
	CreateWall();
	DrawMap();
	CreateNextBlock();
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

void World::CreateNextBlock()
{
	auto block = Block::create(Vec2(10, 3));

	if( block )
	{
		playing_block_ = block;
		block->Run();
	}
	else
	{
		// game over
	}
}

void World::RemoveLine()
{

}

bool World::IsCanCreateBlock(const Vec2& pos)
{
	return gameboard_[pos.x_][pos.y_] == eSpaceType::tBLANK;
}

bool World::IsCanMoveBlock(Vec2 pos, Vec2 dir)
{
	return GetBlockTypeByPos(pos + dir) == eSpaceType::tBLANK;
}

eSpaceType World::GetBlockTypeByPos(const Vec2& pos)
{
	return gameboard_[pos.x_][pos.y_];
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

void World::DrawMap(const Vec2& start_pos, const Vec2& size)
{
	for( int y = start_pos.y_ ; y < size.y_ ; y++ )
	{
		for( int x = start_pos.x_ ; x < size.x_ ; x++ )
		{
			gotoxy(x, y);
			putchar(GetBlockShape(gameboard_[x][y]));
		}
	}
}

void World::DrawMap()
{
	DrawMap(Vec2(0, 0), Vec2(MAP_SIZE_X, MAP_SIZE_Y));
}

void World::MoveBlock(const Vec2& start_pos, const Vec2& end_pos)
{
	auto start_pos = Vec2(std::min<int>(start_pos.x_, end_pos.x_), start_pos.y_);
	auto draw_size = Vec2(abs(start_pos.x_ - end_pos.x_), abs(start_pos.y_ - end_pos.y_));
	 // todo : move block in array
	DrawMap(start_pos, draw_size);
}