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
	OutputDebugString("Block removed!");
}

Block* Block::create(Vec2 pos)
{
	auto block = new(std::nothrow) Block(pos);

	if( block != nullptr )
		return block;

	delete block;
	block = nullptr;

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
	const auto kMoveVec = World::getInstance()->GetDirection(kDirection);
	const auto kFromPos = getBlockPosition();
	const auto kToPos = getBlockPosition() + kMoveVec;

	World::getInstance()->addPositionBuffer(kFromPos, kToPos);
	setBlockPosition(kToPos);

	World::getInstance()->MoveBlock(false);
}