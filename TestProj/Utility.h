#ifndef __TETRIS_UTILITY_H__
#define __TETRIS_UTILITY_H__

#include <Windows.h>
#include <chrono>
#include <ctime>
#include <cmath>
#include <array>
#include <unordered_map>

#define MAP_SIZE_X 20
#define MAP_SIZE_Y 20
#define BLOCK_TICK 1.f

// this macro must use on bottom of class
// or, must re-declare access specifier after use it.
#define SET_SYNTHESIZE_READONLY(vartype, funcname, varname) \
	private: vartype varname;\
	public: vartype get##funcname(){return varname;};\
			void set##funcname(vartype var){varname = var;};

#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet != nullptr) \
    { \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

enum eSpaceType
{
	tBLANK = 0,
	tWALL = 1,
	tBLOCK = 2
};

enum eDirection
{
	tUP = 0,
	tDOWN = 1,
	tLEFT = 2,
	tRIGHT = 3
};


struct Vec2
{
	int x_;
	int y_;

	Vec2()
	{
		x_ = 0;
		y_ = 0;
	}

	Vec2(int x, int y)
	{
		x_ = x;
		y_ = y;
	}

	Vec2& operator+(const Vec2& rhs)
	{
		this->x_ += rhs.x_;
		this->y_ += rhs.y_;

		return *this;
	}
};

static void gotoxy(int x,int y)
{
	COORD pos={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

static char GetBlockShape(eSpaceType type)
{
	switch (type)
	{
		case tBLANK : return ' '; break;
		case tBLOCK : return '*'; break;
		case tWALL : return '@'; break;
		default : return '?'; break;
	}
};

#endif