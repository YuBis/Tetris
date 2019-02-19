#ifndef __TETRIS_UTILITY_H__
#define __TETRIS_UTILITY_H__

#define MAP_SIZE_X 10
#define MAP_SIZE_Y 20

#define SET_SYNTHESIZE_READONLY(vartype, funcname, varname) \
	private: vartype varname;\
	public: vartype get##funcname(){return varname;};\
			void set##funcname(vartype var){varname = var;};

enum eSpaceType
{
	tBLANK = 0,
	tWALL = 1,
	tBLOCK = 2
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
};

#endif