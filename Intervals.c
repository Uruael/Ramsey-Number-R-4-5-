#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nauty.h"
typedef unsigned int Locset;

int IntervalDivisionCounter = 0;

const Locset LocbitInterval[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000};

typedef struct Graph
{
	Locset *G;
	int deg;
} Graph;

typedef struct Interval
{
	Locset bottom;
	Locset top;
} Interval;

typedef struct IntervalElement
{
	Interval i;
	struct IntervalElement *next;
}IntervalElement;

typedef struct IntervalList
{
	IntervalElement *first;
} IntervalList;

Interval getInterval(IntervalList intervals, int n)
{
    IntervalElement*result = intervals.first;
    for(int i = 0; i < n; i++)
    {
        result = result->next;
    }
    return result->i;
}

int ZnajdzWierzcholekDoWyrzucenia(Graph G, Locset check, Locset mask)
{
	Locset masked = check & (check ^ mask);
	while (masked) {
		int position;
		TAKEBIT(position, masked);
		position = 31 - position;
        Locset wx = G.G[position] & check;
		while(wx){
            int v;
            TAKEBIT(v, wx);
            if (G.G[31- v] & wx){
                return position;
            }
		}
	}
	return -1;
}

IntervalList PolaczListy(IntervalList p1, IntervalList p2) {

	if (p1.first == NULL) return p2;
	if (p2.first == NULL) return p1;
	IntervalElement *last = p1.first;
	while (last->next != NULL) {
		last = last->next;
	}
    last->next = p2.first;

	return p1;
}

int CzyKlikaWBottom(Graph G, Locset check)
{
    Locset checkInitial = check;
	while (check) {
		int position;
		TAKEBIT(position, check);
		position = 31 - position;
        Locset wx = G.G[position] & checkInitial;
		while(wx){
            int v;
            TAKEBIT(v, wx);
            if (G.G[31- v] & wx){
                return position;
            }
		}
	}
	return -1;
}

IntervalList PodzialPrzedzialu(Graph G, Interval P)
{
    //Debug helper
    //int cnt = IntervalDivisionCounter;
    //IntervalDivisionCounter++;
    //printf("\nStart %d \t Bottom: %d,\t Top: %d ", cnt, P.bottom, P.top);

	if (CzyKlikaWBottom(G, P.bottom) != -1)
	{
	    //printf("Bottom Bad");
		IntervalList emptyList;
		emptyList.first = NULL;

		return emptyList;
	}

	else
	{

		int wierzcholekDoWyrzucenia = ZnajdzWierzcholekDoWyrzucenia(G, P.top, P.bottom);
		IntervalList ret;
		if (wierzcholekDoWyrzucenia == -1)
		{
			ret.first = malloc(sizeof(IntervalElement));
			ret.first->i.bottom = P.bottom;
			ret.first->i.top = P.top;
			ret.first->next = NULL;
			//printf("OK, Bottom:%d , Top:%d", P.bottom, P.top);
			return ret;
		}

		//printf("Split in two");
		ADDELEMENT1(&P.bottom, 31 - wierzcholekDoWyrzucenia);
		IntervalList p1 = PodzialPrzedzialu(G, P);
		DELELEMENT1(&P.bottom, 31 - wierzcholekDoWyrzucenia);
		DELELEMENT1(&P.top, 31 - wierzcholekDoWyrzucenia);
		IntervalList p2 = PodzialPrzedzialu(G, P);
		//printf("\n Finished %d", cnt);
		p1 = PolaczListy(p1, p2);

		return p1;
	}
}

Locset PobierzTop(Graph G)
{
	int i = 0;
	int ret = 0;
	while (i < G.deg)
	{
		ret = ret | LocbitInterval[i];
		i++;
	}
	return ret;
}

IntervalList ZnajdzPrzedzialy(Graph G)
{
	Locset wszystko = PobierzTop(G);
	Interval P = { 0, wszystko };
	IntervalList l = PodzialPrzedzialu(G, P);
	return l;
}

void testIntervals()
{
    Graph g;
    g.deg = 6;
    g.G = malloc(sizeof(Locset)  * 6);
    g.G[0] = 54;
    g.G[1] = 9;
    g.G[2] = 9;
    g.G[3] = 6;
    g.G[4] = 33;
    g.G[5] = 17;

    IntervalList  i;
    i = ZnajdzPrzedzialy(g);

	    //Wypisanie przedziałów
    IntervalElement * next = i.first;
    while (next != NULL){
        printf("\nB: %d ,T: %d ", (int)next->i.bottom, (int)next->i.top);
        next = next->next;
    }

}
