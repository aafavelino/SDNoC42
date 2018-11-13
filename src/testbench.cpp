//
// Created by Adelino, Raul and Yuri on 02/11/18.
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
	// TESTE FECHAMENTO CIRCUITO
	sd *software_defined = new sd("SD");

	software_defined->solicita_rota();

	software_defined->cores[0][0].write(6);
	software_defined->cores[0][1].write(8);

	sc_start();

	cout << software_defined->noc42->network[0][0]->buffer_local->data.read() << endl;
	cout << software_defined->noc42->network[0][0]->mux_leste->entrada_1.read() << endl;
	cout << software_defined->noc42->network[0][1]->buffer_oeste->data.read() << endl;
	cout << software_defined->noc42->network[0][1]->mux_local->saida << endl;

	cout << software_defined->noc42->network[0][1]->buffer_local->data.read() << endl;
	cout << software_defined->noc42->network[0][1]->mux_oeste->entrada_1.read() << endl;
	cout << software_defined->noc42->network[0][0]->buffer_leste->data.read() << endl;
	cout << software_defined->noc42->network[1][0]->buffer_norte->data.read() << endl;
	cout << software_defined->noc42->network[1][0]->mux_local->saida << endl;

  	return 0;
}