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
	void FillMap(Vec2 start_pos, Vec2 size, eSpaceType type);
	void DrawMap();

public:
	explicit World();
	void CreateMap();

private:
	const Vec2 map_size_;
	std::unordered_map<const char*, Wall*> map_wall_;
	
};

#endif