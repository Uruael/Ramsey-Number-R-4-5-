#ifndef RULES_H_
#define RULES_H_
#include "nauty.h"
#include "Intervals.h"

typedef unsigned int Locset;

int a(Locset u, Locset v, struct IntervalList intervals, setword *g2);
int b(Locset u, Locset v, struct IntervalList intervals, setword *g2);
int c(Locset u, Locset v, Locset w, struct IntervalList intervals, setword *g2);
int d(Locset u, Locset v, Locset w, Locset z, struct IntervalList intervals, setword *g2);

#endif // HRULES_H_
