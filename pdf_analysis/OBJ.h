#ifndef _OBJ_H_
#define _OBJ_H_
#include <string>
#include "Name.h"
#include "tools.h"
#include <stack>
using namespace std;

extern int getContent(ifstream &readpdf, streamoff  off, string &sRes, int maxlen, string sBeg, string sEnd, streamoff &startpos);
extern int parseName(ifstream &readpdf, streamoff &off, Name & n);
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
		char c;
		//parse property
		//可能有bug 未判断文件访问是否越界

		//stack 判断<< 和 >> 的匹配情况
		stack<int> stk;

		while (true)
		{
			readpdf.read((char *)&c, sizeof(c));
			off++;
			if (c == '<')
			{
				readpdf.read((char *)&c, sizeof(c));
				off++;
				if (c == '<')
				{
					stk.push(1);
					break;
				}
				else
				{
					cout << "error obj 1" << endl;
					return -1;
				}
			}

		}
		string s;
		readpdf.seekg(off, ios_base::beg);
		while (true)
		{
			readpdf.read((char *)&c, sizeof(c));
			off++;
			if (c == '>')
			{
				readpdf.read((char *)&c, sizeof(c));
				off++;
				if (c == '>')
				{
					if (stk.empty())break;
					else
					{
						stk.pop();
						if (stk.empty())break;
					}
				}
			}

			if (c == '<')
			{
				readpdf.read((char *)&c, sizeof(c));
				off++;
				if (c == '<')
				{
					stk.push(1);
				}
			}

			if (c == '/')
			{
				s = "";
				readpdf.read((char *)&c, sizeof(c));
				off++;
				while (c != ' ' && c != '\n' && c != '\t')
				{
					s.append(1, c);
					readpdf.read((char *)&c, sizeof(c));
					off++;
				}
				if (s == "Type")
				{
					Name n;
					if (parseName(readpdf, off, n) != 0)
					{
						cout << "error" << endl;
						return -2;
					}
					sType = n.name;
					return 0;
				}
				else
				{
				}
				readpdf.seekg(off, ios_base::beg);
			}
		}
		return 0;
	}
	
private:
	int index;
	int generation;
};
#endif