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
    std::deque<pacote> deque_auxiliar;
    std::deque<long long int> deque_auxiliar_clock_inicial;
    std::deque<long long int> deque_auxiliar_clock_final;

    deque_auxiliar.push_back(pacote(padrao_tfg[0][0],padrao_tfg[0][1], padrao_tfg[0][2], padrao_tfg[0][3],  padrao_tfg[0][4],  padrao_tfg[0][5],  padrao_tfg[0][6],0));
    software_defined->deque_pacotes.push_back(deque_auxiliar);
    software_defined->deque_clock_inicial.push_back(deque_auxiliar_clock_inicial);
    software_defined->deque_clock_final.push_back(deque_auxiliar_clock_final);

    deque_auxiliar.pop_front();

    
  	for (int i = 1; i < size_pct; ++i){
  		if (x_ant != padrao_tfg[i][0] or y_ant != padrao_tfg[i][1]){
  			contador++;
  			x_ant = padrao_tfg[i][0];
    		y_ant = padrao_tfg[i][1];
    		deque_auxiliar.push_back(pacote(padrao_tfg[i][0],padrao_tfg[i][1], padrao_tfg[i][2], padrao_tfg[i][3],  padrao_tfg[i][4],  padrao_tfg[i][5],  padrao_tfg[i][6],contador));
    		software_defined->deque_pacotes.push_back(deque_auxiliar);
        software_defined->deque_clock_inicial.push_back(deque_auxiliar_clock_inicial);
        software_defined->deque_clock_final.push_back(deque_auxiliar_clock_final);


    		deque_auxiliar.pop_front();
    		posicao++;
  		} else {
  			// contador++;
  			software_defined->deque_pacotes[posicao].push_back(pacote(padrao_tfg[i][0],padrao_tfg[i][1], padrao_tfg[i][2], padrao_tfg[i][3],  padrao_tfg[i][4],  padrao_tfg[i][5],  padrao_tfg[i][6], contador));

  		}
  	}

	// software_defined->set_seletor[0][0][LESTE].write(software_defined->tabela_mux[LESTE][LOCAL]);
	// software_defined->set_enables[0][0][LESTE].write(1);
	// software_defined->set_enables[0][1][LOCAL].write(1);
	// software_defined->set_seletor[0][1][LOCAL].write(software_defined->tabela_mux[LOCAL][OESTE]);

	// software_defined->set_seletor[0][1][OESTE].write(software_defined->tabela_mux[OESTE][LOCAL]);
	// software_defined->set_enables[0][1][OESTE].write(1);

	// // software_defined->set_enables[0][0][LESTE].write(1);
	// // software_defined->set_seletor[0][0][LESTE].write(software_defined->tabela_mux[SUL][LESTE]);

	// software_defined->set_enables[0][0][SUL].write(1);
	// software_defined->set_seletor[0][0][SUL].write(software_defined->tabela_mux[SUL][LESTE]);

	// software_defined->set_enables[1][0][LOCAL].write(1);
	// software_defined->set_seletor[1][0][LOCAL].write(software_defined->tabela_mux[LOCAL][NORTE]);
	

	// software_defined->cores[0][0].write(6);
	// software_defined->cores[0][1].write(5);



  	// Roda a simulação até encontrar um sc_stop();
  	sc_start(); 

    double media = 0;
    // for (int i = 0; i < software_defined->deque_clock_inicial.size(); ++i){
      
    //   double media_interna = 0;

    //   for (int j = 0; j < software_defined->deque_clock_inicial[i].size(); ++j) {
    //     // media_interna += software_defined->deque_clock_final[i][j]-software_defined->deque_clock_inicial[i][j];
    //     media_interna += software_defined->deque_clock_inicial[i][j];
        
    //   }

    //   media += (media_interna/software_defined->deque_clock_inicial[i].size());
      
    // }

    // media = media/software_defined->deque_clock_inicial.size();


    for (int i = 0; i < software_defined->deque_clock_inicial.size(); ++i)
    {
      for (int j = 0; j < software_defined->deque_clock_inicial[i].size(); ++j)
      {
        media += software_defined->deque_clock_inicial[i][j];
      }
    }

    int divisao_interna = 0;
    for (int i = 0; i < software_defined->deque_clock_inicial.size(); ++i)
    {
      divisao_interna += software_defined->deque_clock_inicial[i].size();
    }


    cout << "Latência média da simulação " << media/divisao_interna << endl;
  	// cout << software_defined->noc42->network[0][1]->mux_local->saida << endl;
  	// cout << software_defined->noc42->network[1][0]->mux_local->saida << endl;
  	// cout << software_defined->noc42->network[0][1]->mux_oeste->entrada_4.read() << endl;

  	return 0;
}