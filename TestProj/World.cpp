#include "World.h"
#include "Wall.h"
#include "Polyomino.h"
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
playing_polyo_(nullptr)
{
	for( int y = 0 ; y < MAP_SIZE_Y ; y++ )
	{
		for( int x = 0 ; x < MAP_SIZE_X ; x++ )
		{
			gameboard_[x][y] = tBLANK;
		}
	}

	arr_move_dir_[eDirection::tUP] = Vec2(0, -1);
	arr_move_dir_[eDirection::tDOWN] = Vec2(0, 1);
	arr_move_dir_[eDirection::tLEFT] = Vec2(-1, 0);
	arr_move_dir_[eDirection::tRIGHT] = Vec2(1, 0);
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
	auto polyo = Polyomino::create(BLOCK_CREATE_POS);

	if( polyo )
	{
		for( const auto kBlock : *polyo->GetBlockList() )
		{
			gameboard_[(*kBlock)->getBlockPosition().x_][(*kBlock)->getBlockPosition().y_] = eSpaceType::tBLOCK;
		}

		playing_polyo_ = polyo;
		polyo->Run();
	}
	else
	{
		// game over
	}
}

void World::RemoveLine()
{

}

bool World::IsCanCreateBlock(const Vec2& kPos, const std::vector<Vec2>* kRelativeCoordVec) const
{
	if( kRelativeCoordVec->size() == 0 )
		return gameboard_[kPos.x_][kPos.y_] == eSpaceType::tBLANK;

	for( const auto kCoord : *kRelativeCoordVec )
	{
		if( gameboard_[kPos.x_ + kCoord.x_][kPos.y_ + kCoord.y_] != eSpaceType::tBLANK )
			return false;
	}

	return true;
}

bool World::IsBlank(const Vec2& pos) const
{
	return GetBlockTypeByPos(pos) == eSpaceType::tBLANK;
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
	draw_mutex_.lock();
	for( int y = kStartPos.y_ ; y < kSize.y_ ; y++ )
	{
		for( int x = kStartPos.x_ ; x < kSize.x_ ; x++ )
		{
			gotoxy(x, y);
			putchar(GetBlockShape(gameboard_[x][y]));
		}
	}
	draw_mutex_.unlock();
}

void World::DrawMap()
{
	DrawMap(Vec2(0, 0), Vec2(MAP_SIZE_X, MAP_SIZE_Y));
}

void World::MoveBlock(const bool kNeedRedraw)
{
	for (const auto& kBlockRelativeCoord : pos_buffer_)
	{
		if( kBlockRelativeCoord.first == NULL_VEC2 ) continue;

		const auto kNowPos = kBlockRelativeCoord.first;
		
		gameboard_[kNowPos.x_][kNowPos.y_] = eSpaceType::tBLANK;
	}

	for (const auto& kBlockRelativeCoord : pos_buffer_)
	{
		if( kBlockRelativeCoord.second == NULL_VEC2 ) continue;

		const auto kNewPos = kBlockRelativeCoord.second;
		
		gameboard_[kNewPos.x_][kNewPos.y_] = eSpaceType::tBLOCK;
	}

	pos_buffer_.clear();

	if ( kNeedRedraw )
		DrawMap();
}

void World::addPositionBuffer(const Vec2& kBeforePos, const Vec2& kAfterPos)
{
	pos_buffer_.push_back(std::make_pair(kBeforePos, kAfterPos));
}

void World::RunningDone()
{
	std::vector<int> check_lines_;
	 
	for( unsigned int i = 0 ; i < playing_polyo_->GetBlockList()->size() ; i++ )
	{
		auto block = playing_polyo_->GetBlockList()->at(i);
		check_lines_.push_back((*block)->getBlockPosition().y_);

		sleeping_blocks_.emplace(std::make_pair((*block)->getBlockPosition().y_, block));
	}

	playing_polyo_->GetBlockList()->clear();
	delete playing_polyo_;

	std::sort(check_lines_.begin(), check_lines_.end());
	check_lines_.erase( std::unique( check_lines_.begin(), check_lines_.end() ), check_lines_.end() );

	// check line is full
	for( unsigned int i = 0 ; i < check_lines_.size() ; i++ )
	{
		auto line_list = sleeping_blocks_.count(check_lines_.at(i));
		if( line_list != (MAP_SIZE_X - 2) )
			deleted_line_.push_back(check_lines_.at(i));
	}

	/** 
	TestBlock
		**
		 **
	*/
	std::vector<Vec2> tmp_block_pos;
	tmp_block_pos.push_back(Vec2(0, 0));
	tmp_block_pos.push_back(Vec2(1, 0));
	tmp_block_pos.push_back(Vec2(1, 1));
	tmp_block_pos.push_back(Vec2(2, 1));

	if( IsCanCreateBlock(BLOCK_CREATE_POS, &tmp_block_pos) )
	{
		CreateNextBlock();
	}
}

Vec2 World::GetDirection(const eDirection& kDir) const
{
	return arr_move_dir_[kDir];
}