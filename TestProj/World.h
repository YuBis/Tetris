#ifndef __TETRIS_WORLD_H__
#define __TETRIS_WORLD_H__

#include "Utility.h"

class Wall;
class Block;
class World
{
private:
	static World* instance_;
	explicit World();
	virtual ~World();

	void CreateWall();
	void CreateNextBlock();
	void RemoveLine();
	void FillMap(const Vec2& start_pos, const Vec2& size, const eSpaceType& type);

	void DrawMap(); // default function only can be called by world.
	void DrawMap(const Vec2& start_pos, const Vec2& size);

	void GameLoop();
	void update(time_t dt);
	eSpaceType GetBlockTypeByPos(const Vec2& pos);
public:
	static World* getInstance();

	void CreateMap();
	bool IsCanCreateBlock(const Vec2& pos);
	bool IsCanMoveBlock(Vec2 pos, Vec2 dir);
	void MoveBlock(const Vec2& start_pos, const Vec2& end_pos);
private:
	std::array<std::array<eSpaceType, MAP_SIZE_Y>, MAP_SIZE_X> gameboard_;
	std::unordered_map<const char*, Wall*> map_wall_;
	
	SET_SYNTHESIZE_READONLY(Block*, PlayingBlock, playing_block_);
};

#endif // #ifndef __TETRIS_WORLD_H__