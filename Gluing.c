#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nauty.h"
#include "Intervals.h"
#include <string.h>

typedef unsigned int Locset;

<<<<<<< Updated upstream
Graph putEdgeAt(Graph G, int row, int column)
{
    G.G[row] = G.G [row] | (1 << column);
}
=======

>>>>>>> Stashed changes

Locset nextCone(Interval I, Locset cone)x
{
    for(; cone < I.top; ++cone)
    {
        if( (cone&I.bottom == I.bottom) && (i|I.top == I.top) )
            return cone;
    }
}


void constructGraphs(Graph G, Graph H, Graph F, Interval*intervals, int depth)
{
    Interval interval = intervals[depth];
    Locset cone = interval.bottom;
    while(cone != top)
    {
        F.G[depth] = G.G[depth] | (cone<<G.deg);

        for(int i = 0; i < H.deg; i++)
        {
            if(cone & (1<<i))
                F.G[G.deg+i] = F.G[G.deg+i] | (1 << depth);
        }


        if(depth+1 == G.deg)
        {
            //zapis do pliku
        }
        else constructGraphs(G, H, F, intervals, depth+1)
        cone = nextCone(interval, cone)
    }
    if(depth+1 == G.deg)
    {
        return;
    }
}

void permuteIntervals(Graph G, Graph H, IntervalList intervals, Interval*chosenIntervals, int n, int intervalNumber)
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
            if(G.G[n] & (1<<i))
            {
                result = a(i, n, intervals, G.G, 0);
                if(result == FAIL)
                    return;
                flag |= result;
                result = a(n, i, chosenIntervals, H);
                if(result == RULE_FAIL)
                    return RULE_FAIL;
                flag |= result;
            }
            else
            {
                //zasada B
<<<<<<< Updated upstream
                result = b(i, n, intervals, G.G, 0);
                if(result == FAIL)
                    return;
                flag |= result;
                for(int j = i+1; j < n; j++)
                    if(G.G[n] & (1<<j) & G.G[i])
=======
                result = b(i, n, chosenIntervals, H);
                if(result == RULE_FAIL)
                    return RULE_FAIL;

                flag |= result;
                result = b(n, i, chosenIntervals, H);
                if(result == RULE_FAIL)
                    return RULE_FAIL;

                flag |= result;
                for(int j = i+1; j < n; j++)
                    if(!(G.G[n] & (1<<(31-j))) & !(G.G[i] & (1<<(31-j))))
>>>>>>> Stashed changes
                    {
                        //zasada C
                        result = c(i, j, n, intervals, G.G, 0);
                        if(result == FAIL)
                            return;
                        flag |= result;
                        result = c(j, i, n, chosenIntervals, H);
                        if(result == RULE_FAIL)
                            return RULE_FAIL;
                        flag |= result;
                        result = c(n, i, j, chosenIntervals, H);
                        if(result == RULE_FAIL)
                            return RULE_FAIL;

                        flag |= result;

                        for(int k = j+1; k < n; k++)
                        {
                            //zasada D
<<<<<<< Updated upstream
                            if((G.G[n] & G.G[i] & G.G[j] & (1<<k))
=======
                            if(!(G.G[n] & (1<<(31-k))) & !(G.G[i] & (1<<(31-k))) & !(G.G[j] & (1<<(31-k))))
>>>>>>> Stashed changes
                            {
                                result = d(i, j, k, n, intervals, G.G, 0);
                                if(result == FAIL)
                                    return;
                                flag |= result;

                                result = d(j, i, k, n, chosenIntervals, H);
                                if(result == RULE_FAIL)
                                    return RULE_FAIL;
                                flag |= result;

                                result = d(k, j, i, n, chosenIntervals, H);
                                if(result == RULE_FAIL)
                                    return RULE_FAIL;
                                flag |= result;

                                result = d(n, j, k, i, chosenIntervals, H);
                                if(result == RULE_FAIL)
                                    return RULE_FAIL;
                                flag |= result;
                            }
                        }
                    }
                }
            }

        }
    }
<<<<<<< Updated upstream
=======
    F.deg = G.deg + H.deg;
    count++;
    FILE *f = fopen("graph.bin", "a+");
    fwrite(F.G, sizeof(Locset), F.deg, f);
    fclose(f);
    return;
}*/


void constructGraphs(Graph G, Graph H, Graph F, Interval*intervals, int depth)
{
    Interval interval = intervals[depth];
    TwoIntervals twoIntervals;

    int a=0;
    //printf("%d\n", a);
    while(interval.bottom != interval.top)
    {
        a++;
        //printf("%d\n", a);
        twoIntervals = PodzielPrzedzial(interval);
        interval = twoIntervals.first;
        intervals[depth] = twoIntervals.first;
        Interval*newIntervals = (Interval*)malloc(sizeof(Interval)*G.deg);
        for(int i = 0; i < G.deg; i++)
            {
                newIntervals[i] = intervals[i];
        }

        newIntervals[depth] = twoIntervals.second;


        constructGraphs(G, H, F, newIntervals, depth);
    }

    if(ApplyAD(G, H,intervals, depth) == RULE_FAIL)
    {
        return;
    }


    F.G[depth] = G.G[depth] | (interval.bottom>>G.deg);

    for(int i = 0; i < H.deg; i++)
    {
        if(interval.bottom & (1<<(31-i)))
            F.G[G.deg+i] = F.G[G.deg+i] | (1 << (31-depth));
    }


    if(depth+1 < G.deg)
    {
        constructGraphs(G, H, F, intervals, depth+1);
    }

    else
        {
            F.deg = G.deg + H.deg;
            count++;
            FILE *f = fopen("graph.bin", "a+");
            fwrite(F.G, sizeof(Locset), F.deg, f);
            fclose(f);
            return;
        }

}

void permuteIntervals(struct Graph G, struct Graph H, struct IntervalList intervals, struct Interval*chosenIntervals, int n, int intervalNumber)
{
    if(n<2 | n>10)
        printf("Vertex: %d Interval: %d\n",n,intervalNumber);
    chosenIntervals[n] = getInterval(intervals, intervalNumber);
    //apply rules A-D here
    if(ApplyAD(G, H, chosenIntervals, n) == RULE_FAIL)
    {
        return;
    }


>>>>>>> Stashed changes

    if(G.deg == n+1)
    {
        printf("Idsdsd");
        Graph F;
        F.G = G;
        F.deg = 24;
        for(int i = G.deg; i < F.deg; i++)
        {
            F.G[i] = ( H.G[i-G.deg]<<G.deg );
        }
        constructGraphs(G, H, F,chosenIntervals, 0)
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

void Glue(Graphs G, Graphs H)
{

    for(int i = 0; i < H.length; i++)
    {
        IntervalList intervals = ZnajdzPrzedzialy(H.graphs[i]);

        for(int j = 0; j < G.length; j++)
        {
            int index = 0;
            IntervalElement * next = intervals.first;
             while (next != NULL){
                permuteIntervals(G, H, intervals, chosenIntervals, 0, index++);
                next = next->next;
            }
        }
    }

}
