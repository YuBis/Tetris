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
	void FillMap(const Vec2& kStartPos, const Vec2& kSize, const eSpaceType& kType);

	void DrawMap(); // default function only can be called by world.
	void DrawMap(const Vec2& kStartPos, const Vec2& kSize);

	void GameLoop();
	void update(time_t dt);
	eSpaceType GetBlockTypeByPos(const Vec2& kPos) const;
public:
	static World* getInstance();

	void CreateMap();
	bool IsCanCreateBlock(const Vec2& kPos) const;
	bool IsCanMoveBlock(const Vec2& pos, const Vec2& dir) const;
	void MoveBlock(const Vec2& kStartPos, const Vec2& kMoveForce, const std::array<Vec2, tTETROMINO>* kBlockShape);
	void RunningDone(const int kPosY);

private:
	std::array<std::array<eSpaceType, MAP_SIZE_Y>, MAP_SIZE_X> gameboard_;
	std::unordered_map<const char*, Wall*> map_wall_;
	std::multimap<int, Block*> sleeping_blocks_;
	
	SET_SYNTHESIZE_READONLY(Block*, PlayingBlock, playing_block_);
};

#endif // #ifndef __TETRIS_WORLD_H__