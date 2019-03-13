#include "Block.h"
#include "World.h"

#include "conio.h"

Block::Block(Vec2 pos) :
bef_time_(std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count()),
my_pos_(pos)
{
	arr_move_dir_[eDirection::tUP] = Vec2(0, -1);
	arr_move_dir_[eDirection::tDOWN] = Vec2(0, 1);
	arr_move_dir_[eDirection::tLEFT] = Vec2(-1, 0);
	arr_move_dir_[eDirection::tRIGHT] = Vec2(1, 0);

	//std::fill(block_shape_.begin(), block_shape_.end(), NULL_VEC2);
	//block_shape_[0] = Vec2(1, 1);
	//block_shape_[1] = Vec2(0, 1);
	//block_shape_[2] = Vec2(1, 0);
	//block_shape_[0] = Vec2(0, 0);
}

Block::~Block()
{

}

Block* Block::create(Vec2 pos)
{
	auto block = new(std::nothrow) Block(pos);

	if( block != nullptr )
		return block;

	delete block;
	block = nullptr;

	assert(block != nullptr); // always assert when reach this line.
	return nullptr;
}

void Block::Run()
{
	//isRunning_ = true;

	//std::thread key_thread(&Block::CheckKeyInput, this);

	//while( isRunning_ )
	//{
	//	//CheckKeyInput();

	//	auto now_time = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
	//	if( now_time - bef_time_ >= BLOCK_TICK )
	//	{
	//		bef_time_ = now_time;
	//		MoveBlock(eDirection::tDOWN);
	//	}

	//}

	//key_thread.join();
}

void Block::update()
{

}

void Block::MoveToBottom()
{

}

void Block::MoveBlock(const eDirection& kDirection)
{
	/*if( !World::getInstance()->IsCanMoveBlock(my_pos_, arr_move_dir_[kDirection]) )
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

	World::getInstance()->MoveBlock(bef_pos, arr_move_dir_[kDirection], &block_shape_);*/
}