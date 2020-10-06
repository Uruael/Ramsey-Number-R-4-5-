#ifndef HRULES_H_
#define HRULES_H_
#include "nauty.h"

typedef unsigned int Locset;


int h1(Locset vertex, setword *g2);
int h2(Locset vertex, int n, setword *g2);
int h3_n3(Locset vertex, int i, setword *g2);
int h3(Locset vertex, int n, setword *g2);

#endif // HRULES_H_
