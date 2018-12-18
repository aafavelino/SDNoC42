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
#include <../constantes/constantes.h>
#include "sd.h"

using namespace std;


std::function<int (int, int)> func = [](int i, int j) {
  return rand() % i + j;
};

int sc_main (int argc, char* argv[]) {

  std::srand (unsigned(std::time(0)));

	// Instância do software defined
	sd *software_defined = new sd("SD");
	// Geração de sinal de clock para métodos sensíveis a mudança de clock
	sc_clock clock("clock", 1, SC_NS, 1, 1, SC_NS);
	software_defined->Clk(clock);


	// Leitura do Arquivo de tráfego
	// FILE *traffic;
	// int **padrao_tfg;
	// int size_pct = 0;

	// traffic = fopen("traffic.txt","r");
	// fscanf(traffic,"tg %i",&size_pct);
	// padrao_tfg = (int**)malloc(size_pct * sizeof(int*));

	/**
	 * Atribuindo os fluxos de tráfego ao padrão_tfg onde essa matriz será utilizada para 
	 * construir os pacotes da simulação.
	 */
	// for (int i = 0; i < size_pct; i++) {
 //    	padrao_tfg[i] = (int*) malloc(7 * sizeof(int));
 //    	fscanf(traffic,"%i %i %i %i %i %i %i", &padrao_tfg[i][0], &padrao_tfg[i][1], &padrao_tfg[i][2], &padrao_tfg[i][3]
 //    	,&padrao_tfg[i][4], &padrao_tfg[i][5], &padrao_tfg[i][6]);
 //   	}

 //   	fclose(traffic);




  double **padrao_tfg;
  int size_pct = 0;
  int size_fluxo = 0;

  std::vector<std::tuple<int, int> > map_roteadores;

  for(int i = ALTURA_REDE - 1; i >= 0; i--){
    for(int j = 0; j < LARGURA_REDE; j++){
      map_roteadores.push_back(std::make_tuple(i,j));
      cout << i << j << " ";
    }
    cout << endl;
  }

  int qtd_de_roteadores = ALTURA_REDE * LARGURA_REDE;
  int qtd_de_fluxos = 240;
  std::vector<int> roteadores;

  // Leitura do Arquivo de tráfego
  FILE *traffic;
  traffic = fopen("traffic.txt","r");
  
  // fscanf(traffic,"tg_%i",&size_pct);
  // fscanf(traffic,"%i",&size_fluxo);
  // roteadores.push_back(size_fluxo);



  padrao_tfg = (double**)malloc(qtd_de_fluxos * sizeof(double*));

  for (int i = 0; i < qtd_de_fluxos; i++) 
     padrao_tfg[i] = (double*) malloc(15 * sizeof(double));



  int indice = 0;



  for (int j = 0; j < qtd_de_roteadores; ++j){
    fscanf(traffic,"\ntg_%i",&size_pct);
    fscanf(traffic,"\n%i",&size_fluxo);    
    roteadores.push_back(size_fluxo);

    for (int i = 0; i < roteadores[j]; ++i,indice++){
      fscanf(traffic,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &padrao_tfg[indice][0], &padrao_tfg[indice][1], &padrao_tfg[indice][2], &padrao_tfg[indice][3], &padrao_tfg[indice][4], &padrao_tfg[indice][5], &padrao_tfg[indice][6], &padrao_tfg[indice][7], &padrao_tfg[indice][8], &padrao_tfg[indice][9], &padrao_tfg[indice][10], &padrao_tfg[indice][11], &padrao_tfg[indice][12], &padrao_tfg[indice][13], &padrao_tfg[indice][14]);
        
      // cout << "testes "<< padrao_tfg[indice][7] << endl;
    }


    // cout << "entrou "<< size_pct  <<", " << size_fluxo << " "<< roteadores.size() << endl;
  }

  fclose(traffic);





    cout << "antes" << endl;







    /**
     * Aqui é feito o sorteio dos fluxos para o padrão uniforme.
     * caso queira fazer o sorteio sete o primeiro argumento para
     * 1 e os respectivos argumentos serão a largura e altura da NoC.
     */
    if (atoi(argv[1]) == 1) {
      int fluxos = atoi(argv[2]) * atoi(argv[3]);
      int value = fluxos-1;
      int topo = value;
      for (int i = 0; i < fluxos; ++i) {
        for (int j = i*value; j < topo; ++j) {
          swap(padrao_tfg[j],padrao_tfg[(j + (rand() % (int)((topo-1) - j + 1)))]);
        }
          topo += value;
      }
    }


 	
 	std::queue<bool> fila_aux;
    // int x_ant = padrao_tfg[0][0];
    // int y_ant = padrao_tfg[0][1];
    // int contador = 0;
    // int posicao = 0;
    std::deque<pacote> deque_auxiliar;
    std::deque<long long int> deque_auxiliar_clock_inicial;
    std::deque<long long int> deque_auxiliar_clock_final;

   //  deque_auxiliar.push_back(pacote(padrao_tfg[0][0],padrao_tfg[0][1], padrao_tfg[0][2], padrao_tfg[0][3],  padrao_tfg[0][4],  padrao_tfg[0][5],  padrao_tfg[0][6],0));
   //  software_defined->deque_pacotes.push_back(deque_auxiliar);
   //  software_defined->deque_clock_inicial.push_back(deque_auxiliar_clock_inicial);
   //  software_defined->deque_clock_final.push_back(deque_auxiliar_clock_final);

   //  deque_auxiliar.pop_front();

    
  	// for (int i = 1; i < size_pct; ++i){
  	// 	if (x_ant != padrao_tfg[i][0] or y_ant != padrao_tfg[i][1]){
  	// 		contador++;
  	// 		x_ant = padrao_tfg[i][0];
   //  		y_ant = padrao_tfg[i][1];
   //  		deque_auxiliar.push_back(pacote(padrao_tfg[i][0],padrao_tfg[i][1], padrao_tfg[i][2], padrao_tfg[i][3],  padrao_tfg[i][4],  padrao_tfg[i][5],  padrao_tfg[i][6],contador));
   //  		software_defined->deque_pacotes.push_back(deque_auxiliar);
   //      software_defined->deque_clock_inicial.push_back(deque_auxiliar_clock_inicial);
   //      software_defined->deque_clock_final.push_back(deque_auxiliar_clock_final);


   //  		deque_auxiliar.pop_front();
   //  		posicao++;
  	// 	} else {
  	// 		software_defined->deque_pacotes[posicao].push_back(pacote(padrao_tfg[i][0],padrao_tfg[i][1], padrao_tfg[i][2], padrao_tfg[i][3],  padrao_tfg[i][4],  padrao_tfg[i][5],  padrao_tfg[i][6], contador));

  	// 	}
  	// }




    cout << "antes" << endl;



  int foda = 0;
  for (int j = 0; j < qtd_de_roteadores; ++j){   
    for (int i = 0; i < roteadores[j]; ++i,indice++){
     
      // map_roteadores

      cout << std::get<0>(map_roteadores[j]) << std::get<1>(map_roteadores[j])<<" "<< std::get<0>(map_roteadores[padrao_tfg[foda][1]])<< std::get<1>(map_roteadores[padrao_tfg[foda][1]]) <<" " << padrao_tfg[foda][8]<<" "<<  padrao_tfg[foda][5]<< " " << padrao_tfg[foda][9] << " "<<j<< endl;
      deque_auxiliar.push_back(pacote(std::get<0>(map_roteadores[j]),std::get<1>(map_roteadores[j]), std::get<0>(map_roteadores[padrao_tfg[foda][1]]), std::get<1>(map_roteadores[padrao_tfg[foda][1]]),  padrao_tfg[foda][8],  padrao_tfg[foda][5],  padrao_tfg[foda][9],j));
     
      foda++;



      // deque_auxiliar.push_back(pacote(padrao_tfg[0][0],padrao_tfg[0][1], padrao_tfg[0][2], padrao_tfg[0][3],  padrao_tfg[0][4],  padrao_tfg[0][5],  padrao_tfg[0][6],j));
      software_defined->deque_pacotes.push_back(deque_auxiliar);
      software_defined->deque_clock_inicial.push_back(deque_auxiliar_clock_inicial);
      software_defined->deque_clock_final.push_back(deque_auxiliar_clock_final);

      deque_auxiliar.pop_front();    
      
    }

  }














    cout << "antes" << endl;
  	// Roda a simulação até encontrar um sc_stop();
  	sc_start(); 
    cout << "depois" << endl;


    double media = 0;

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

  	return 0;
}