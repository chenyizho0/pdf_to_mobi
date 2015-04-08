#ifndef _CATALOG_H_
#define _CATALOG_H_
#include "name.h"
#include "OBJ.h"
#include <string>
using namespace std;
class Name;
class Catalog
	:public OBJ
{
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
	Stream Metadata;
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