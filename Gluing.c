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
    G.G[row] = G.G [row] | (1 << (31 - column));
}

int ApplyAD(Graph G, Graph H, Interval*chosenIntervals, int n)
{
    for(int i = 0; i < n; i++)
    {
        int flag = 0;
        int result;

        while(flag != NOT_CHANGED)
        {
            flag = 0;
             //zasada A
            if(G.G[n] & (1<<(31-i)))
            {
                result = a(i, n, chosenIntervals, H);
                if(result == RULE_FAIL)
                    return RULE_FAIL;
                flag |= result;
            }
            else
            {
                //zasada B
                result = b(i, n, chosenIntervals, H);
                if(result == RULE_FAIL)
                    return RULE_FAIL;;
                flag |= result;
                for(int j = i+1; j < n; j++)
                    if(!(G.G[n] & (1<<(31-j)) & G.G[i]))
                    {
                        //zasada C
                        result = c(i, j, n, chosenIntervals, H);
                        if(result == RULE_FAIL)
                            return RULE_FAIL;
                        flag |= result;

                        for(int k = j+1; k < n; k++)
                        {
                            //zasada D
                            if(!(G.G[n] & G.G[i] & G.G[j] & (1<<(31-k))))
                            {
                                result = d(i, j, k, n, chosenIntervals, H);
                                if(result == RULE_FAIL)
                                    return RULE_FAIL;
                                flag |= result;
                            }
                        }
                    }
                }
            }

        }
        return CHANGED;

}

void constructGraphs(Graph G, Graph H, Graph F, Interval*intervals, int depth)
{
    Interval interval = intervals[depth];
    TwoIntervals twoIntervals;

    int a=0;

    while(interval.bottom != interval.top)
    {
        a++;
        printf("%d\n", a);
        twoIntervals = PodzielPrzedzial(interval);
        interval = twoIntervals.first;
        intervals[depth] = twoIntervals.first;
        Interval*newIntervals = (Interval*)malloc(sizeof(Interval)*G.deg);
        for(int i = 0; i < G.deg; i++)
            {
                newIntervals[i] = intervals[i];
        }

        newIntervals[depth] = twoIntervals.second;

        if(ApplyAD(G, H, newIntervals, depth) != RULE_FAIL)
        {
            constructGraphs(G, H, F, newIntervals, depth);
        }

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

    //printf("Intervals: %d Number: %d\n",n,intervalNumber);
    chosenIntervals[n] = getInterval(intervals, intervalNumber);
    //apply rules A-D here
    if(ApplyAD(G, H, chosenIntervals, n) == RULE_FAIL)
        return;


    if(G.deg == n+1)
    {

        Graph F;
        for(int i = 0; i<G.deg; i++)
            F.G[i] = G.G[i];
        F.deg = 24;
        for(int i = G.deg; i < F.deg; i++)
        {
            F.G[i] = ( H.G[i-G.deg]>>G.deg );
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
            printf("Test: %d i %d\n",i,j);
            Interval* chosenIntervals = malloc(sizeof(Interval)*G.graphs[0].deg);
            int index = 0;
            IntervalElement * next = intervals.first;
             while (next != NULL){
                permuteIntervals(G.graphs[j], H.graphs[i], intervals, chosenIntervals, 0, index++);
                next = next->next;
            }
        }
    }

}
