//
// Created by Adelino, Raul and Yuri on 11/11/18.
//

#ifndef _SD_H_
#define _SD_H_ 

#include "systemc.h"
#include "noc.h"
#include "../constantes/constantes.h"


SC_MODULE (sd) {
	// Tabela de multiplexação
	sc_uint<2> tabela_mux[5][5] = {{-1,1,2,3,0}, {1,-1,2,3,0}, {1,2,-1,3,0}, {1,2,3,-1,0}, {0,1,2,3,-1}};


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


	SC_CTOR(sd) {
		noc42 = new noc("noc_42");

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


	}
};
#endif