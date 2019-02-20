#ifndef __TETRIS_WORLD_H__
#define __TETRIS_WORLD_H__

#include "Utility.h"
#include <unordered_map>

class Wall;
class World
{
private:
	virtual ~World();

	void CreateWall();
	void FillMap(const Vec2& start_pos, const Vec2& size, const eSpaceType& type);
	void DrawMap();

public:
	explicit World();
	void CreateMap();

private:
	eSpaceType gameboard_[MAP_SIZE_X][MAP_SIZE_Y];
	std::unordered_map<const char*, Wall*> map_wall_;
	
};

#endif