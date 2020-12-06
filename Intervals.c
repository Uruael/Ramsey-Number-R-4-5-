#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nauty.h"
#include "Intervals.h"
#include "VertexExtension.h"
typedef unsigned int Locset;

const Locset LocbitInterval[] = {
	0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
	0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
	0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
	0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001};

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

int stozkiWPrzedziale(Interval i){
    Locset diffrence = i.top & ~i.bottom;
    int ret = 1;
    while(diffrence){
        int idontcare;
        TAKEBIT(idontcare, diffrence);
        ret *=2;
    }
    return ret;
}

DisallowedStructs GetK3List(Graph F) {
	DisallowedStructs ret;
	ret.n = malloc(sizeof(DisallowedStruct) * 2048);
	Locset all_n = ALLMASK(F.deg);

	ret.number = 0;
	for (int i = 0; i < F.deg; i++) {
		Locset row = F.G[i];

		for (int j = i + 1; j < F.deg; j++) {
            if (F.G[i] & LocbitInterval[j]){
                Locset rowmask = ALLMASK(j+1);
                rowmask = all_n & ~rowmask;
                Locset k3cases = row & F.G[j] & rowmask;
                while (k3cases) {
                    int index;
                    TAKEBIT(index, k3cases);
                    AddDisallowed(&ret, clique, i, j, index, 0);
                    //printf("Found! Vert. %d,%d,%d\n",i,j,index);
                }
            }
		}
	}
	//printf("Disallowed: %d\n",ret.number);
	return ret;
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

IntervalList TworzI(Graph F){
	DisallowedStructs disallowed;
	disallowed = GetK3List(F);

	IntervalList ListCurrent;
	IntervalList ListNext;
	IntervalElement *e = malloc(sizeof(IntervalElement));
	Interval i;
	i.bottom = 0;
	i.top = PobierzTop(F);
	e->next = NULL;
	e->i = i;
	ListCurrent.first = e;
	ListNext.first = NULL;

	for (int i = 0; i < disallowed.number; i++){
		if ((disallowed.n)[i].type == clique){

			IntervalElement* currentI = ListCurrent.first;

			while (currentI != NULL){
				if (!isAllInLocset(currentI->i.top, disallowed.n[i])) {
                    AppendToFront(currentI, &ListNext);
				}
				else if (isAllInLocset(currentI->i.bottom, disallowed.n[i])){
				}
				else{
					CutToThree (currentI->i, disallowed.n[i], &ListNext);
				}
                IntervalElement* freedis = currentI;
                currentI = currentI->next;
                free(freedis);
			}
		}

        ListCurrent.first = ListNext.first;
        ListNext.first = NULL;
	}

	return ListCurrent;
}


IntervalListUndirected Undirect(IntervalList lista)
{
    IntervalListUndirected ret;

    IntervalElementBackwards*e1 = malloc(sizeof(IntervalElementBackwards));
    IntervalElementBackwards*previous = NULL;
    IntervalElement*next = lista.first;
    IntervalElementBackwards*e = e1;
    while (next != NULL){

                e->i = next->i;
                e->previous = previous;
                if(previous!=NULL)
                    previous->next = e;

                previous = e;
                next = next->next;
                e = malloc(sizeof(IntervalElementBackwards));
            }
    ret.first = e1;
    return ret;
}

IntervalList Direct(IntervalListUndirected lista)
{
    IntervalList ret;

    IntervalElement*e1 = malloc(sizeof(IntervalElement));
    IntervalElementBackwards*next = lista.first;
    IntervalElement*e = e1;
    IntervalList*pom;
    while (next != NULL){
        e->i = next->i;
        e->next = next->next;

        next = next->next;
        pom = malloc(sizeof(IntervalElement));
        e->next = pom;
        e = pom;
        }

    ret.first = e1;
    return ret;
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
    /*
    Graph g;
    g.deg = 6;
    g.G = malloc(sizeof(Locset)  * 6);
    g.G[0] = IntToLocset(50);
    g.G[1] = IntToLocset(13);
    g.G[2] = IntToLocset(10);
    g.G[3] = IntToLocset(22);
    g.G[4] = IntToLocset(41);
    g.G[5] = IntToLocset(17);//More than one correct solution!!
    */


    Graph g;
    g.deg = 4;
    //g.G = malloc(sizeof(Locset)  * 4);
    g.G[0] = IntToLocset(14);
    g.G[1] = IntToLocset(13);
    g.G[2] = IntToLocset(3);
    g.G[3] = IntToLocset(3);//More than one correct solution!!

    for(int i =0; i<g.deg; i++){
        WriteLocsetAsBits(g.G[i]);
        printf("\n");

    }

    IntervalList  j;
    j = TworzI(g);

	    //Wypisanie przedziałów
    IntervalElement * next = j.first;
    while (next != NULL){

        printf("\nInterval\n");
        WriteLocsetAsBits(next->i.bottom);
        printf("\n");
        WriteLocsetAsBits(next->i.top);
        printf("\n");

        next = next->next;
    }

}

TwoIntervals RozdzielPrzedzial(Interval todivide, int vertex){
    TwoIntervals ret;
    ret.first.bottom = todivide.bottom | LocbitInterval[vertex];
    ret.first.top = todivide.top;
    ret.second.bottom = todivide.bottom;
    ret.second.top = todivide.top ^ LocbitInterval[vertex];
    return ret;
}

TwoIntervals PodzielPrzedzial(Interval toobig){
    Locset diffrence = toobig.bottom ^ toobig.top;
    int vertex;
    TAKEBIT(vertex, diffrence);
    return RozdzielPrzedzial(toobig, vertex);
}

void AppendToFront(IntervalElement *i, IntervalList *l){
    IntervalElement *toadd = malloc(sizeof(IntervalElement));
    toadd->i.bottom = i->i.bottom;
    toadd->i.top = i->i.top;
    toadd->next = l->first;
    l->first = toadd;
    return;
}
/*
IntervalList testDivIntervals(Interval starter)
{
    if (starter.bottom != starter.top){
        TwoIntervals two = PodzielPrzedzial(starter);
        IntervalList i = testDivIntervals(two.first);
        IntervalList j = testDivIntervals(two.second);
        return PolaczListy(i,j);
    }
    else
    {
        IntervalElement *e = malloc(sizeof(IntervalElement));
        e->i.bottom = starter.bottom;
        e->i.top = starter.top;
        e->next = NULL;
        IntervalList m;
        m.first = e;
        return m;
    }
}

void testPodzielPrzedzial()
{
    IntervalElement starter;
    starter.i.bottom = IntToLocset(17);
    starter.i.top = IntToLocset(2047);
    printf("StartingWith Interval\n");
    WriteLocsetAsBits(starter.i.bottom);
    printf("\n");
    WriteLocsetAsBits(starter.i.top);
    printf("\n");printf("\n");

    IntervalList a = testDivIntervals(starter.i);

    IntervalElement *e = a.first;
    while(e != NULL){
        //WriteLocsetAsBits(e->i.bottom);
        //printf("\n");
        //WriteLocsetAsBits(e->i.top);
        //printf("\n");
        IntervalElement *compareto = e->next;
        while(compareto != NULL){
            if (e->i.bottom == compareto->i.bottom ){
                printf("Duplikat Bottom");
            }

            if (e->i.top == compareto->i.top ){
                printf("Duplikat Top");
            }
            compareto = compareto->next;
        }
        e = e->next;
    }
}
*/
Interval Polacz(IntervalListUndirected lista, IntervalElementBackwards*i, IntervalElementBackwards*j){
    Interval ret;
    ret.bottom = i->i.bottom;
    ret.top = j->i.top;

    IntervalElementBackwards*e = malloc(sizeof(IntervalElement));
    e->i = ret;

    i->previous->next = e;
    i->next->previous = e;

    e->previous = i->previous;
    e->next = i->next;
    if(j->previous != NULL)
    j->previous->next = j->next;
    j->next->previous = j-> previous;

    return ret;
}

int CzyMoznaPolaczyc(Interval i, Interval j){

    const Locset LocbitInterval[] = {
    0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
    0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
    0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
    0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001};

    if (stozkiWPrzedziale(i) == stozkiWPrzedziale(j)){

        Locset d1 = j.bottom & ~i.bottom;
        Locset d2 = j.top & ~i.top;
        //Locset d3 = j.top & ~j.bottom;
        if (d1 == d2){
            int counter = 0;
            int diffrentBit;
            while(d1){
                TAKEBIT(diffrentBit, d1);
                counter++;
            }
            if (counter == 1){
                if ((j.bottom & ~LocbitInterval[diffrentBit]) == i.bottom){
                    if ((i.top | LocbitInterval[diffrentBit]) == j.top){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
