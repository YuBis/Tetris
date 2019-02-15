#ifndef __TETRIS_WORLD_H__
#define __TETRIS_WORLD_H__

#include "Utility.h"
#include <unordered_map>

class Wall;
class World
{
private:
	static World* instance;

	explicit World();
	virtual ~World();
	void RemoveInstance();

	void CreateMap(Vec2 mapsize);
	void CreateWall();
	void FillMap(Vec2 start_pos, Vec2 size, eSpaceType type);
	void DrawMap();

public:
	static World* create();

private:
	Vec2 map_size_;
	std::unordered_map<std::string, Wall*> map_wall_;

};

#endif