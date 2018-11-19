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


///////

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









void sd::solicita_rota() {
  // cout<< sc_time_stamp()<<endl;

    
  if (!solicitacoes_de_rota.empty())
  {
    switch(solicitacoes_de_rota.front()) {
      case 0:
        deque_pacotes[0].front().possui_rota = true;  
        set_seletor[0][0][LESTE].write(tabela_mux[LESTE][LOCAL]);
        set_enables[0][0][LESTE].write(1);
        set_enables[0][1][LOCAL].write(1);
        set_seletor[0][1][LOCAL].write(tabela_mux[LOCAL][OESTE]);
        solicitacoes_de_rota.pop();
      break;
      case 1:
        deque_pacotes[1].front().possui_rota = true;
        set_seletor[0][1][OESTE].write(tabela_mux[OESTE][LOCAL]);
        set_enables[0][1][OESTE].write(1);
        set_enables[0][0][SUL].write(1);
        set_seletor[0][0][SUL].write(tabela_mux[SUL][LESTE]);
        set_enables[1][0][LOCAL].write(1);
        set_seletor[1][0][LOCAL].write(tabela_mux[LOCAL][NORTE]);
        solicitacoes_de_rota.pop();
      break;
      case 2:
        deque_pacotes[2].front().possui_rota = true;
        set_seletor[2][0][LESTE].write(tabela_mux[LESTE][LOCAL]);
        set_enables[2][0][LESTE].write(1);
        set_enables[2][1][LOCAL].write(1);
        set_seletor[2][1][LOCAL].write(tabela_mux[LOCAL][OESTE]);
        solicitacoes_de_rota.pop();
      break;
      case 3:
        deque_pacotes[3].front().possui_rota = true;
        set_seletor[2][2][OESTE].write(tabela_mux[OESTE][LOCAL]);
        set_enables[2][2][OESTE].write(1);
        set_enables[2][1][OESTE].write(1);
        set_seletor[2][1][OESTE].write(tabela_mux[OESTE][LESTE]);
        set_enables[2][0][NORTE].write(1);
        set_seletor[2][0][NORTE].write(tabela_mux[NORTE][LESTE]);   
        set_enables[1][0][NORTE].write(1);
        set_seletor[1][0][NORTE].write(tabela_mux[NORTE][SUL]); 
        set_enables[0][0][LOCAL].write(1);
        set_seletor[0][0][LOCAL].write(tabela_mux[LOCAL][SUL]);                 
        solicitacoes_de_rota.pop();
      break;
    }
  }
}