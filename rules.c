#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hrules.h"
#include "Intervals.h"

int a(Locset u, Locset v, IntervalList intervals ,setword *g2)
{
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    if (U.bottom & V.bottom & h1(U.bottom & U.bottom,g2))
        return 1;
    else if ( U.top == V.top ^ ( h1(U.bottom & V.bottom, g2) & V.bottom ))
        return 0;

    getInterval(intervals, u).top = U.top ^ ( h1(U.bottom & V.bottom, g2) & V.bottom );
    return a(u, v, intervals , g2);
}


int b(Locset u, Locset v, IntervalList intervals ,setword *g2)
{
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    if (h3((U.top | V.top), g2) ^ (U.top | V.top))
        return 1;
    else if ( U.bottom == U.bottom | ( h3(U.top & V.top, g2) ^ V.top ))
        return 0;

    getInterval(intervals, u).bottom == U.bottom | ( h3(U.top & V.top, g2) ^ V.top )
    return b(u, v, intervals , g2);
}


int c(Locset u, Locset v, Locset w, IntervalList intervals, setword *g2);
{
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    Interval W = getInterval(intervals, w);
    if (h3(U.top | V.top | W.top) ^ (U.top | V.top | W.top))
        return 1;
    else if ( U.bottom == U.bottom | ( h3(U.top & V.top, g2) ^ (V.top | W.top)))
        return 0;
     getInterval(intervals, u).bottom = U.bottom | ( h3(U.top & V.top, g2) ^ (V.top | W.top));
     return c(u, v, w, intervals, g2);
}

int d(Locset u, Locset v, Locset w, Locset z, IntervalList intervals, setword *g2);
{
    Interval U = getInterval(intervals, u);
    Interval V = getInterval(intervals, v);
    Interval W = getInterval(intervals, w);
    Interval Z = getInterval(intervals, z);
    if ( ~(U.top | V.top | W.top | Z.top) == 0 )
        return 1;
    else if (U.bottom == (U.bottom | ~(V.top | W.top | Z.top)))
        return 0;
    getInterval(intervals, u).bottom = (U.bottom | ~(V.top | W.top | Z.top));
    return d(u, v, w, z, intervals, g2);
}
