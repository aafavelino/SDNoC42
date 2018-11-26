//
// Created by Adelino, Raul and Yuri on 11/11/18.
//

#ifndef _SD_H_
#define _SD_H_ 

#include "systemc.h"
#include "noc.h"
#include "pacote.h"
#include "../constantes/constantes.h"
#define V LARGURA_REDE * ALTURA_REDE



SC_MODULE (sd) {


	std::vector<vector<tuple<int, int> > > rotas;

	// Clock
 	sc_in_clk Clk;
 	unsigned long long int clock;	
 	bool fim_simulacao;

	// Objeto do tipo vector de deques de pacote que fará as delegações de caminhos entre os cores
	std::vector<std::deque<pacote> > deque_pacotes;  
	std::vector<std::deque<long long int> > deque_clock_inicial;  
	std::vector<std::deque<long long int> > deque_clock_final;  

	std::queue<int> solicitacoes_de_rota;

	// Tabela de multiplexação
	sc_uint<2> tabela_mux[5][5] = {{-1,1,2,3,0}, {1,-1,2,3,0}, {1,2,-1,3,0}, {1,2,3,-1,0}, {0,1,2,3,-1}};

	// Matriz para ser consultada de acordo com os algoritmos de rotamento

	int grafo_de_rotas[(ALTURA_REDE*LARGURA_REDE)][(ALTURA_REDE*LARGURA_REDE)];


	// Cores de cada roteador
	sc_out<sc_uint <32> > cores[ALTURA_REDE][LARGURA_REDE];


	// Instância da NoC para simulação
	noc *noc42;

	// sc_signal< sc_uint <2> > sinais_seletores
	sc_signal< sc_uint<2> > sinais_seletores[ALTURA_REDE][LARGURA_REDE][5];

	// sc_signal< sc_uint <1> > sinais_enables
	sc_signal< sc_uint<1> > sinais_enables[ALTURA_REDE][LARGURA_REDE][5];


	// Portas de saídas para setar os seletores dos mux.
	sc_out<sc_uint<2> > set_seletor[ALTURA_REDE][LARGURA_REDE][5];


	// Portas de saídas para setar os enables dos mux.
	sc_out<sc_uint<1> > set_enables[ALTURA_REDE][LARGURA_REDE][5];

	/** 1º ler o arquivo de tráfego e após leitura, verifica quais são os roteadores de 
	 *  origem e destino e somente após isso ele chama o método solicita_rota e o sd dá um
	 *  caminho para comunicação.
	 */
	void injeta_pacote();

	/** 
	 * Este método gera os caminhos solicitados e faz os chaveamentos necessários
	 */
	void solicita_rota();

	void verifica_trailer();

	void remove_rota(tuple<int, int> destino);

	void close_circuit(std::vector<tuple<int,int>> rota);


	

	void parada();


	// Métodos e Funções referêntes ao Algoritmo de Dijikstra
	int min_distancia(int dist[], bool visitados[]);
	int printSolution(int dist[], int n);
	int tupleToInt(tuple<int, int> tpl);
	tuple<int, int> intToTuple(int i);

	// Algoritmos de roteamento implementados
	bool dijkstra(tuple<int, int> origem, tuple<int, int> destino, int pos_solicitante);
	bool roteamento_xy(tuple<int, int> origem, tuple<int, int> destino, int pos_solicitante);
	bool roteamento_west_first(tuple<int, int> origem, tuple<int, int> destino, int pos_solicitante);

	SC_CTOR(sd) {
		noc42 = new noc("noc_42");
		fim_simulacao = false;

		// Preenchimento do Grafo da Aplicação
		for (int i = 0; i < (ALTURA_REDE*LARGURA_REDE); ++i) {
			for (int j = i; j < (ALTURA_REDE*LARGURA_REDE); ++j) {
				grafo_de_rotas[i][j] = 0;
			}
		}



		for (int i = 0; i < (ALTURA_REDE*LARGURA_REDE); ++i) {
			for (int j = i; j < (ALTURA_REDE*LARGURA_REDE); ++j) {
				if(i == j){
					grafo_de_rotas[i][j] = -1;
				}else{

					if((i%ALTURA_REDE) < (ALTURA_REDE - 1)){
						if(j == (i + 1)){
							grafo_de_rotas[i][j] = 1;
							grafo_de_rotas[j][i] = 1;
						}
					}

					if(j == (i + 1)){
						if((j + ALTURA_REDE - 1) < (ALTURA_REDE * LARGURA_REDE)){
							grafo_de_rotas[i][(j + ALTURA_REDE - 1)] = 1;
							grafo_de_rotas[(j + ALTURA_REDE - 1)][i] = 1;
						}
					}
				}
			}
		}


		// Bind sinais entre os mux dos roteadores	
		for (int x = 0; x < ALTURA_REDE; ++x){
			for (int y = 0; y < LARGURA_REDE; ++y){
				// Bind dos cores
				cores[x][y](noc42->canais_locais[x][y]);

				// Bind com seletores (portas de entrada) de cada mux de cada roteador
				noc42->network[x][y]->mux_norte->seletor(sinais_seletores[x][y][NORTE]);
				noc42->network[x][y]->mux_sul->seletor(sinais_seletores[x][y][SUL]);
				noc42->network[x][y]->mux_leste->seletor(sinais_seletores[x][y][LESTE]);
				noc42->network[x][y]->mux_oeste->seletor(sinais_seletores[x][y][OESTE]);
				noc42->network[x][y]->mux_local->seletor(sinais_seletores[x][y][LOCAL]);
				// Bind com as saídas (set_seletor) do SD nos mux de cada roteador
				set_seletor[x][y][NORTE](sinais_seletores[x][y][NORTE]);
				set_seletor[x][y][SUL](sinais_seletores[x][y][SUL]);
				set_seletor[x][y][LESTE](sinais_seletores[x][y][LESTE]);
				set_seletor[x][y][OESTE](sinais_seletores[x][y][OESTE]);
				set_seletor[x][y][LOCAL](sinais_seletores[x][y][LOCAL]);

				// Bind com enables (portas de entrada) de cada mux de cada roteador
				noc42->network[x][y]->mux_norte->enable(sinais_enables[x][y][NORTE]);
				noc42->network[x][y]->mux_sul->enable(sinais_enables[x][y][SUL]);
				noc42->network[x][y]->mux_leste->enable(sinais_enables[x][y][LESTE]);
				noc42->network[x][y]->mux_oeste->enable(sinais_enables[x][y][OESTE]);
				noc42->network[x][y]->mux_local->enable(sinais_enables[x][y][LOCAL]);

				// Bind com as saídas (set_enables) do SD nos mux de cada roteador
				set_enables[x][y][NORTE](sinais_enables[x][y][NORTE]);
				set_enables[x][y][SUL](sinais_enables[x][y][SUL]);
				set_enables[x][y][LESTE](sinais_enables[x][y][LESTE]);
				set_enables[x][y][OESTE](sinais_enables[x][y][OESTE]);
				set_enables[x][y][LOCAL](sinais_enables[x][y][LOCAL]);				
			}
		}



		// Verificar se esse método é sensitivo ao clock também..
		SC_METHOD(injeta_pacote);
			sensitive << Clk.pos();

		SC_METHOD(solicita_rota);
			sensitive << Clk.pos();			
		
		SC_METHOD(verifica_trailer);
			sensitive << Clk.pos();

		SC_METHOD(parada);
			sensitive << Clk.pos();
	}
};
#endif