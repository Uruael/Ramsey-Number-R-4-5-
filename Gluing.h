#ifndef GLUING_H_INCLUDED
#define GLUING_H_INCLUDED
#include "nauty.h"
#include "Intervals.h"

typedef unsigned int Locset;

void permuteIntervals(struct Graph G, struct Graph H, struct IntervalList intervals, struct Interval*chosenIntervals, int n, int intervalNumber);
void Glue(struct Graphs G, struct Graphs H);

#endif // GLUING_H_INCLUDED
