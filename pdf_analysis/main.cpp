#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include "tools.h"
#include "trailer.h"
#include "pagetreenode.h"
#include "catalog.h"
#include "test.h"
#include "page.h"
#include "stream.h"

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
			vecObj.push_back(NULL);
			off = off + sKey.size();
		}
		endoff = off;
		return 0;
	}
	int getCrossRef_Offset()
	{
		streamoff off = -3;
		string sCrossRef_off;
		if (getContentVer(readpdf_, off, sCrossRef_off, 1000, "\n", "\n",off) != 0)
		{
			cout << "error" << endl;
			return -1;
		}
		cout << sCrossRef_off << endl;
		crossRefTable_offset = stringToInt(sCrossRef_off);
		cout <<  crossRefTable_offset << endl;
		return 0;
	}
	int parseTrailer(streamoff traileroff)
	{
		return trailer_.parse(readpdf_,traileroff);
	}
	int parseCatalog()
	{
		catalog_ = new Catalog();
		streamoff catalog_off;
		int catalog_idx = trailer_.Root->idx();
		catalog_off = vecCrossRef[catalog_idx].off;
		int iRet = catalog_->parse(readpdf_,catalog_off);
		vecObj[catalog_idx] = catalog_;
		return iRet;
	}
	Trailer getTrailer()
	{
		return this->trailer_;
	}
	//ÉîËÑpage tree
	int parsePageTree()
	{
		OBJ *obj = new OBJ();
		streamoff pageroot_off;
		obj->setIdx(catalog_->Pages->idx());
		parsePageTreeNode(obj);
		pageRoot_ = (PageTreeNode*) vecObj[catalog_->Pages->idx()];
		return 0;
	}
	void parsePageTreeNode(OBJ * node)
	{
		if (node == NULL)return;
		int node_idx = node->idx();
		cout << "this->" << node_idx << endl;
		streamoff node_off = vecCrossRef[node_idx].off;
		string sType;
		node->getType(readpdf_, node_off, sType);
		if (sType == "Page")
		{
			Page * p = new Page();
			p->parse(readpdf_, node_off);
			vecObj[node_idx] = p;
			int cont_idx = p->Contents->idx();
			node_off = vecCrossRef[cont_idx].off;
			Stream * cont = new Stream();
			cont->parse(readpdf_, node_off);
			vecObj[cont_idx] = cont;
			cout << "page " << node_idx << endl << endl << endl << endl << endl;
			cout << cont->decodeData << endl;
			return;
		}
		if (sType == "Pages")
		{
			PageTreeNode * pt = new PageTreeNode();
			pt->parse(readpdf_, node_off);
			vecObj[node_idx] = pt;
			for (int i = 0; i < pt->Kids.arr.size(); i++)
			{
				node_idx = pt->Kids.arr[i].idx();
				OBJ * obj = new OBJ();
				obj->setIdx(node_idx);
				parsePageTreeNode(obj);
			}
		}
		return;
	}


//for debug
	void printTree()
	{
		printTreeNode(pageRoot_);
	}
	void printTreeNode(OBJ * node)
	{
		if (node == NULL)
		{
			return;
		}
		string sType;
		int node_idx = node->idx();
		streamoff node_off = vecCrossRef[node_idx].off;
		node->getType(readpdf_, node_off, sType);
		if (sType == "Page")
		{
			Page * p = (Page*)node;
			p->print();
			return;
		}
		if (sType == "Pages")
		{
			PageTreeNode * pt = (PageTreeNode *)vecObj[node_idx];
			for (int i = 0; i < pt->Kids.arr.size(); i++)
			{
				node_idx = pt->Kids.arr[i].idx();
				printTreeNode(vecObj[node_idx]);
			}
			return;

		}
	}
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
		catalog_->print();
	}
	void printPageTreeNode()
	{
		pageRoot_->print();
	}
	int testGetContent(streamoff off, string a,string b)
	{
		string sRes;
		getContent(readpdf_, off, sRes, 1000, a, b, off);
		cout << sRes << endl;
		return 0;
	}
	
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
	vector<OBJ *> vecObj;
	Trailer trailer_;
	Catalog * catalog_;

	PageTreeNode * pageRoot_;
};





int main()
{
	PdfRead pdfread;
	//pdfread.init("F:\\pdf_analysis\\laixukai.pdf");
	pdfread.init("C:\\Users\\chenyizho\\Desktop\\Jorge Luis Borges - Collection.pdf");
	int iRet = 0;
	streamoff off;
	iRet = pdfread.getCrossRef_Offset();
	iRet = pdfread.readCrossRefTable(off);
	//pdfread.printCrossTable();
	iRet = pdfread.parseTrailer(off);
	pdfread.printTrailer();
	iRet = pdfread.parseCatalog();
	pdfread.printCatalog();
	iRet = pdfread.parsePageTree();
	//pdfread.printTree();
	return 0;
}
