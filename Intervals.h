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

typedef struct IntervalList
{
	IntervalElement *first;
} IntervalList;

typedef struct TwoIntervals
{
    Interval first;
    Interval second;
} TwoIntervals;

TwoIntervals PodzielPrzedzial(Interval toobig);

TwoIntervals RodzielPrzedzial(Interval toobig, int vertex);

struct Interval getInterval( IntervalList intervals, int n);

int ZnajdzWierzcholekDoWyrzucenia( Graph G, Locset check, Locset mask);

struct IntervalList PolaczListy( IntervalList p1,  IntervalList p2);

int CzyKlikaWBottom( Graph G, Locset check);

struct IntervalList PodzialPrzedzialu( Graph G,  Interval P);

Locset PobierzTop( Graph G);

struct IntervalList ZnajdzPrzedzialy( Graph G);

void testIntervals();

void AppendToEnd(IntervalElement *i, IntervalList *l);

struct DisallowedStructs GetK3List(Graph F);
IntervalList TworzI(Graph F);

int stozkiWPrzedziale(Interval i);

#endif // INTERVALS_H_INCLUDED
