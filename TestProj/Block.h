#ifndef __TETRIS_BLOCK_H__
#define __TETRIS_BLOCK_H__

#include "Utility.h"

class Block
{

private:
	explicit Block(Vec2 pos);
	virtual ~Block();

public:
	static Block* create(Vec2 pos);
	void Run();
	void update();
	void MoveBlock(const eDirection& direction);
	void CheckKeyInput();

private:
	std::array<Vec2, 4> arr_move_dir_;
	time_t bef_time_;
	Vec2 my_pos_;
};

#endif // #ifndef __TETRIS_BLOCK_H__