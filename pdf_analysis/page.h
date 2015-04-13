#ifndef PAGE_H_
#define PAGE_H_

#include "rectangle.h"
#include "Name.h"
#include "OBJ.h"
#include "tools.h"
#include <iostream>
using namespace std;

class Page
	:public OBJ
{
public:
	Page()
		:Contents(NULL), Parent(NULL)
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
				if (s == "Contents")
				{
					OBJ *obj = new OBJ();
					if (parseObj(readpdf, off, obj) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					this->Contents = obj;
				}
				else if (s == "Parent")
				{
					OBJ *obj = new OBJ();
					if (parseObj(readpdf, off, obj) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					this->Parent = obj;
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
				else if (s == "Resources")
				{
					/*
					OBJ *obj = new OBJ();
					if (parseObj(readpdf, off, obj) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					this->Parent = obj;
					*/
				}
				else if (s == "MediaBox")
				{
					Rectangle r;
					if (parseRect(readpdf, off, r) != 0)
					{
						cout << "error" << endl;
						return -3;
					}
					this->MediaBox = r;
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
		cout << "Page :" << endl;
		cout << "Type" << " " << Type.name << endl;
		cout << "Contents" << " ";
		if (Contents != NULL) cout << Contents->idx();
		cout << endl;
		cout << "Parent" << " ";
		if (Parent != NULL)cout << Parent->idx();
		cout << endl;
		cout << "MediaBox " << MediaBox.ll_x << " " << MediaBox.ll_y << " " << MediaBox.ur_x << " " << MediaBox.ur_y << endl;
		if (Contents != NULL)cout << Contents->idx();
		cout << "---------------" << endl;
	}

	Name Type;
	OBJ * Parent;
	//Date LastModified;
	OBJ * Resources;
	Rectangle MediaBox;
	//Rectangle CropBox;
	//Rectangle BleedBox;
	//Rectangle TrimBox;
	//Rectangle ArtBox;
	//OBJ BoxColorInfo;
	OBJ * Contents;
	
	//int Rotate;
	//OBJ Group;
	//Stream Thumb;
	//Array B;
	//Number Dur;
	//OBJ Trans;
	//Array Annots;
	//OBJ AA;
	//Stream Metadata;
	//OBJ PieceInfo;
	//int StructParents;
	//string ID;
	//Number PZ;
	//OBJ SeparationInfo;
	//Name Tabs;
	//Name TemplateInstantiated;
	//OBJ PresSteps;
	//Number UserUnit;
	//OBJ VP;
};


#endif