#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hrules.h"
#include "Intervals.h"

#define FAIL 0
#define NOT_CHANGED 1
#define CHANGED 2

int a(Locset u, Locset v, IntervalList intervals, Locset *g2)
{
<<<<<<< Updated upstream
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    if (U.bottom & V.bottom & h1(U.bottom & U.bottom,g2))
        return FAIL;
    else if ( U.top == V.top ^ ( h1(U.bottom & V.bottom, g2) & V.bottom ))
        return NOT_CHANGED;

    getInterval(intervals, u).top = U.top ^ ( h1(U.bottom & V.bottom, g2) & V.bottom );
    if (a(u, v, intervals , g2) == FAIL)
    {
        return FAIL;
    }
    else return CHANGED;
=======
    Interval U = intervals[u];
    Interval V = intervals[v];
    if (U.bottom & V.bottom & h1(U.bottom & V.bottom,G.G))
        return RULE_FAIL;
    else if ( U.top == (V.top & ~ ( h1(U.bottom & V.bottom, G.G) & V.bottom )))
        return NOT_CHANGED;

    intervals[u].top = (V.top & ~ ( h1(U.bottom & V.bottom, G.G) & V.bottom ));

   return CHANGED;
>>>>>>> Stashed changes
}


int b(Locset u, Locset v, IntervalList intervals, Locset *g2)
{
<<<<<<< Updated upstream
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    if (h3((U.top | V.top), g2) ^ (U.top | V.top))
        return FAIL;
    else if ( U.bottom == U.bottom | ( h3(U.top & V.top, g2) ^ V.top ))
        return NOT_CHANGED;

    getInterval(intervals, u).bottom == U.bottom | ( h3(U.top & V.top, g2) ^ V.top );

    if (b(u, v, intervals , g2) == FAIL)
    {
        return FAIL;
    }
    else return CHANGED;
=======
    Interval U = intervals[u];
    Interval V = intervals[v];
    if ((h3((U.top | V.top), G.deg, G.G) & ~ (U.top | V.top)))
        return RULE_FAIL;
    else if ( U.bottom == (U.bottom | ( h3(U.top | V.top, G.deg, G.G) & ~V.top) ))
        return NOT_CHANGED;

    intervals[u].bottom = (U.bottom | ( h3(U.top | V.top, G.deg, G.G) & ~V.top ));

    return CHANGED;
>>>>>>> Stashed changes
}


int c(Locset u, Locset v, Locset w, IntervalList intervals, Locset *g2);
{
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    Interval W = getInterval(intervals, w);
    if (h3(U.top | V.top | W.top) ^ (U.top | V.top | W.top))
        return FAIL;
    else if ( U.bottom == U.bottom | ( h3(U.top & V.top, g2) ^ (V.top | W.top)))
        return NOT_CHANGED;

     getInterval(intervals, u).bottom = U.bottom | ( h3(U.top & V.top, g2) ^ (V.top | W.top));

<<<<<<< Updated upstream
    if (c(u, v, w, intervals, g2) == FAIL)
    {
        return FAIL;
    }
    else return CHANGED;
=======
     return CHANGED;
>>>>>>> Stashed changes
}

int d(Locset u, Locset v, Locset w, Locset z, IntervalList intervals, Locset *g2);
{
<<<<<<< Updated upstream
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    Interval W = getInterval(intervals, w);
    Interval Z = getInterval(intervals, z);
    if ( ~(U.top | V.top | W.top | Z.top) == 0 )
        return FAIL;
    else if (U.bottom == (U.bottom | ~(V.top | W.top | Z.top)))
=======
    Interval U = intervals[u];
    Interval V = intervals[v];
    Interval W = intervals[w];
    Interval Z = intervals[z];

    if ( (U.top | V.top | W.top | Z.top) != ((~0) << (32 - G.deg)) )
        return RULE_FAIL;

    else if (U.bottom == (U.bottom | (((~0) << (32 - G.deg)) & ~(V.top | W.top | Z.top))))
>>>>>>> Stashed changes
        return NOT_CHANGED;

    getInterval(intervals, u).bottom = (U.bottom | ~(V.top | W.top | Z.top));

<<<<<<< Updated upstream
    if(d(u, v, w, z, intervals, g2) == 0)
    {
        return FAIL;
    }
    else return CHANGED;
=======
    return CHANGED;
    //return NOT_CHANGED;
>>>>>>> Stashed changes
}
