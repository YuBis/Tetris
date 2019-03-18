#ifndef __TETRIS_POLYOMINO_H__
#define __TETRIS_POLYOMINO_H__

#include "Utility.h"

class Block;
class Polyomino
{

private:
	explicit Polyomino(const Vec2& pos, const int& kTurnLimit, const std::vector<Vec2>* kBlockPos);

	void AddBlock(const Vec2& kBasePos, const Vec2& kRelativePos);
	void MoveBlock(const eDirection& kDirection, const bool kNeedRedraw = true);
	void RotateBlock();

	bool IsCanMoveBlock(const Vec2& kDir) const;
	bool IsCanRotateBlock() const;
	bool HasFriendBlock(const Vec2& kBlockPos, const Vec2& kDir) const;
	bool CheckReachBottom();
	void CheckKeyInput();
	void ChangeRotateState();

public:
	static Polyomino* create(const Vec2& pos, const int& kPolyoIdx);
	virtual ~Polyomino();
	void Run();

	std::vector<std::shared_ptr<Block*>>* GetBlockList() const;

private:
	Vec2 my_origin_;
	std::vector<std::shared_ptr<Block*>>* blocks_;
	std::vector<Vec2> shape_;
	int turn_limit_;
	bool is_next_turn_to_right_;
	time_t bef_time_;
	std::mutex move_mutex_;

	SET_SYNTHESIZE_READONLY(bool, IsRunning, isRunning_);
};

#endif // #ifndef __TETRIS_POLYOMINO_H__