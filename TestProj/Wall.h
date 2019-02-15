#ifndef __TETRIS_WALL_H__
#define __TETRIS_WALL_H__

#include "Utility.h"

class Wall
{

private:
	static Wall* instance;

	explicit Wall();
	virtual ~Wall();
	void RemoveInstance();

	Vec2 wall_size_;
	Vec2 wall_pos_;
public:
	static Wall* create(Vec2 pos, Vec2 size);
}

#endif