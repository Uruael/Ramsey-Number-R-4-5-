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
} Graphs;


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


struct Interval getInterval(struct IntervalList intervals, int n);

Interval* getIntervalRef(IntervalList intervals, int n);

int ZnajdzWierzcholekDoWyrzucenia(struct Graph G, Locset check, Locset mask);

struct IntervalList PolaczListy(struct IntervalList p1, struct IntervalList p2);

int CzyKlikaWBottom(struct Graph G, Locset check);

struct IntervalList PodzialPrzedzialu(struct Graph G, struct Interval P);

Locset PobierzTop(struct Graph G);

struct IntervalList ZnajdzPrzedzialy(struct Graph G);

void testIntervals();

#endif // INTERVALS_H_INCLUDED
