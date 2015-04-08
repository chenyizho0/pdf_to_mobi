#ifndef _OBJ_H_
#define _OBJ_H_
class OBJ
{
public:
	OBJ()
		:index(-1)
	{}
	OBJ(int i)
		:index(i)
	{}
	void setIdx(int i)
	{
		index = i;
	}
	int idx()
	{
		return index;
	}
private:
	int index;
};
#endif