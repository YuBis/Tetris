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
	void MoveToBottom();
	void MoveBlock(const eDirection& kDirection);

private:
	std::array<Vec2, eDirection_COUNT> arr_move_dir_;
	std::array<Vec2, tTETROMINO> block_shape_;
	time_t bef_time_;
	Vec2 my_pos_;
	bool isRunning_;
};

#endif // #ifndef __TETRIS_BLOCK_H__