#ifndef _OBJ_H_
#define _OBJ_H_
#include <string>
#include "Name.h"
#include "tools.h"
using namespace std;

extern int getContent(ifstream &readpdf, streamoff  off, string &sRes, int maxlen, string sBeg, string sEnd, streamoff &startpos);
extern int parseName(string & sSrc, size_t &startpos, Name & n);
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
	
	int  getType(ifstream &readpdf, streamoff off,string &sType)
	{
		string sRes;
		//get obj index
		if (getContent(readpdf, off, sRes, 1000, "", " ", off) != 0)
		{
			cout << "error 1" << endl;
			return -1;
		}
		off = off + sRes.size() + 1;

		//get obj generation
		if (getContent(readpdf, off, sRes, 1000, "", " ", off) != 0)
		{
			cout << "error 1" << endl;
			return -2;
		}
		off = off + sRes.size() + 1;

		//parse property
		if (getContent(readpdf, off, sRes, 1000, "\n<<", "\n>>", off) != 0)
		{
			cout << "error" << endl;
			return -3;
		}
		for (size_t i = 0; i < sRes.size(); i++)
		{
			if (sRes[i] == '/')
			{
				i++;
				string s;
				size_t j;
				for (j = i; sRes[j] != ' '; j++)
				{
					s.append(1, sRes[j]);
				}
				if (s == "Type")
				{
					Name n;
					if (parseName(sRes, j, n) != 0)
					{
						cout << "error" << endl;
						return -4;
					}
					sType = n.name;
				}
				else
				{
				}
				i = j;
			}
		}
		return 0;
	}
	
private:
	int index;
	int generation;
};
#endif