#include "Polyomino.h"
#include "World.h"
#include "Block.h"

#include "conio.h"

Polyomino::Polyomino(const Vec2& pos, const int& kTurnLimit, const std::vector<Vec2>* kBlockPos) :
	bef_time_(0),
	blocks_(nullptr),
	is_next_turn_to_right_(true),
	turn_limit_(kTurnLimit)
{
	blocks_ = new std::vector<std::shared_ptr<Block*>>;
	my_origin_ = pos;

	std::for_each(kBlockPos->cbegin(), kBlockPos->cend(), [&](Vec2 relative_pos){
		AddBlock(my_origin_, relative_pos);
	});
}

Polyomino::~Polyomino()
{
	blocks_->clear();
	delete blocks_;
}

void Polyomino::AddBlock(const Vec2& kBasePos, const Vec2& kRelativePos)
{
	shape_.push_back(kRelativePos);
	blocks_->push_back(std::make_shared<Block*>(Block::create(kBasePos + kRelativePos)));
}

Polyomino* Polyomino::create(const Vec2& pos, const int& kPolyoIdx)
{
	const auto kThisPolyo = World::getInstance()->getPolyoTemplate(kPolyoIdx);

	if( !World::getInstance()->IsCanCreateBlock(pos, kThisPolyo->second) )
		return nullptr; // cannot create -> game over.

	auto polyo = new(std::nothrow) Polyomino(pos, kThisPolyo->first, kThisPolyo->second);

	if( polyo != nullptr )
		return polyo;

	delete polyo;
	polyo = nullptr;
	return nullptr;
}

void Polyomino::Run()
{
	isRunning_ = true;

	std::thread key_thread(&Polyomino::CheckKeyInput, this);

	while( isRunning_ )
	{
		auto now_time = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
		if( now_time - bef_time_ >= BLOCK_TICK )
		{
			bef_time_ = now_time;
			move_mutex_.lock();
			if( !CheckReachBottom() )
			{
				MoveBlock(eDirection::tDOWN);
			}
			move_mutex_.unlock();
		}

	}

	key_thread.join();
	World::getInstance()->RunningDone();
}

void Polyomino::MoveBlock(const eDirection& kDirection, const bool kNeedRedraw)
{
	const auto kMoveVec = World::getInstance()->GetDirection(kDirection);

	std::transform(blocks_->begin(), blocks_->end(), blocks_->begin(), [kMoveVec](std::shared_ptr<Block*> block) -> std::shared_ptr<Block*>
	{
		const auto kFromPos = (*block)->getBlockPosition();
		const auto kToPos = kFromPos + kMoveVec;

		World::getInstance()->addPositionBuffer(kFromPos, kToPos);
		(*block)->setBlockPosition(kToPos);
		return block;
	});

	my_origin_ += kMoveVec;

	World::getInstance()->MoveBlock(kNeedRedraw);
}

void Polyomino::RotateBlock()
{
	std::transform(shape_.begin(), shape_.end(), shape_.begin(), [&](Vec2 relative_pos)
	{
		auto x_new = relative_pos.y_;
		auto y_new = relative_pos.x_;

		if( is_next_turn_to_right_ )
			x_new *= -1;
		else
			y_new *= -1;

		return Vec2(x_new, y_new);
	});

 	for( unsigned int i = 0 ; i < blocks_->size() ; i++ )
	{
		auto new_pos = (my_origin_ + shape_.at(i));
		World::getInstance()->addPositionBuffer((*blocks_->at(i))->getBlockPosition(), new_pos);
		(*blocks_->at(i))->setBlockPosition(new_pos);
	}

	World::getInstance()->MoveBlock();
}

void Polyomino::ChangeRotateState()
{
	// base case : point symmetry block has only 2 direction
	if( turn_limit_ == 2 )
	{
		is_next_turn_to_right_ = !is_next_turn_to_right_;
		return;
	}	
}

/**
this function only execute array key and spacebar.
except these case, send input to World.
*/
void Polyomino::CheckKeyInput()
{
	std::map<char, eDirection> kDirectionMap;

	kDirectionMap.insert(std::make_pair('a', eDirection::tLEFT));
	kDirectionMap.insert(std::make_pair('d', eDirection::tRIGHT));
	kDirectionMap.insert(std::make_pair('s', eDirection::tDOWN));

	while( getIsRunning() )
	{
		if( !_kbhit() )
			continue;

		char ch = _getch();
		switch ( ch )
		{
		case 's' :
			if ( CheckReachBottom() )
				break;

		case 'a' : 
		case 'd' : 
			{
				move_mutex_.lock();
				const auto kMoveVec = World::getInstance()->GetDirection(kDirectionMap.at(ch));
				if( IsCanMoveBlock(kMoveVec) )
					MoveBlock(kDirectionMap.at(ch)); 
				move_mutex_.unlock();
			} break;
		case 'w' :
			{
				move_mutex_.lock();
				if( IsCanRotateBlock() )
				{
					RotateBlock();
					ChangeRotateState();
				}
				move_mutex_.unlock();
			} break;

		case VK_SPACE :
			{
				move_mutex_.lock();
				while( !CheckReachBottom() )
				{
					MoveBlock(eDirection::tDOWN, false);
				}
				World::getInstance()->DrawMap();
				move_mutex_.unlock();
			} break;
		}
	}
}

bool Polyomino::IsCanMoveBlock(const Vec2& kDir) const
{	
	for( const auto kBlock : *blocks_ )
	{
		// check each block is last pos for dir
		if( HasFriendBlock((*kBlock)->getBlockPosition(), kDir) )
		{
			continue;
		}

		if( !World::getInstance()->IsBlank((*kBlock)->getBlockPosition() + kDir) )
		{
			return false;
		}
	}

	return true;
}

bool Polyomino::IsCanRotateBlock() const
{
	// base case : square block cannot turn
	if( turn_limit_ == 1 )
		return false;

	// anchor point is (0,0)

	for( const auto kBlockShape : shape_ )
	{
		auto x_new = kBlockShape.y_;
		auto y_new = kBlockShape.x_;

		if( is_next_turn_to_right_ )
			x_new *= -1;
		else
			y_new *= -1;

		auto check_pos = my_origin_ + Vec2(x_new, y_new);

		if( !HasFriendBlock(check_pos, Vec2(0, 0)) && 
			!World::getInstance()->IsBlank(check_pos) )
		{
			return false;
		}
	}

	return true;
}

bool Polyomino::HasFriendBlock(const Vec2& kBlockPos, const Vec2& kDir) const
{
	for( const auto kBlock : *blocks_ )
	{
		if( (*kBlock)->getBlockPosition() == kBlockPos + kDir )
		{
			return true;
		}
	}

	return false;
}

bool Polyomino::CheckReachBottom()
{
	const auto kMoveVec = World::getInstance()->GetDirection(eDirection::tDOWN);

	if( !IsCanMoveBlock(kMoveVec) )
	{
		isRunning_ = false;
		return true;
	}

	return false;
}

std::vector<std::shared_ptr<Block*>>* Polyomino::GetBlockList() const
{
	return blocks_;
}