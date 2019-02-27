#ifndef __TETRIS_POLYOMINO_H__
#define __TETRIS_POLYOMINO_H__

#include "Utility.h"

class Polyomino
{

private:
	explicit Polyomino(Vec2 pos);
	virtual ~Polyomino();

	void CreateBlocks(const Vec2& kRelativePos);
	void MoveBlock(const eDirection& kDirection);
public:
	static Polyomino* create(Vec2 pos);
	void Run();

	void CheckKeyInput();

private:
	std::array<Vec2, eDirection_COUNT> arr_move_dir_;
	std::vector<std::pair<Vec2, Block*>> blocks_;
	time_t bef_time_;
	Vec2 my_pos_;
	bool isRunning_;
};

#endif // #ifndef __TETRIS_POLYOMINO_H__