#ifndef RULES_H_
#define RULES_H_
#include "nauty.h"
#include "Intervals.h"

typedef unsigned int Locset;

int a(Locset u, Locset v, struct IntervalList intervals, struct Graph G);
int b(Locset u, Locset v, struct IntervalList intervals, struct Graph G);
int c(Locset u, Locset v, Locset w, struct IntervalList intervals, struct Graph G);
int d(Locset u, Locset v, Locset w, Locset z, struct IntervalList intervals, struct Graph G);

#endif // HRULES_H_
