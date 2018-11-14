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
#include "sd.h"

using namespace std;

int sc_main (int argc, char* argv[]) {

	// Instância do software defined
	sd *software_defined = new sd("SD");
	// Geração de sinal de clock para métodos sensíveis a mudança de clock
	sc_clock clock("clock", 1, SC_NS, 1, 1, SC_NS);
	software_defined->Clk(clock);


	// Leitura do Arquivo de tráfego
	FILE *traffic;
	int **padrao_tfg;
	int size_pct = 0;

	traffic = fopen("traffic.txt","r");
	fscanf(traffic,"tg %i",&size_pct);
	padrao_tfg = (int**)malloc(size_pct * sizeof(int*));

	/**
	 * Atribuindo os fluxos de tráfego ao padrão_tfg onde essa matriz será utilizada para 
	 * construir os pacotes da simulação.
	 */
	for (int i = 0; i < size_pct; i++) {
    	padrao_tfg[i] = (int*) malloc(7 * sizeof(int));
    	fscanf(traffic,"%i %i %i %i %i %i %i", &padrao_tfg[i][0], &padrao_tfg[i][1], &padrao_tfg[i][2], &padrao_tfg[i][3]
    	,&padrao_tfg[i][4], &padrao_tfg[i][5], &padrao_tfg[i][6]);
   	}

   	fclose(traffic);

 	
 	std::queue<bool> fila_aux;

    int x_ant = padrao_tfg[0][0];
    int y_ant = padrao_tfg[0][1];
    int contador = 0;
    int posicao = 0;
    std::deque<pacote> auxiliar;
    auxiliar.push_back(pacote(padrao_tfg[0][0],padrao_tfg[0][1], padrao_tfg[0][2], padrao_tfg[0][3],  padrao_tfg[0][4],  padrao_tfg[0][5],  padrao_tfg[0][6]));
    software_defined->deque_pacotes.push_back(auxiliar);

    auxiliar.pop_front();

    
  	for (int i = 1; i < size_pct; ++i){
  		if (x_ant != padrao_tfg[i][0] or y_ant != padrao_tfg[i][1]){
  			contador++;
  			x_ant = padrao_tfg[i][0];
    		y_ant = padrao_tfg[i][1];
    		// cout << contador << "x "<< padrao_tfg[i][0] << " y "<< padrao_tfg[i][1] << endl;
    		auxiliar.push_back(pacote(padrao_tfg[i][0],padrao_tfg[i][1], padrao_tfg[i][2], padrao_tfg[i][3],  padrao_tfg[i][4],  padrao_tfg[i][5],  padrao_tfg[i][6]));
    		software_defined->deque_pacotes.push_back(auxiliar);

    		auxiliar.pop_front();
    		posicao++;
  		} else {
  			contador++;
  			software_defined->deque_pacotes[posicao].push_back(pacote(padrao_tfg[i][0],padrao_tfg[i][1], padrao_tfg[i][2], padrao_tfg[i][3],  padrao_tfg[i][4],  padrao_tfg[i][5],  padrao_tfg[i][6]));

  		}
  	}

    
    // for (int i = 0; i < software_defined->deque_pacotes.size(); ++i)
    // {
    // 	cout << software_defined->deque_pacotes[i].front().qtd_pcts << endl;
    // 	software_defined->deque_pacotes[i].pop_front();
    // 	software_defined->deque_pacotes[i].pop_front();
    // 	cout << software_defined->deque_pacotes[i].front().qtd_pcts << endl<< endl;
    // }
  	// cout << software_defined->deque_pacotes.size() << endl;

  	return 0;
}