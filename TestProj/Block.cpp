#include "Block.h"
#include "World.h"

Block::Block(Vec2 pos) :
bef_time_(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())),
my_pos_(pos)
{
	arr_move_dir_[0] = Vec2(0, 1);
	arr_move_dir_[1] = Vec2(0, -1);
	arr_move_dir_[2] = Vec2(-1, 0);
	arr_move_dir_[3] = Vec2(1, 0);

	Run();
}

Block::~Block()
{

}

Block* Block::create(Vec2 pos)
{
	if( !World::getInstance()->IsCanCreateBlock(pos) )
		return nullptr; // cannot create -> game over.

	auto block = new(std::nothrow) Block(pos);

	if( block != nullptr )
		return block;

	delete block;
	block = nullptr;
	return nullptr;
}

void Block::Run()
{
	while( true )
	{
		CheckKeyInput();

		auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		if( now_time - bef_time_ > BLOCK_TICK )
		{

			bef_time_ = now_time;
			MoveBlock(eDirection::tDOWN);
		}

	}
}

void Block::update()
{

}

void Block::MoveBlock(const eDirection& direction)
{
	if( !World::getInstance()->IsCanMoveBlock(my_pos_, arr_move_dir_[direction]) )
		return;

	auto bef_pos = my_pos_;
	my_pos_ = my_pos_ + arr_move_dir_[direction];

	World::getInstance()->MoveBlock(bef_pos, my_pos_);
}

/**
this function only execute array key and spacebar.
except these case, send input to World.
*/
void Block::CheckKeyInput()
{
	//if( GetKeyState(VK_LEFT 
}