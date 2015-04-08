#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <fstream>
#include <string>
#include "OBJ.h"
using namespace std;


int stringToInt(string s)
{
	if (s.size() == 0)return 0;
	int pos = 0;
	int k = 1;
	if (s[pos] == '-')
	{
		k = -1;
		pos++;
	}
	else if (s[pos] == '+')
	{
		pos++;
	}
	int ans = 0;
	for (int i = pos; i < s.size(); i++)
	{
		ans = ans * 10 + s[i] - '0';
	}
	return ans * k;
}

int getContent(ifstream &readpdf, streamoff  off, string &sRes, int maxlen, string sBeg, string sEnd,streamoff &startpos)
{
	readpdf.seekg(off, ios_base::beg);
	char c;
	streamoff iBeg = -1;
	streamoff curoff = off;
	while (true)
	{
		readpdf.read((char *)&c, sizeof(c));
		int  k = 0;
		if (c == sBeg[k])
		{
			k++;
			for (; curoff - off + k < maxlen && k < sBeg.size(); k++)
			{
				readpdf.read((char *)&c, sizeof(c));
				if (c != sBeg[k])
				{
					break;
				}
			}
			if (k == sBeg.size())
			{
				//cout << "beg" << endl;
				iBeg = curoff;
				break;
			}
			readpdf.seekg(curoff + 1, ios_base::beg);
		}
		curoff++;
	}
	if (iBeg == -1)
	{
		return -1;
	}
	readpdf.seekg(iBeg + sBeg.size(), ios_base::beg);
	streamoff iEnd = -1;
	curoff = iBeg+sBeg.size();
	while (true)
	{
		readpdf.read((char *)&c, sizeof(c));
		int  k = 0;
		if (c == sEnd[k])
		{
			k++;
			for (; curoff - off + k < maxlen && k < sEnd.size(); k++)
			{
				readpdf.read((char *)&c, sizeof(c));
				if (c != sEnd[k])
				{
					break;
				}
			}
			if (k == sEnd.size())
			{
				//cout << "end" << endl;
				iEnd = curoff;
				break;
			}
			readpdf.seekg(curoff + 1, ios_base::beg);
		}
		curoff++;
	}
	if (iEnd == -1)
	{
		return -1;
	}
	readpdf.seekg(iBeg+sBeg.size(), ios_base::beg);
	char buf[10000];
	readpdf.read((char *)buf, iEnd - iBeg - sBeg.size());
	sRes = sRes.assign(buf, iEnd-iBeg-sBeg.size());
	startpos = iBeg;
	return 0;
}


int getContentVer(ifstream &readpdf, streamoff  off, string &sRes, int maxlen, string sBeg, string sEnd,streamoff & startpos)
{
	for (int i = 0; i < sBeg.size() / 2; i++)
	{
		char tmp = sBeg[i];
		sBeg[i] = sBeg[sBeg.size() - 1 - i];
		sBeg[sBeg.size() - 1 - i] = tmp;
	}
	for (int i = 0; i < sEnd.size() / 2; i++)
	{
		char tmp = sEnd[i];
		sEnd[i] = sEnd[sEnd.size() - 1 - i];
		sEnd[sEnd.size() - 1 - i] = tmp;
	}
	readpdf.seekg(off, ios_base::end);
	char c;
	streamoff iEnd = -1;
	streamoff curoff = off;
	while (true)
	{
		readpdf.read((char *)&c, sizeof(c));
		curoff--;
		readpdf.seekg(curoff, ios_base::end);
		int k = 0;
		if (c == sEnd[k])
		{
			k++;
			for (; k < sEnd.size() && off - curoff + k < maxlen; k++)
			{
				readpdf.read((char*)&c, sizeof(c));
				readpdf.seekg(curoff + k, ios_base::end);
				if (c != sEnd[k])
				{
					break;
				}
			}
			if (k == sEnd.size())
			{
				iEnd = curoff;
				break;
			}
			readpdf.seekg(curoff, ios_base::end);
		}
	}
	if (iEnd == -1)
	{
		return -1;
	}

	readpdf.seekg(iEnd-sEnd.size(), ios_base::end);
	streamoff iBeg = -1;
	curoff = iEnd-sEnd.size();
	while (true)
	{
		readpdf.read((char *)&c, sizeof(c));
		curoff--;
		readpdf.seekg(curoff, ios_base::end);
		int k = 0;
		if (c == sBeg[k])
		{
			k++;
			for (; k < sBeg.size() && off - curoff + k < maxlen; k++)
			{
				readpdf.read((char*)&c, sizeof(c));
				readpdf.seekg(curoff + k, ios_base::end);
				if (c != sBeg[k])
				{
					break;
				}
			}
			if (k == sBeg.size())
			{
				iBeg = curoff;
				break;
			}
			readpdf.seekg(curoff, ios_base::end);
		}
	}
	if (iBeg == -1)
	{
		return -2;
	}
	char buf[10000];
	readpdf.seekg(iBeg + sBeg.size()+1,ios_base::end);
	readpdf.read((char*)buf,iEnd-iBeg-sBeg.size() );
	sRes.assign(buf, iEnd - iBeg - sBeg.size());
	startpos = iBeg;
	return 0;
}

int parseInt(string & sSrc,size_t &startpos,int & iRet)
{
	
	string s;
	while (startpos < sSrc.size())
	{
		if (sSrc[startpos] <= '9' && sSrc[startpos] >= '0')
			break;
		startpos++;
	}
	while (startpos < sSrc.size())
	{
		if (sSrc[startpos] <= '9' && sSrc[startpos] >= '0')
		{
			s.append(1, sSrc[startpos]);
		}
		else
		{
			break;
		}
		startpos++;
	}
	iRet = stringToInt(s);
	return 0;
}

int parseObj(string & sSrc, size_t &startpos, OBJ &obj)
{
	int objnum = -1;
	if (parseInt(sSrc, startpos, objnum) != 0)
	{
		cout << "error" << endl;
		return -1;
	}
	int generation_num = -1;
	if (parseInt(sSrc, startpos, generation_num) != 0)
	{
		cout << "error" << endl;
		return -2;
	}
	while (startpos < sSrc.size())
	{
		if (sSrc[startpos] == ' ')
		{
			startpos++;
		}
		else if (sSrc[startpos] != 'R')
		{
			cout << "error not R" << endl;
			return -3;
		}
		else
		{
			startpos++;
			break;
		}
	}
	obj.setIdx(objnum);
	return 0;
}


#endif