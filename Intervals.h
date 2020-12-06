#ifndef INTERVALS_H_INCLUDED
#define INTERVALS_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nauty.h"
typedef unsigned int Locset;


typedef struct Graph
{
	Locset G[32];
	int deg;
} Graph;

typedef struct Graphs
{
	Graph *graphs;
	int length;
}Graphs;



typedef struct Interval
{
	Locset bottom;
	Locset top;
} Interval;


typedef struct IntervalElement
{
	Interval i;
	struct IntervalElement *next;
}IntervalElement;

typedef struct IntervalElementBackwards
{
	Interval i;
	struct IntervalElementBackwards *next;
	struct IntervalElementBackwards *previous;
}IntervalElementBackwards;

typedef struct IntervalListUndirected
{
	IntervalElementBackwards *first;
} IntervalListUndirected;


typedef struct IntervalList
{
	IntervalElement *first;
} IntervalList;

typedef struct TwoIntervals
{
    Interval first;
    Interval second;
} TwoIntervals;

IntervalListUndirected Undirect(IntervalList lista);

IntervalList Direct(IntervalListUndirected lista);

TwoIntervals PodzielPrzedzial(Interval toobig);

TwoIntervals RodzielPrzedzial(Interval toobig, int vertex);

struct Interval getInterval( IntervalList intervals, int n);

struct IntervalList PolaczListy( IntervalList p1,  IntervalList p2);

Locset PobierzTop( Graph G);

void AppendToFront(IntervalElement *i, IntervalList *l);

struct DisallowedStructs GetK3List(Graph F);
IntervalList TworzI(Graph F);

int stozkiWPrzedziale(Interval i);

Interval Polacz(IntervalListUndirected lista, IntervalElementBackwards*i, IntervalElementBackwards*j);

int CzyMoznaPolaczyc(Interval i, Interval j);

#endif // INTERVALS_H_INCLUDED
