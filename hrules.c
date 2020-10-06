#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hrules.h"

int h1(Locset vertex, setword *g2){
    Locset result = 0;

    int position=0;
    while(vertex){
        TAKEBIT(position,vertex);

        result = g2[31-position] | result;
    }
    return result;
}

int h2(Locset vertex, int n, setword *g2){
    Locset result=0;
    int pom = (pow(2,n)-1);
        for (int i=0;i<n;i++){
           for(int j=0;j<n;j++){
                int temp=pow(2,i);
                if(i!=j && ((g2[i]|temp)&(pom^vertex))!=(pom^vertex)){
                    result = result | temp;

                }
           }
        }
        return result;
}

int h3_n3(Locset vertex, int i, setword *g2){

	setword wx = g2[i];
    int pos = pow(2,i);
    int mask = pow(2,6)-1;
    wx = (wx | pos)^mask;


    //printf("DD %d\n", wx);

	while(wx) {
		int v;
		TAKEBIT(v, wx);

        pos = pow(2,31-v);
		if(((wx & ~g2[31-v]) & ~vertex)  && !(vertex&pos)) {
        //printf("END 1\n");
			return 1;
		}
	}
	//printf("END 0\n");
	return 0;
}

int h3(Locset vertex, int n, setword *g2){
    Locset result=0;
    for (int i=0;i<n;i++){
        int temp = pow(2,i);
        if(h3_n3(vertex,i,g2)){

            result=result | temp;
        }
    }
    return result;
}
