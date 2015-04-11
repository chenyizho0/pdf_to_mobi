#ifndef PAGE_H_
#define PAGE_H_

#include "rectangle.h"
#include "Name.h"
#include "OBJ.h"
class Page
{
public:
	Name Type;
	OBJ Parent;
	//Date LastModified;
	OBJ Resources;
	Rectangle MediaBox;
	//Rectangle CropBox;
	//Rectangle BleedBox;
	//Rectangle TrimBox;
	//Rectangle ArtBox;
	OBJ BoxColorInfo;
	//Stream or Array Contents;
	
	int Rotate;
	OBJ Group;
	//Stream Thumb;
	Array B;
	//Number Dur;
	OBJ Trans;
	Array Annots;
	OBJ AA;
	//Stream Metadata;
	OBJ PieceInfo;
	int StructParents;
	string ID;
	//Number PZ;
	OBJ SeparationInfo;
	Name Tabs;
	Name TemplateInstantiated;
	OBJ PresSteps;
	//Number UserUnit;
	OBJ VP;
};


#endif