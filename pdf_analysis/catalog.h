#ifndef _CATALOG_H_
#define _CATALOG_H_
#include "name.h"
#include "OBJ.h"
#include <string>
#include "tools.h"
#include <iostream>
using namespace std;
class Name;
class Catalog
	:public OBJ
{
public:
	int parse(ifstream &readpdf, streamoff off)
	{
		string sRes;
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
				if (s == "Pages")
				{
					OBJ obj;
					if (parseObj(sRes, j, obj) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					this->Pages = obj;
				}
				else if (s == "Type")
				{
					Name n;
					if (parseName(sRes, j, n) != 0)
					{
						cout << "error" << endl;
						return -2;
					}
					this->Type = n;
				}
				else
				{
				}
				i = j;
			}
		}
		return 0;
	}
	void print()
	{
		cout << "---------------" << endl;
		cout << "Catalog :" << endl;
		cout << "Type" << " " << Type.name << endl;
		cout << "Pages" << " " << Pages.idx() << endl;
		cout << "---------------" << endl;
	}
public:
	Name Type;
	Name Version;
	OBJ Extensions;
	OBJ Pages;
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
};

#endif