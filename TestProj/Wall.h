#ifndef __TETRIS_WALL_H__
#define __TETRIS_WALL_H__

#include "Utility.h"

class Wall
{

private:
	virtual ~Wall();

public:
	explicit Wall(Vec2 pos, Vec2 size);

	SET_SYNTHESIZE_READONLY(Vec2, WallSize, wall_size_);
	SET_SYNTHESIZE_READONLY(Vec2, WallPos, wall_pos_);
};

#endif