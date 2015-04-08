#ifndef _TRAILER_H_
#define _TRAILER_H_
#include <fstream>
#include "tools.h"
#include "OBJ.h"
class OBJ;
class Trailer
{
public:
	Trailer()
	{}
	int parse(ifstream &readpdf,streamoff off)
	{
		string sRes;
		if (getContent(readpdf, off, sRes, 1000, "<<", ">>", off) != 0)
		{
			cout << "error" << endl;
			return -1;
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
				if (s == "Size")
				{
					int iSize = -1;
					if (parseInt(sRes, j, iSize) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					Size = iSize;
				}
				else if (s == "Prev")
				{
					int iPrev = -1;
					if (parseInt(sRes, j, iPrev) != 0)
					{
						cout << "error" << endl;
						return -2;
					}
					Prev = iPrev;
				}
				else if (s == "Root")
				{
					OBJ obj;
					if (parseObj(sRes, j, obj) != 0)
					{
						cout << "error" << endl;
						return -3;
					}
					Root = obj;
				}
				else if (s == "Info")
				{
					OBJ obj;
					if (parseObj(sRes, j, obj) != 0)
					{
						cout << "error" << endl;
						return -3;
					}
					Info = obj;
				}
				else
				{
				}
				i = j;
			}
		}
		return 0;
	}
	streamoff  crossRefTable_offset;
	int Size;
	int Prev;
	OBJ Root;
	//OBJ Encrypt;
	OBJ Info;
	//Array ID;
};


#endif