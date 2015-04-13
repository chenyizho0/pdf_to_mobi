#ifndef STREAM_H_
#define STREAM_H_

#include "Name.h"
#include "OBJ.h"
#include <string>
#include "tools.h"
using namespace std;

class Stream
	:public OBJ
{
public:
	Stream()
	{}
	int parse(ifstream &readpdf, streamoff & off)
	{
		string sRes;
		char c;
		//get obj index
		if (getContent(readpdf, off, sRes, 1000, "", " ", off) != 0)
		{
			cout << "error 1" << endl;
			return -1;
		}
		this->setIdx(stringToInt(sRes));
		off = off + sRes.size() + 1;
		//get obj generation
		if (getContent(readpdf, off, sRes, 1000, "", " ", off) != 0)
		{
			cout << "error 1" << endl;
			return -1;
		}
		this->setGeneration(stringToInt(sRes));
		off = off + sRes.size() + 1;
		//parse property
		//可能有bug 未判断文件访问是否越界
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
					break;
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

				if (s == "Filter")
				{
					Name n;
					if (parseName(readpdf, off, n) != 0)
					{
						cout << "error" << endl;
						return -2;
					}
					this->Filter = n;
				}
				if (s == "Length")
				{
					int i;
					if (parseInt(readpdf, off, i) != 0)
					{
						cout << "error" << endl;
						return -3;
					}
					this->Length = i;
				}
				else
				{
				}
				readpdf.seekg(off, ios_base::beg);
			}
		}
		sRes = "";
		streamoff startpos;
		int iRet = getContent(readpdf, off, sRes, 100000, "\nstream", "\nendstream", startpos);
		this->decodeData = sRes;
		return iRet;
	}



public:
	string decodeData;
	int Length;
	Name Filter;
	//OBJ * DecodeParms;
	//file specification F;
	//Name FFilter;
	//OBJ * FDecodeParms;
	//int DL;
};

#endif