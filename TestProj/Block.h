#ifndef __TETRIS_BLOCK_H__
#define __TETRIS_BLOCK_H__

#include "Utility.h"

class Block
{

private:
	explicit Block(Vec2 pos);

public:
	static Block* create(Vec2 pos);
	virtual ~Block();
	void Run();
	void update();
	void MoveToBottom();
	void MoveBlock(const eDirection& kDirection);

private:
	std::array<Vec2, eDirection_COUNT> arr_move_dir_;
	time_t bef_time_;
	bool isRunning_;

	SET_SYNTHESIZE(Vec2, BlockPosition, my_pos_);
};

#endif // #ifndef __TETRIS_BLOCK_H__