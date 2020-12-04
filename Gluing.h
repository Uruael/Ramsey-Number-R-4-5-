#ifndef GLUING_H_INCLUDED
#define GLUING_H_INCLUDED
#include "nauty.h"
#include "Intervals.h"

typedef unsigned int Locset;

void permuteIntervals(Graph G, Graph H, IntervalList intervals, Interval*chosenIntervals, int n, int intervalNumber);
int Glue(struct Graphs G, struct Graphs H);

#endif // GLUING_H_INCLUDED
