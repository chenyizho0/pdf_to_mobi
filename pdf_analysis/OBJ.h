#ifndef _OBJ_H_
#define _OBJ_H_
class OBJ
{
public:
	OBJ()
		:index(-1), generation(0)
	{}
	OBJ(int i)
		:index(i)
	{}
	void setIdx(int i)
	{
		index = i;
	}
	void setGeneration(int g)
	{
		generation = g;
	}
	int Gen()
	{
		return generation;
	}
	int idx()
	{
		return index;
	}
private:
	int index;
	int generation;
};
#endif