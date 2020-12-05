#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hrules.h"


int h1(Locset vertex, setword *g2)
{
    Locset result = 0;

    int position = 0;
    while (vertex)
    {
        TAKEBIT(position, vertex);
        result = (g2[position] | result);
    }

    return result;
}

int h2(Locset vertex, int n, setword *g2)
{
    Locset result = 0;
    int veryfi = (1<<n)- 1;
    veryfi = veryfi << (32 - n);
    for (int i = 0; i < n; i++)
    {
        int addpos = 1<< (31 - i);
        if (((g2[i] | addpos) | vertex) != veryfi)
        {

            result = result | addpos;
        }
    }
    return result;
}

int h3_n3(Locset vertex, int i, setword *g2, int n)
{

    setword wx = g2[i];
    int pos = 1<<(31 - i);
    setword mask = ALLMASK(n);
    wx = (wx | pos) ^ mask;

    while (wx)
    {
        int v;
        TAKEBIT(v, wx);

        pos = 1<<(31 - v);
        if (((wx & ~g2[v]) & ~vertex) && !(vertex & pos))
        {
            return 1;
        }
    }
    return 0;
}

int h3(Locset vertex, int n, setword *g2)
{
    Locset result = 0;
    for (int i = 0; i < n; i++)
    {
        int temp = 1<<(31 - i);
        if (h3_n3(vertex, i, g2, n))
        {
            result = result | temp;
        }
    }
    return result;
}
