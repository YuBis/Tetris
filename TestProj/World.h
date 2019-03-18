#ifndef __TETRIS_WORLD_H__
#define __TETRIS_WORLD_H__

#include "Utility.h"

class Wall;
class Block;
class Polyomino;
class World
{
private:
	static World* instance_;
	explicit World();
	virtual ~World();

	void ParseBlockTemplate();
	void CreateWall();
	void CreateNextBlock(const int& kPolyoIdx);
	void RemoveLine(const int kLine);
	void FillMap(const Vec2& kStartPos, const Vec2& kSize, const eSpaceType& kType);

	void DrawMap(const Vec2& kStartPos, const Vec2& kSize);

	void GameLoop();
	eSpaceType GetBlockTypeByPos(const Vec2& kPos) const;
	void OrderBlockDown(const int kStartLine);

public:
	static World* getInstance();

	void CreateMap();
	void DrawMap();
	void MoveBlock(const bool kNeedRedraw = true);
	void RunningDone();
	void addPositionBuffer(const Vec2& before, const Vec2& after);

	bool IsCanCreateBlock(const Vec2& kPos, const std::vector<Vec2>* kRelativeCoordVec) const;
	bool IsBlank(const Vec2& pos) const;
	Vec2 GetDirection(const eDirection& kDir) const;
	std::pair<int, std::vector<Vec2>*>* getPolyoTemplate(const int& kPolyoIdx);

private:
	std::vector<std::pair<int, std::vector<Vec2>*>> polyo_template_list_;
	std::array<Vec2, eDirection_COUNT> arr_move_dir_;
	std::array<std::array<eSpaceType, MAP_SIZE_Y>, MAP_SIZE_X> gameboard_;
	std::unordered_map<const char*, Wall*> map_wall_;
	std::multimap<int, std::shared_ptr<Block*>> sleeping_blocks_;
	std::vector<std::pair<Vec2, Vec2>> pos_buffer_;
	std::mutex draw_mutex_;

	bool game_is_running_;

	SET_SYNTHESIZE_READONLY(Polyomino*, PlayingPolyo, playing_polyo_);
};

#endif // #ifndef __TETRIS_WORLD_H__