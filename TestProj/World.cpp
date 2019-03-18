#include "World.h"
#include "Wall.h"
#include "Polyomino.h"
#include "Block.h"
#include "json.h"

#include <fstream>
#include <iostream>
#include <random>

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
	playing_polyo_(nullptr),
	game_is_running_(true)
{
	ParseBlockTemplate();

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
	std::random_device rd;
    std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, POLYO_KIND - 1);

	while ( game_is_running_ ) 
	{
		if( playing_polyo_ == nullptr )
		{
			const int kPolyoIdx = dis(gen);
			const auto kSelectedBlock = polyo_template_list_.at(kPolyoIdx);

			if( IsCanCreateBlock(BLOCK_CREATE_POS, kSelectedBlock.second) )
			{
				CreateNextBlock(kPolyoIdx);
			}
			else
			{
				game_is_running_ = false;
			}
		}
	}
}

void World::ParseBlockTemplate()
{
	std::ifstream json_file("blocks.json");
	Json::Value root;
	Json::Reader reader;

	if( reader.parse(json_file, root) )
	{
		const auto &kWholeData = root["blocks"];

		for( int i = 0 ; i < POLYO_KIND ; i++ )
		{
			const auto &kBlock = kWholeData[std::to_string(i)]; 
			const auto &kDirInfo = kBlock["direction"];
			const auto &kCoordInfo = kBlock["coord"];
			std::vector<Vec2>* coord_list = new std::vector<Vec2>();

			for( int j = 0 ; j < BLOCK_COUNT ; j++ )
			{
				coord_list->push_back(Vec2(kCoordInfo[j][0].asInt(), kCoordInfo[j][1].asInt()));
			}

			polyo_template_list_.push_back(std::make_pair(kDirInfo.asInt(), coord_list));
		}
	}
	else
	{
		// block shape parse failed!
		std::cout  << reader.getFormattedErrorMessages() << "\n";
		assert(false);
	}
}


void World::CreateMap()
{
	CreateWall();
	DrawMap();
	GameLoop();
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

void World::CreateNextBlock(const int& kPolyoIdx)
{
	auto polyo = Polyomino::create(BLOCK_CREATE_POS, kPolyoIdx);

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

void World::RemoveLine(const int kLine)
{
	for( unsigned int i = 1 ; i < MAP_SIZE_X - 1 ; i++ )
	{
		gameboard_[i][kLine] = tBLANK;
	}

	sleeping_blocks_.erase(kLine);
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

std::pair<int, std::vector<Vec2>*>* World::getPolyoTemplate(const int& kPolyoIdx)
{
	return &polyo_template_list_.at(kPolyoIdx);
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
			std::cout << GetBlockShape(gameboard_[x][y]);
		}
	}
	draw_mutex_.unlock();
}

void World::DrawMap()
{
	DrawMap(Vec2(0, 0), Vec2(MAP_SIZE_X, MAP_SIZE_Y));
}

void World::MoveBlock(const bool kNeedRedraw /* = true */)
{
	for (const auto& kBlockRelativeCoord : pos_buffer_)
	{
		const auto kNowPos = kBlockRelativeCoord.first;

		gameboard_[kNowPos.x_][kNowPos.y_] = eSpaceType::tBLANK;
	}

	for (const auto& kBlockRelativeCoord : pos_buffer_)
	{
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

void World::OrderBlockDown(const int kStartLine)
{
	pos_buffer_.clear();

	auto block_list_ = sleeping_blocks_.equal_range(kStartLine);

	for( unsigned int y = kStartLine ; y > 1 ; y-- )
	{
		auto it = sleeping_blocks_.begin();

		while ( it != sleeping_blocks_.end() )
		{
			if( it->first != y )
			{
				++it;
				continue;
			}

			auto key = it->first + 1;
			auto val = it->second;
			(*it->second)->MoveBlock(eDirection::tDOWN);

			it = sleeping_blocks_.erase(it);
			sleeping_blocks_.insert(std::make_pair(key, val));
		}

		MoveBlock(false);
	}
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
		auto line_block_count = sleeping_blocks_.count(check_lines_.at(i));
		if( line_block_count == (MAP_SIZE_X
			- map_wall_["LEFT"]->getWallSize().x_
			- map_wall_["RIGHT"]->getWallSize().x_ ) )
		{
			RemoveLine(check_lines_.at(i));
			OrderBlockDown(check_lines_.at(i) - 1);
		}
	}

	DrawMap();

	playing_polyo_ = nullptr;
}

Vec2 World::GetDirection(const eDirection& kDir) const
{
	return arr_move_dir_[kDir];
}