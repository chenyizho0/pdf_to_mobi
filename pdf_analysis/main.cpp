#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include "tools.h"
#include "trailer.h"
#include "catalog.h"
using namespace std;


class PdfRead
{
public:
	PdfRead()
		:crossRefTable_offset(0)
	{
	}
	~PdfRead()
	{
		if (readpdf_.is_open())
		{
			readpdf_.close();
		}
	}
	int init(const char * pdf_file_name)
	{
		readpdf_.open(pdf_file_name, ifstream::binary);
		return 0;
	}
	int readCrossRefTable(streamoff & endoff)
	{
		streamoff off = crossRefTable_offset;
		if (crossRefTable_offset == 0)
			return -1;
		string sStart;
		if (getContent(readpdf_, off, sStart, 1000, "\n", " ",off) != 0)
		{
			cout << "error " << endl;
			return -1;
		}
		int startnum = stringToInt(sStart);
		off = off + sStart.size() + 1;
		string sObjNum;
		if (getContent(readpdf_, off, sObjNum, 1000, " ", "\n", off) != 0)
		{
			cout << "error " << endl;
			return -2;
		}
		int objnum = stringToInt(sObjNum);
		streamoff t_off;
		int t_generation_num;
		bool t_key;
		string sOff, sGenNum, sKey;
		off = off + sObjNum.size();
		for (int i = 0; i < objnum; i++)
		{
			if (getContent(readpdf_, off, sOff, 1000, "\n", " ", off) != 0)
			{
				cout << "error" << endl;
				return -3;
			}
			t_off = stringToInt(sOff);
			off = off + sOff.size();
			if (getContent(readpdf_, off, sGenNum, 1000, " ", " ", off) != 0)
			{
				cout << "error" << endl;
				return -4;
			}
			t_generation_num = stringToInt(sGenNum);
			off = off + sGenNum.size() + 1;
			if (getContent(readpdf_, off,sKey, 1000, " ", "\n", off) != 0)
			{
				cout << "error" << endl;
				return -4;
			}
			t_key = (sKey == "n");
			vecCrossRef.push_back(CrossRefItem(t_off, startnum + i, t_generation_num, t_key));
			off = off + sKey.size();
		}
		endoff = off;
		return 0;
	}
	int getCrossRef_Offset()
	{
		streamoff off = -1;
		string sCrossRef_off;
		if (getContentVer(readpdf_, off, sCrossRef_off, 1000, "\n", "\n",off) != 0)
		{
			cout << "error" << endl;
			return -1;
		}
		crossRefTable_offset = stringToInt(sCrossRef_off);
		cout << crossRefTable_offset << endl;
		return 0;
	}
	int parseTrailer(streamoff traileroff)
	{
		return trailer_.parse(readpdf_,traileroff);
	}
	int parseCatalog()
	{
		streamoff catalog_off;
		int catalog_idx = trailer_.Root.idx();
		catalog_off = vecCrossRef[catalog_idx].off;
		return catalog_.parse(readpdf_,catalog_off);
	}
	Trailer getTrailer()
	{
		return this->trailer_;
	}


//for debug
	void printCrossTable()
	{
		for (int i = 0; i < vecCrossRef.size(); i++)
		{
			cout << vecCrossRef[i].off << " " << vecCrossRef[i].objnum << " " << vecCrossRef[i].generation_num << " " << vecCrossRef[i].key << endl;
		}
	}
	void printTrailer()
	{
		trailer_.print();
	}
	void printCatalog()
	{
		catalog_.print();
	}
private:
	
private:
	ifstream readpdf_;
	streamoff  crossRefTable_offset;
	class CrossRefItem
	{
	public:
		CrossRefItem(streamoff o, int obj, int g, bool k)
			:off(o), objnum(obj), generation_num(g), key(k)
		{
		}
		streamoff off;
		int objnum;
		int generation_num;
		bool key;
	};
	vector<CrossRefItem> vecCrossRef;
	vector<OBJ> vecObj;
	Trailer trailer_;
	Catalog catalog_;
};



int main()
{
	
	PdfRead pdfread;
	pdfread.init("F:\\pdf_analysis\\laixukai.pdf");
	int iRet = 0;
	streamoff off;
	iRet = pdfread.getCrossRef_Offset();
	iRet = pdfread.readCrossRefTable(off);
	pdfread.printCrossTable();
	iRet = pdfread.parseTrailer(off);
	pdfread.printTrailer();
	iRet = pdfread.parseCatalog();
	pdfread.printCatalog();
	return 0;
}
