#include <stdio.h>
#include <stdlib.h>
#include "nauty.h"

typedef unsigned int Locset;

const Locset Locbit[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
};

const int Rightbit[] = {
    8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
};

#define LOCFIRSTBIT(x) ((x)&0xFFFF ? ((x)&0xFF ? 0+Rightbit[(x)&0xFF] : 8+Rightbit[((x)>>8)&0xFF])\
                     : ((x)&0xFF0000 ? 16+Rightbit[((x)>>16)&0xFF] : 24+Rightbit[((x)>>24)&0xFF]))
#define LOCTAKEBIT(iw,w) {(iw) = LOCFIRSTBIT(w); (w) ^= Locbit[iw];}

#define TIDYUP w = cones[i]; while(w) {LOCTAKEBIT(j, w);DELELEMENT1(&g[j],n);} g[n] = 0; first = -1;second = -1;good = 0;


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

int vertexconnect(int a, int b){

        Locset temp = g[a];
        temp = temp >> (31-b);
        if(temp & 1)
            return 1;
        else
            return 0;
}

static void
userautomproc(int count, int *p, int *orbits,
          int numorbits, int stabvertex, int n)
{
    int i;
    for(i=0; i<n; i++)
        perm[perm_len][i] = p[i];
    perm_len++;
}

int cnt_graph=0;


int gen(int n) {
    int i, j, j1, j2, p, c, cx;
    Locset *cones = level_cones[n];
    int *orbits = level_orbits[n];
    int llen = level_len[n];
    setword *conesNext = cones+llen;
    int *orbitsNext = orbits+llen;

    for(i=0; i<llen; i++) {
        if(orbits[i]!=i) continue;

		//Wygeneruj nowy graf
        g[n] = 0;
        Locset w = cones[i];
        while(w) {
            LOCTAKEBIT(j, w);
            ADDELEMENT1(&g[j],n);
            ADDELEMENT1(&g[n],j);
        }
		//SprawdŸ, czy graf spe³nia warunki
		//Je¿eli nie, to siê nie nadaje

        //Sprawdź (3,N)
        int good = 1; //Je¿eli nie znajdziemy kliki, to jest ok
        for (int first = n-1; first>=0; first--){
            int check1 = vertexconnect(first,n);// weŸ krawêdŸ pomiêdzy pierwszym a najnowszym
            if (check1 == 0) continue; // Sprawdzamy tylko klikę
            for (int second = first - 1; second>=0; second--){
                int check2 = vertexconnect(second,n);
                int check3 = vertexconnect(first,second);
                int check = (check1 == check2 && check1 == check3); // je¿eli wszystkie wierzcho³ki takie same
                if (check == 1){ //ZnaleŸliœmy cos negujace (3,N)
                    TIDYUP
                    }
                }
            }
        if (!good) continue;

        //Sprawdź (N,5)
        good = 1; //Je¿eli nie znajdziemy kliki, to jest ok
        for (int first = n-1; first>=0; first--){
            int check1 = vertexconnect(first,n);// weŸ krawêdŸ pomiêdzy pierwszym a najnowszym
            if (check1 == 1) continue; // Sprawdzamy tylko zbiór niezależny
            for (int second = first - 1; second>=0; second--){
                int check2 = vertexconnect(second,n);
                int check3 = vertexconnect(first,second);
                int check = (check1 == check2 && check1 == check3); // je¿eli wszystkie wierzcho³ki takie same
                if (check == 1){ //ZnaleŸliśmy cos negujace (N,3)
                    for (int third = second - 1; third>=0; third--){
                        int check2 = vertexconnect(third,n);
                        int check3 = vertexconnect(first,third);
                        int check4 = vertexconnect(second,third);
                        int check = (check1 == check2 && check1 == check3 && check1 == check4);
                        if (check == 1){ //ZnaleŸliśmy cos negujace (N,4)
                            for (int fourth = third - 1; fourth>=0; fourth--){
                                int check2 = vertexconnect(fourth,n);
                                int check3 = vertexconnect(first,fourth);
                                int check4 = vertexconnect(second,fourth);
                                int check5 = vertexconnect(third,fourth);
                                int check = (check1 == check2 && check1 == check3 && check1 == check4 && check1 == check5);
                                if (check == 1) {
                                    fourth = -1;
                                    third = -1;
                                    TIDYUP
                                }
                            }
                        }
                    }

                }
            }
        }
        if (!good) continue;


		//Zrób gupe automorfizmow oraz forme kanoniczna
        perm_len = 0;
        nau_options.getcanon = TRUE;
        nau_options.userautomproc = userautomproc;
        nauty(g, nau_lab, nau_ptn, NULL, nau_orbits, &nau_options,
                &nau_stats, nau_workspace, 50, 1, n+1, gcan);

		// Je¿eli nasz wieszcho³ek jest w ostatniej orbicie to nale¿y go dodaæ
        if(nau_orbits[nau_lab[n]] == nau_orbits[n]) {
            if(n+1<max_n) {
                int llenNext = 1<<(n+1);
                for(j=0; j<llenNext; j++) {
                    conesNext[j] = j; //permutacje, prawe j to zbiór
                    orbitsNext[j] = j; //Las Zbiorów Roz³¹czych
                }
                 for(p=0; p<perm_len; p++) {
                    int *locperm = perm[p]; //locperm to obecna permutacja

					//Budowa podzbioru wierzcho³ków które siê pozamieniaj¹
                    Locset moved = 0;
                    for(j = 0; j <= n; ++j)
                        if (locperm[j] != j) moved |= Locbit[j];

					//
                    for(c = 0; c<llenNext; c++) {
                        if ((w = conesNext[c] & moved) == 0) continue;
                        cx = conesNext[c] & ~moved;
                        while(w) {
                            LOCTAKEBIT(j1, w);
                            cx |= Locbit[locperm[j1]];
                        }

						//Las Zbiorów Roz³¹czych
                        j1 = orbitsNext[c];
                        while(orbitsNext[j1]!=j1) j1 = orbitsNext[j1];
                        j2 = orbitsNext[cx];
                        while(orbitsNext[j2]!=j2) j2 = orbitsNext[j2];
                        if (j1 < j2) orbitsNext[j2] = orbitsNext[c] = orbitsNext[cx] = j1;
                        else if (j1 > j2) orbitsNext[j1] = orbitsNext[c] = orbitsNext[cx] = j2;
                    }
                }
                level_cones[n+1] = conesNext;
                level_orbits[n+1] = orbitsNext;
                level_len[n+1] = llenNext;
                gen(n+1);
            } else {
                cnt_graph++;
            }
        }

		//napraw po tym przejœciu pêtli
        w = cones[i];
        while(w) {
            LOCTAKEBIT(j, w);
            DELELEMENT1(&g[j],n);
        }
        g[n] = 0;
    }
}

int main(int argc, char *argv[]) {
    if(argc<1) {
        fprintf(stderr, "Za malo zmiennych\n");
        return 1;
    }
    //max_n = atoi(argv[1]);
    max_n =  10; // Test Here
    level_cones[0] = malloc((1<<max_n)*sizeof(setword));
    level_orbits[0] = malloc((1<<max_n)*sizeof(int));
    if(level_cones[0]==NULL || level_orbits[0]==NULL) {
        printf("za malo pamieci");
        return 1;
    }
    level_cones[1] = level_cones[0];
    level_cones[1][0] = 0;
    level_cones[1][1] = 1;
    level_orbits[1] = level_orbits[0];
    level_orbits[1][0] = 0;
    level_orbits[1][1] = 1;
    level_len[1] = 2;

    gen(1);

    free(level_cones[0]);
    free(level_orbits[0]);
    printf("%d\n", cnt_graph);
    return 0;
}
