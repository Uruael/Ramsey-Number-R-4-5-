#include "VertexExtension.h"

const Locset LocbitVE[] = {
	0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
	0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
	0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
	0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001
};


IntervalList VertexExtension(Graph F){
	DisallowedStructs disallowed;
	disallowed = GetDisallowedGroups(F); //Pobierz liste zbiorów niedozwolonych

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
				if (!isAllInLocset(currentI->i.top, disallowed.n[i])) { //Przedzial OK
                    AppendToFront(currentI, &ListNext);
				}
				else if (isAllInLocset(currentI->i.bottom, disallowed.n[i])){ //Przedzial do usuniecia
				}
				else{//Przedzial do podzialu
					CutToThree (currentI->i, disallowed.n[i], &ListNext);
				}
                IntervalElement* freedis = currentI;
                currentI = currentI->next;
                free(freedis);
			}
		}

		else //disallowed.n[i].type == indSet
		{

			IntervalElement* currentI = ListCurrent.first;

			while (currentI != NULL){
				if (!isNoneinLocset(currentI->i.bottom, disallowed.n[i])) {//Przedzial OK
                    AppendToFront(currentI, &ListNext);
				}
				else if (isNoneinLocset(currentI->i.top, disallowed.n[i])){//Przedzial do usuniecia
				}
				else{//Przedzial do podzialu
					CutToFour(currentI->i, disallowed.n[i], &ListNext);
				}
				IntervalElement* freeThis = currentI;
                    currentI = currentI->next;
                    free(freeThis);
			}
		}

        ListCurrent.first = ListNext.first;
        ListNext.first = NULL;
	}
    /*
    printf("Disallowed: %d\n",disallowed.number);
    IntervalElement *test = ListCurrent.first;
    while (test != NULL){
        printf("Extension\n");
        printf("Bottom: \t");
        WriteLocsetAsBits(test->i.bottom);
        printf("\nTop:    \t");
        WriteLocsetAsBits(test->i.top);

        printf("\n");
        test = test->next;
    }
     */
	return ListCurrent;
}


DisallowedStructs GetDisallowedGroups(Graph F) {
	DisallowedStructs ret;
	ret.n = malloc(sizeof(DisallowedStruct) * 2048);
	Locset all_n = ALLMASK(F.deg);

	ret.number = 0;
	for (int i = 0; i < F.deg; i++) {
		Locset row = F.G[i];

		for (int j = i + 1; j < F.deg; j++) {
            if (F.G[i] & LocbitVE[j]){ // k2
                Locset rowmask = ALLMASK(j+1);
                rowmask = all_n & ~rowmask;
                Locset k3cases = row & F.G[j] & rowmask;
                while (k3cases) {//k3
                    int index;
                    TAKEBIT(index, k3cases);
                    AddDisallowed(&ret, clique, i, j, index, 0);
                    //printf("Found! Vert. %d,%d,%d\n",i,j,index);
                }
            }
            else{ //n2
                Locset row3mask = ALLMASK(j+1);
                row3mask = all_n & ~row3mask;
                Locset n3cases = ~row & ~F.G[j] & row3mask;
                while (n3cases) {//n3
                    int index3;
                    TAKEBIT(index3, n3cases);
                    Locset row4mask = ALLMASK(index3+1);
                    row4mask = all_n & ~row4mask;
                    Locset n4cases = ~row & ~F.G[j] & ~F.G[index3] & row4mask;
                    while (n4cases) {//n4
                        int index4;
                        TAKEBIT(index4, n4cases);
                        //printf("Found! Vert. %d,%d,%d,%d\n",i,j,index3,index4);
                        AddDisallowed(&ret, indSet, i, j, index3, index4);
                    }
                }
            }
		}
	}
	//printf("Disallowed: %d\n",ret.number);
	return ret;
}

void AddDisallowed(DisallowedStructs *ret, enum DisallowedStructType type, int v1, int v2, int v3, int v4) {
	int addAt = ret->number;
	DisallowedStruct *a = &(ret->n[addAt]);
	a->type = type;
	a->vert1 = v1;
	a->vert2 = v2;
	a->vert3 = v3;
	a->vert4 = v4;

	ret->number++;
}

int isAllInLocset(Locset l, DisallowedStruct set) {
	Locset i1 = LocbitVE[set.vert1];
	Locset i2 = LocbitVE[set.vert2];
	Locset i3 = LocbitVE[set.vert3];
	Locset compare = i1 | i2 | i3;

	if ((l & compare) == compare) {
		return 1;
	}
	return 0;
}

int isNoneinLocset(Locset l, DisallowedStruct set) {
	Locset i1 = LocbitVE[set.vert1];
	Locset i2 = LocbitVE[set.vert2];
	Locset i3 = LocbitVE[set.vert3];
	Locset i4 = LocbitVE[set.vert4];
	Locset compare = i1 | i2 | i3 | i4;

	if (((~l) & compare) == compare) {
		return 1;
	}
	return 0;
}

void CutToThree(Interval currentI, DisallowedStruct dir, IntervalList *target) {
	Locset i1 = LocbitVE[dir.vert1];
	Locset i2 = LocbitVE[dir.vert2];
	Locset i3 = LocbitVE[dir.vert3];
	Locset compare = i1 |i2 | i3;
	Locset addtoBottom = 0;
    IntervalElement *temp;
	compare = compare & ~currentI.bottom;

	while (compare) {
		int index;
		TAKEBIT(index, compare);
		temp = malloc(sizeof(IntervalElement));
		temp->i.bottom = currentI.bottom | addtoBottom;
		temp->i.top = currentI.top & ~LocbitVE[index];
		temp->next = NULL;

		addtoBottom |= LocbitVE[index];
        AppendToFront(temp, target);
	}
}

void CutToFour(Interval currentI, DisallowedStruct dir, IntervalList *target) {
	Locset i1 = LocbitVE[dir.vert1];
	Locset i2 = LocbitVE[dir.vert2];
	Locset i3 = LocbitVE[dir.vert3];
	Locset i4 = LocbitVE[dir.vert4];
	Locset compare = i1 | i2 | i3 | i4;
	Locset removeFromTop = 0;

	IntervalElement* temp;

	compare = compare & currentI.top;
	while (compare) {
		int index;
		TAKEBIT(index, compare);

		temp = malloc(sizeof(IntervalElement));
		temp->i.bottom = currentI.bottom | LocbitVE[index];
		temp->i.top = currentI.top & ~removeFromTop;
		temp->next = NULL;

		removeFromTop |= LocbitVE[index];
		AppendToFront(temp, target);

	}
}
/*
void VertexExtensionTest(){
    Graph f;
    f.G = malloc(12 * sizeof(Locset));
    f.G[0] = IntToLocset(6);
    f.G[1] = IntToLocset(5);
    f.G[2] = IntToLocset(3);
    f.G[3] = IntToLocset(48);
    f.G[4] = IntToLocset(40);
    f.G[5] = IntToLocset(24);
    f.G[6] = IntToLocset(384);
    f.G[7] = IntToLocset(320);
    f.G[8] = IntToLocset(192);
    f.G[9] = IntToLocset(3072);
    f.G[10] = IntToLocset(2560);
    f.G[11] = IntToLocset(1536);
    f.deg = 12;

    for(int i = 0; i<f.deg; i++){
        WriteLocsetAsBits(f.G[i]);
        printf("\n");

    }
    printf("\n");

    VertexExtension(f);


    printf("\n\n\nTest2\n\n");
    Graph test2;
    test2.G = malloc(4 * sizeof(Locset));
    test2.deg = 3;
    test2.G[0] = IntToLocset(6);
    test2.G[1] = IntToLocset(5);
    test2.G[2] = IntToLocset(3);
    for(int i =0; i<test2.deg; i++){
        WriteLocsetAsBits(test2.G[i]);
        printf("\n");

    }
    printf("\n");
    VertexExtension(test2);


    printf("\n\n\nTest3\n\n");
    Graph test3;
    test3.G = malloc(4 * sizeof(Locset));
    test3.deg = 4;
    test3.G[0] = IntToLocset(0);
    test3.G[1] = IntToLocset(0);
    test3.G[2] = IntToLocset(0);
    test3.G[3] = IntToLocset(0);
    for(int i =0; i<test3.deg; i++){
        WriteLocsetAsBits(test3.G[i]);
        printf("\n");

    }
    printf("\n");
    VertexExtension(test3);

    printf("\n\n\nTest4\n\n");
    Graph t4;
    t4.deg = 6;
    t4.G = malloc(sizeof(Locset)  * 6);
    t4.G[0] = IntToLocset(50);
    t4.G[1] = IntToLocset(13);
    t4.G[2] = IntToLocset(10);
    t4.G[3] = IntToLocset(22);
    t4.G[4] = IntToLocset(41);
    t4.G[5] = IntToLocset(17);
    for(int i =0; i<t4.deg; i++){
        WriteLocsetAsBits(t4.G[i]);
        printf("\n");

    }
    printf("\n");
    VertexExtension(t4);

    printf("\n\n\nTest5\n\n");
    Graph t5;
    t5.deg = 5;
    t5.G = malloc(sizeof(Locset)  * 5);
    t5.G[0] = IntToLocset(0);
    t5.G[1] = IntToLocset(0);
    t5.G[2] = IntToLocset(0);
    t5.G[3] = IntToLocset(0);
    t5.G[4] = IntToLocset(0);
    for(int i =0; i<t5.deg; i++){
        WriteLocsetAsBits(t5.G[i]);
        printf("\n");

    }
    printf("\n");
    VertexExtension(t5);

}
*/
void WriteLocsetAsBits(Locset input){
    unsigned int temp = 0;
    for(int i =0; i<31; i++){
        temp += input%2;
        input /=2;
        temp = temp <<1;
    }
    temp += input%2;
    for(int i =0; i<31; i++){
            printf("%d", temp%2);
            temp = temp /2;
    }
    printf("%d", temp%2);
}

Locset MaskNBits(int n){
    Locset ret = 0;
    for (int i =0; i< n; i++){
        ret |= LocbitVE[i];
    }
}


