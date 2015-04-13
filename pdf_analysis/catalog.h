#ifndef _CATALOG_H_
#define _CATALOG_H_
#include <iostream>
#include "name.h"
#include "OBJ.h"
#include <string>
#include "tools.h"

using namespace std;

class Catalog
	:public OBJ
{
public:
	Catalog()
		:Pages(NULL)
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
				if (s == "Pages")
				{
					OBJ *obj = new OBJ();
					if (parseObj(readpdf, off, obj) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					this->Pages = obj;
				}
				else if (s == "Type")
				{
					Name n;
					if (parseName(readpdf, off, n) != 0)
					{
						cout << "error" << endl;
						return -2;
					}
					this->Type = n;
				}
				else
				{
				}
				readpdf.seekg(off, ios_base::beg);
			}
		}
		return 0;
	}
	void print()
	{
		cout << "---------------" << endl;
		cout << "Catalog :" << endl;
		cout << "Type" << " " << Type.name << endl;
		cout << "Pages" << " " << Pages->idx() << endl;
		cout << "---------------" << endl;
	}
public:
	Name Type;
	
	OBJ * Pages;
	/*
	Name Version;
	OBJ * Extensions;
	//NumberTree PageLabels;
	OBJ Names;
	OBJ Dests;
	OBJ ViewerPreferences;
	Name PageLayout;
	Name PageMode;
	OBJ Outlines;
	//Array Threads;
	//OpenAction;
	OBJ AA;
	OBJ URI;
	OBJ AcroForm;
	//Stream Metadata;
	OBJ StructTreeRoot;
	OBJ MarkInfo;
	string Lang;
	OBJ SpiderInfo;
	//Array OutputIntents
	OBJ PieceInfo;
	OBJ OCProperties;
	OBJ Perms;
	OBJ Legal;
	//Array Requirements;
	OBJ Collections;
	bool NeedsRendering;
	*/
};
#endif