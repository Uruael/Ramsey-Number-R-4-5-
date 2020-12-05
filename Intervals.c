#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nauty.h"
#include "Intervals.h"
typedef unsigned int Locset;

int IntervalDivisionCounter = 0;

const Locset LocbitInterval[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000};

unsigned int IntToLocset(int input){
    Locset out = 0;
    for(int i =0; i<31; i++){
        out += input%2;
        input /=2;
        out = out <<1;
    }
    out += input%2;

    return out;
}


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
        Locset wx = G.G[position] & check;
		while(wx){
            int v;
            TAKEBIT(v, wx);
            if (G.G[v] & wx){

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
        Locset wx = G.G[position] & (checkInitial );
		while(wx){
            int v;
            TAKEBIT(v, wx);
            if (G.G[v] & wx & checkInitial){
                return position;
            }
		}
	}
	return -1;
}

IntervalList PodzialPrzedzialu(Graph G, Interval P)
{
    //Debug helper
	/*
    int cnt = IntervalDivisionCounter;
    IntervalDivisionCounter++;
    //printf("\nStart %d \t Bottom: %d,\t Top: %d ", cnt, P.bottom, P.top);
    printf("\nStart %d, Bottom:", cnt );
			WriteLocsetAsBits(P.bottom);
            printf(", Top:");
            WriteLocsetAsBits(P.top);
            printf(" ");
	*/
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
			//printf("OK");
			return ret;
		}

		//printf("Split in two");
		ADDELEMENT1(&P.bottom, wierzcholekDoWyrzucenia); //
		IntervalList p1 = PodzialPrzedzialu(G, P);
		DELELEMENT1(&P.bottom, wierzcholekDoWyrzucenia);//
		DELELEMENT1(&P.top, wierzcholekDoWyrzucenia);//
		IntervalList p2 = PodzialPrzedzialu(G, P);
		//printf("\n Finished %d", cnt);
		p1 = PolaczListy(p1, p2);

		return p1;
	}
}

TwoIntervals RozdzielPrzedzial(Interval todivide, int vertex){
    TwoIntervals ret;
    ret.first.bottom = todivide.bottom | LocbitInterval[31 - vertex];
    ret.first.top = todivide.top;
    ret.second.bottom = todivide.bottom;
    ret.second.top = todivide.top ^ LocbitInterval[31 - vertex];
    return ret;
}


TwoIntervals PodzielPrzedzial(Interval toobig){
    Locset diffrence = toobig.bottom ^ toobig.top;
    int vertex;
    TAKEBIT(vertex, diffrence);
    return RozdzielPrzedzial(toobig, vertex);
}

Locset PobierzTop(Graph G)
{
	int i = 0;
	int ret = 0;
	while (i < G.deg)
	{
		ret = ret | LocbitInterval[31-i];
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
    /*
    Graph g;
    g.deg = 6;
    g.G = malloc(sizeof(Locset)  * 6);
    g.G[0] = IntToLocset(54);
    g.G[1] = IntToLocset(9);
    g.G[2] = IntToLocset(9);
    g.G[3] = IntToLocset(6);
    g.G[4] = IntToLocset(33);
    g.G[5] = IntToLocset(17);//More than one correct solution!!
    */
        Graph g;
    g.deg = 6;
//    g.G = malloc(sizeof(Locset)  * 6);
    g.G[0] = IntToLocset(50);
    g.G[1] = IntToLocset(13);
    g.G[2] = IntToLocset(10);
    g.G[3] = IntToLocset(22);
    g.G[4] = IntToLocset(41);
    g.G[5] = IntToLocset(17);//More than one correct solution!!


    /*
    Graph g;
    g.deg = 4;
    //g.G = malloc(sizeof(Locset)  * 4);
    g.G[0] = IntToLocset(14);
    g.G[1] = IntToLocset(13);
    g.G[2] = IntToLocset(3);
    g.G[3] = IntToLocset(3);//More than one correct solution!!
    */
    /*for(int i =0; i<g.deg; i++){
        WriteLocsetAsBits(g.G[i]);
        printf("\n");

    }
    IntervalList  i;
    i = ZnajdzPrzedzialy(g);

	    //Wypisanie przedziałów
    IntervalElement * next = i.first;
    while (next != NULL){

        printf("\nInterval\n");
        WriteLocsetAsBits(next->i.bottom);
        printf("\n");
        WriteLocsetAsBits(next->i.top);
        printf("\n");

        next = next->next;
    }*/
}

