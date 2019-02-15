#ifndef __TETRIS_UTILITY_H__
#define __TETRIS_UTILITY_H__

#define SET_SYNTHESIZE_READONLY(vartype, varname) \
	private: vartype varname;\
	public: vartype get##varname(){return varname;};\
			void set##varname(vartype var){varname = var;};

enum eSpaceType
{
	tBLANK,
	tWALL,
	tBLOCK
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