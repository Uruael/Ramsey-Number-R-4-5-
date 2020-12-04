#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nauty.h"
#include "Gluing.h"
#include <string.h>
#include "rules.c"

int count = 0;

typedef unsigned int Locset;

Graph putEdgeAt(Graph G, int row, int column)
{
    G.G[row] = G.G [row] | (1 << (32 - column));
}

Locset nextCone(Interval I, Locset cone)
{
    for(; cone < I.top; ++cone)
    {
        if( (cone&I.bottom == cone) && (cone|I.top == I.top) )
            return cone;
        else return 999;
    }
}


void constructGraphs(Graph G, Graph H, Graph F, Interval*intervals, int depth)
{
    Interval interval = intervals[depth];
    Locset cone = interval.bottom;
    while(cone <= interval.top)
    {
        F.G[depth] = G.G[depth] | (cone<<G.deg);

        for(int i = 0; i < H.deg; i++)
        {
            if(cone & (1<<(32-i)))
                F.G[G.deg+i] = F.G[G.deg+i] | (1 << (32-depth));
        }


        if(depth == G.deg)
        {
            F.deg = G.deg + H.deg;
            count++;
            //zapis do pliku
            FILE *f = fopen("graph.txt", "wb");
            fwrite(F.G, sizeof(Locset), WORDSIZE * F.deg, f);
            fclose(f);
        }
        else
        {
            constructGraphs(G, H, F, intervals, depth+1);
        }
        cone = nextCone(interval, cone);
    }
    if(depth == G.deg)
    {
        return;
    }
}

void permuteIntervals(struct Graph G, struct Graph H, struct IntervalList intervals, struct Interval*chosenIntervals, int n, int intervalNumber)
{
    chosenIntervals[n] = getInterval(intervals, intervalNumber);
    //apply rules A-D here
    for(int i = 0; i < n; i++)
    {
        IntervalList intervalsCopy = intervals;
        int flag = 0;
        int result;

        while(flag != NOT_CHANGED)
        {
             //zasada A
            if(G.G[n] & (1<<(31-i)))
            {
                result = a(i, n, chosenIntervals, G);
                if(result == RULE_FAIL)
                    return;
                flag |= result;
            }
            else
            {
                //zasada B
                result = b(i, n, chosenIntervals, G);
                if(result == RULE_FAIL)
                    return;
                flag |= result;
                for(int j = i+1; j < n; j++)
                    if(!(G.G[n] & (1<<(31-j)) & G.G[i]))
                    {
                        //zasada C
                        result = c(i, j, n, chosenIntervals, G);
                        if(result == RULE_FAIL)
                            return;
                        flag |= result;

                        for(int k = j+1; k < n; k++)
                        {
                            //zasada D
                            if(!(G.G[n] & G.G[i] & G.G[j] & (1<<(31-k))))
                            {
                                result = d(i, j, k, n, chosenIntervals, G);
                                if(result == RULE_FAIL)
                                    return;
                                flag |= result;
                            }
                        }
                    }
                }
            }

        }


    if(G.deg == n+1)
    {

        Graph F;
        for(int i = 0; i<24; i++)
            F.G[i] = G.G[i];
        F.deg = 24;
        for(int i = G.deg; i < F.deg; i++)
        {
            F.G[i] = ( H.G[i-G.deg]<<G.deg );
        }
        constructGraphs(G, H, F,chosenIntervals, 0);
        return;
    }
    else
    {
        int index = 0;
        IntervalElement * next = intervals.first;
         while (next != NULL){
            permuteIntervals(G, H, intervals, chosenIntervals, n+1, index++);
            next = next->next;
        }
    }
}

void Glue(struct Graphs G, struct Graphs H)
{

    for(int i = 0; i < H.length; i++)
    {
        IntervalList intervals = ZnajdzPrzedzialy(H.graphs[i]);

        for(int j = 0; j < G.length; j++)
        {
            Interval* chosenIntervals = malloc(sizeof(Interval)*G.graphs[j].deg);
            int index = 0;
            IntervalElement * next = intervals.first;
             while (next != NULL){
                permuteIntervals(G.graphs[j], H.graphs[i], intervals, chosenIntervals, 0, index++);
                next = next->next;
            }
        }
    }

}
