#ifndef VERTEX_EXTENSION_H_INCLUDED
#define VERTEX_EXTENSION_H_INCLUDED


#include "Intervals.h"
enum DisallowedStructType {
	clique = 0,
	indSet = 1

};

typedef struct DisallowedStruct {
	int vert1;
	int vert2;
	int vert3;
	int vert4;
	enum DisallowedStructType type;
}DisallowedStruct;



typedef struct DisallowedStructs {
	int number;
	DisallowedStruct* n;
}DisallowedStructs;

DisallowedStructs GetDisallowedGroups(Graph F);
void AddDisallowed(DisallowedStructs *ret, enum DisallowedStructType type, int v1, int v2, int v3, int v4);
int isAllInLocset(Locset l, DisallowedStruct z) ;
int isNoneinLocset(Locset l, DisallowedStruct z) ;
void CutToThree(Interval currentI, DisallowedStruct dir, IntervalList *target);
void CutToFour(Interval currentI, DisallowedStruct dir, IntervalList *target);
IntervalList VertexExtension(Graph F);
#endif // VERTEX_EXTENSION_H_INCLUDED
