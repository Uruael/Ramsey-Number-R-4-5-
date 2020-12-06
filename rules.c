#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hrules.h"
#include "Intervals.h"

#define RULE_FAIL 0
#define NOT_CHANGED 1
#define CHANGED 2

int a(Locset u, Locset v, Interval* intervals, Graph G)
{
    Interval U = intervals[u];
    Interval V = intervals[v];
    if (U.bottom & V.bottom & h1(U.bottom & V.bottom,G.G))
        return RULE_FAIL;
    else if ( U.top == (V.top & ~ ( h1(U.bottom & V.bottom, G.G) & V.bottom )))
        return NOT_CHANGED;

    intervals[u].top = (V.top & ~ ( h1(U.bottom & V.bottom, G.G) & V.bottom ));

   return CHANGED;
}


int b(Locset u, Locset v, Interval*intervals, Graph G)
{
    Interval U = intervals[u];
    Interval V = intervals[v];
    if ((h3((U.top | V.top), G.deg, G.G) & ~ (U.top | V.top)))
        return RULE_FAIL;
    else if ( U.bottom == (U.bottom | ( h3(U.top | V.top, G.deg, G.G) & ~V.top) ))
        return NOT_CHANGED;

    intervals[u].bottom = (U.bottom | ( h3(U.top | V.top, G.deg, G.G) & ~V.top ));

    return CHANGED;
}


int c(Locset u, Locset v, Locset w, Interval* intervals, Graph G)
{
    Interval U = intervals[u];
    Interval V = intervals[v];
    Interval W = intervals[w];
    if (h2((U.top | V.top | W.top), G.deg, G.G) & ~ (U.top | V.top | W.top))
        return RULE_FAIL;
    else if ( U.bottom == (U.bottom | ( h2(U.top | V.top | W.top, G.deg, G.G) & ~ (V.top | W.top))))
        return NOT_CHANGED;

     intervals[u].bottom = (U.bottom | ( h2(U.top | V.top | W.top, G.deg, G.G) & ~ (V.top | W.top)));
   return CHANGED;
}

int d(Locset u, Locset v, Locset w, Locset z, Interval* intervals, Graph G)
{
    Interval U = intervals[u];
    Interval V = intervals[v];
    Interval W = intervals[w];
    Interval Z = intervals[z];
    if ( (U.top | V.top | W.top | Z.top) != ((~0) << (32 - G.deg)) )
        return RULE_FAIL;

    else if (U.bottom == (U.bottom | (((~0) << (32 - G.deg)) & ~(V.top | W.top | Z.top))))
        return NOT_CHANGED;
    intervals[u].bottom = (U.bottom | ((~0) << (32 - G.deg) & ~(V.top | W.top | Z.top)));

    return CHANGED;
}
