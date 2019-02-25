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
		gameboard_[10][3] = eSpaceType::tBLOCK;
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

bool World::IsCanCreateBlock(const Vec2& kPos) const
{
	return gameboard_[kPos.x_][kPos.y_] == eSpaceType::tBLANK;
}

bool World::IsCanMoveBlock(const Vec2& pos, const Vec2& dir) const
{
	const auto kCheckingPos = pos + dir;
	return GetBlockTypeByPos(kCheckingPos) == eSpaceType::tBLANK;
}

eSpaceType World::GetBlockTypeByPos(const Vec2& kPos) const
{
	return gameboard_[kPos.x_][kPos.y_];
}

void World::FillMap(const Vec2& kStartPos, const Vec2& kSize, const eSpaceType& kType)
{
	for( int y = kStartPos.y_ ; y < kStartPos.y_ + kSize.y_ ; ++y )
	{
		for( int x = kStartPos.x_ ; x < kStartPos.x_ + kSize.x_ ; ++x )
		{
			gameboard_[x][y] = kType;
		}
	}
}

void World::DrawMap(const Vec2& kStartPos, const Vec2& kSize)
{
	for( int y = kStartPos.y_ ; y < kSize.y_ ; y++ )
	{
		for( int x = kStartPos.x_ ; x < kSize.x_ ; x++ )
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

void World::MoveBlock(const Vec2& kStartPos, const Vec2& kMoveForce, const std::array<Vec2, tTETROMINO>* kBlockShape)
{
	for (const auto& kBlockRelativeCoord : *kBlockShape)
	{
		if( kBlockRelativeCoord == NULL_VEC2 ) continue;

		auto now_pos = kStartPos + kBlockRelativeCoord;
		auto new_pos = now_pos + kMoveForce;
		
		gameboard_[now_pos.x_][now_pos.y_] = eSpaceType::tBLANK;
		gameboard_[new_pos.x_][new_pos.y_] = eSpaceType::tBLOCK;
	}

	//auto startpos = Vec2(std::min<int>(kStartPos.x_, kStartPos.x_ + kMoveForce.x_), std::min<int>(kStartPos.y_, kStartPos.y_ + kMoveForce.y_));
	//auto drawsize = Vec2(abs(start_pos.x_ - end_pos.x_), abs(start_pos.y_ - end_pos.y_));
	 // todo : move block in array
	DrawMap();
}

void World::RunningDone(const int kPosY)
{
	sleeping_blocks_.insert(std::make_pair(kPosY, playing_block_));

	if( IsCanCreateBlock(Vec2(10, 3)) )
	{
		CreateNextBlock();
	}
}