#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hrules.h"
#include "nauty.h"
#include "rules.h"
#include "Intervals.h"
#include "Gluing.c"


typedef unsigned int Locset;

const Locset Locbit[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000};

const int Rightbit[] = {
    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

#define LOCFIRSTBIT(x) ((x)&0xFFFF ? ((x)&0xFF ? 0 + Rightbit[(x)&0xFF] : 8 + Rightbit[((x) >> 8) & 0xFF]) \
                                   : ((x)&0xFF0000 ? 16 + Rightbit[((x) >> 16) & 0xFF] : 24 + Rightbit[((x) >> 24) & 0xFF]))
#define LOCTAKEBIT(iw, w)      \
    {                          \
        (iw) = LOCFIRSTBIT(w); \
        (w) ^= Locbit[iw];     \
    }

int max_n;
setword *level_cones[20];
int *level_orbits[20];
int level_len[20];
setword g[WORDSIZE], gcan[WORDSIZE];

int perm[WORDSIZE][WORDSIZE]; //generatory grupy automorfizmow
int perm_len;

// dla nauty
int nau_lab[MAXN], nau_ptn[MAXN], nau_orbits[MAXN];
static DEFAULTOPTIONS_GRAPH(nau_options);
statsblk nau_stats;
setword nau_workspace[50];

static void
userautomproc(int count, int *p, int *orbits,
              int numorbits, int stabvertex, int n)
{
    int i;
    for (i = 0; i < n; i++)
        perm[perm_len][i] = p[i];
    perm_len++;
}

int cnt_graph = 0;

int gen(int n)
{
    int i, j, j1, j2, p, c, cx;
    Locset *cones = level_cones[n];
    int *orbits = level_orbits[n];
    int llen = level_len[n];
    setword *conesNext = cones + llen;
    int *orbitsNext = orbits + llen;

    for (i = 0; i < llen; i++)
    {
        if (orbits[i] != i)
            continue;
        g[n] = 0;
        Locset w = cones[i];
        while (w)
        {
            LOCTAKEBIT(j, w);
            ADDELEMENT1(&g[j], n);
            ADDELEMENT1(&g[n], j);
        }

        perm_len = 0;
        nau_options.getcanon = TRUE;
        nau_options.userautomproc = userautomproc;
        nauty(g, nau_lab, nau_ptn, NULL, nau_orbits, &nau_options,
              &nau_stats, nau_workspace, 50, 1, n + 1, gcan);

        if (nau_orbits[nau_lab[n]] == nau_orbits[n])
        {
            if (n + 1 < max_n)
            {
                int llenNext = 1 << (n + 1);
                for (j = 0; j < llenNext; j++)
                {
                    conesNext[j] = j;
                    orbitsNext[j] = j;
                }
                for (p = 0; p < perm_len; p++)
                {
                    int *locperm = perm[p];
                    Locset moved = 0;
                    for (j = 0; j <= n; ++j)
                        if (locperm[j] != j)
                            moved |= Locbit[j];
                    for (c = 0; c < llenNext; c++)
                    {
                        if ((w = conesNext[c] & moved) == 0)
                            continue;
                        cx = conesNext[c] & ~moved;
                        while (w)
                        {
                            LOCTAKEBIT(j1, w);
                            cx |= Locbit[locperm[j1]];
                        }
                        j1 = orbitsNext[c];
                        while (orbitsNext[j1] != j1)
                            j1 = orbitsNext[j1];
                        j2 = orbitsNext[cx];
                        while (orbitsNext[j2] != j2)
                            j2 = orbitsNext[j2];
                        if (j1 < j2)
                            orbitsNext[j2] = orbitsNext[c] = orbitsNext[cx] = j1;
                        else if (j1 > j2)
                            orbitsNext[j1] = orbitsNext[c] = orbitsNext[cx] = j2;
                    }
                }
                level_cones[n + 1] = conesNext;
                level_orbits[n + 1] = orbitsNext;
                level_len[n + 1] = llenNext;
                gen(n + 1);
            }
            else
            {
                cnt_graph++;
            }
        }

        w = cones[i];
        while (w)
        {
            LOCTAKEBIT(j, w);
            DELELEMENT1(&g[j], n);
        }
        g[n] = 0;
    }
}

int main(int argc, char *argv[])
{
    FILE *graphs1 = fopen("g358.bin", "r");
    FILE *graphs2 = fopen("g4416.bin", "r");

    Graphs G;
    G.length=179;
    G.graphs = malloc(sizeof(Graph)*G.length);

    for(int j=0;j<G.length;j++){
            //G.graphs[j].G = malloc(sizeof(Locset)*WORDSIZE);
            fread (G.graphs[j].G,sizeof(setword)*WORDSIZE,1,graphs1);
            G.graphs[j].deg=8;
        }

    Graphs H;
    H.length=2;
    H.graphs = malloc(sizeof(Graph)*G.length);

    for(int j=0;j<H.length;j++){
            //H.graphs[j].G = (Locset*)malloc(sizeof(Locset)*WORDSIZE);
            fread (H.graphs[j].G,sizeof(setword)*WORDSIZE,1,graphs2);
            H.graphs[j].deg=16;
        }



    FILE *f = fopen("graph.bin", "w");
    fclose(f);

    //Glue(G, H);


    Graph X, Y;

    X.deg = 4;
    X.G[0] = 0;
    X.G[1] = 0;
    X.G[2] = 0;
    X.G[3] = 0;

    Y.deg = 2;
    Y.G[0] = (1 << 30);
    Y.G[1] = (1 << 31);

    struct IntervalList intervals = ZnajdzPrzedzialy(Y);
    Interval* chosenIntervals = malloc(sizeof(Interval)*7);
    int index = 0;
    IntervalElement * next = intervals.first;
    while (next != NULL){
        printf("%d", next->i.bottom);
        permuteIntervals(X, Y, intervals, chosenIntervals, 0, index++);
        next = next->next;
    }

    return 0;
}
