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
	void print()
	{
		cout << "---------------" << endl;
		cout << "Trailer :" << endl;
		cout << "Size" << " " << Size << endl;
		cout << "Prev" << " " << Prev << endl;
		cout << "Root" << " " << Root->idx() << endl;
		cout << "Info" << " " << Info->idx() << endl;
		cout << "----------------" << endl;
	}





	int parse(ifstream &readpdf, streamoff & off)
	{
		string sRes;
		char c;
		readpdf.seekg(off, ios_base::beg);
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


				if (s == "Size")
				{
					int iSize = -1;
					if (parseInt(readpdf, off, iSize) != 0)
					{
						cout << "error" << endl;
						return -1;
					}
					Size = iSize;
				}
				else if (s == "Prev")
				{
					int iPrev = -1;
					if (parseInt(readpdf, off, iPrev) != 0)
					{
						cout << "error" << endl;
						return -2;
					}
					Prev = iPrev;
				}
				else if (s == "Root")
				{
					OBJ * obj = new OBJ();
					if (parseObj(readpdf, off, obj) != 0)
					{
						cout << "error" << endl;
						return -3;
					}
					Root = obj;
				}
				else if (s == "Info")
				{
					OBJ * obj = new OBJ();
					if (parseObj(readpdf, off, obj) != 0)
					{
						cout << "error" << endl;
						return -3;
					}
					Info = obj;
				}
				else
				{
				}
				readpdf.seekg(off, ios_base::beg);
			}
		}
		return 0;
	}

	streamoff  crossRefTable_offset;
	int Size;
	int Prev;
	OBJ * Root;
	//OBJ Encrypt;
	OBJ * Info;
	//Array ID;
};


#endif