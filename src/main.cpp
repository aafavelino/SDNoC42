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

	sc_start();


  	return 0;
}