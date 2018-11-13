//
// Created by Adelino, Raul and Yuri on 13/11/18.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <queue>
#include <ctime>    
#include <stdlib.h>  
#include <time.h>   
#include <systemc.h>
// #include "noc.h"
#include "sd.h"

using namespace std;

int sc_main (int argc, char* argv[]) {
	// Leitura do Arquivo de tráfego
	FILE *traffic;
	int **padrao_tfg;
	int size_pct = 0;

	traffic = fopen("traffic.txt","r");
	fscanf(traffic,"tg %i",&size_pct);
	padrao_tfg = (int**)malloc(size_pct * sizeof(int*));

	for (int i = 0; i < size_pct; i++) {
    	padrao_tfg[i] = (int*) malloc(7 * sizeof(int));
    	fscanf(traffic,"%i %i %i %i %i %i %i", &padrao_tfg[i][0], &padrao_tfg[i][1], &padrao_tfg[i][2], &padrao_tfg[i][3], &padrao_tfg[i][4], &padrao_tfg[i][5], &padrao_tfg[i][6]);
   	}

   	fclose(traffic);
    

  	return 0;
}