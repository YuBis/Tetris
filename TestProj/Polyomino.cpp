#include "Polyomino.h"
#include "World.h"
#include "Block.h"

#include "conio.h"

Polyomino::Polyomino(Vec2 pos) :
bef_time_(std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count()),
my_pos_(pos)
{
	arr_move_dir_[eDirection::tUP] = Vec2(0, -1);
	arr_move_dir_[eDirection::tDOWN] = Vec2(0, 1);
	arr_move_dir_[eDirection::tLEFT] = Vec2(-1, 0);
	arr_move_dir_[eDirection::tRIGHT] = Vec2(1, 0);

	/** 
	TestBlock
		**
		 **
	*/
	
	blocks_position_.push_back(Vec2(1, 0));
	blocks_position_.push_back(Vec2(1, 1));
	blocks_position_.push_back(Vec2(2, 1));
}

Polyomino::~Polyomino()
{

}

void Polyomino::CreateBlocks(const Vec2& kRelativePos)
{
	
	blocks_.push_back(std::make_pair(kRelativePos, Block::create(kRelativePos)));
}

Polyomino* Polyomino::create(Vec2 pos)
{
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

	if( !World::getInstance()->IsCanCreateBlock(pos, tmp_block_pos) )
		return nullptr; // cannot create -> game over.

	auto block = new(std::nothrow) Polyomino(pos);

	if( block != nullptr )
		return block;

	delete block;
	block = nullptr;
	return nullptr;
}

void Polyomino::Run()
{
	isRunning_ = true;

	std::thread key_thread(&Polyomino::CheckKeyInput, this);

	while( isRunning_ )
	{
		//CheckKeyInput();

		auto now_time = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
		if( now_time - bef_time_ >= BLOCK_TICK )
		{
			bef_time_ = now_time;
			MoveBlock(eDirection::tDOWN);
		}

	}

	key_thread.join();
}

void Polyomino::MoveBlock(const eDirection& kDirection)
{
	if( !World::getInstance()->IsCanMoveBlock(my_pos_, arr_move_dir_[kDirection]) )
	{
		if( kDirection == eDirection::tDOWN )
		{
			isRunning_ = false;
			World::getInstance()->RunningDone();
		}
		return;
	}

	auto bef_pos = my_pos_;
	my_pos_ = my_pos_ + arr_move_dir_[kDirection];

	World::getInstance()->MoveBlock(bef_pos, arr_move_dir_[kDirection], &blocks_position_);
}

/**
this function only execute array key and spacebar.
except these case, send input to World.
*/
void Polyomino::CheckKeyInput()
{
	while( isRunning_ )
	{
		switch ( getch() )
		{
			case 'a' : MoveBlock(eDirection::tLEFT); break;
			case 'd' : MoveBlock(eDirection::tRIGHT); break;
			case 's' : MoveBlock(eDirection::tDOWN); break;
			case 'w' :
				{
				// turn
				} break;

			case VK_SPACE :
				{

				} break;
		}
	}
}