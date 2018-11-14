//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#ifndef _PACOTE_H_
#define _PACOTE_H_ 
#include <iostream>
#include <systemc.h>
#include "flit.h"

class pacote {
	
public:

// Modelo de tráfego
// 0 0 2 0 23 1 10 

// Mesh padrão:  
// 	     xs ys xd yd qtd_flits qtd_pcts idleCycles 
//       xs: X source; 
// 		 xd: X dest
//       xs: Y source; 
// 		 xd: Y dest     
//		 qtd_flits : Quantos flits haverá dentro de cada pacote
// 		 qtd_pcts : Quantos pacotes serão enviados por fluxo         
//       idleCycles: De quanto em quanto tempo é enviado 



	std::queue<Flit> fila_flits;
	std::tuple<int, int> origem;
	std::tuple<int, int> destino;
	int qtd_flits;
	int idleCycles; 
	int qtd_pcts;
	Flit flit;


	pacote(int xs, int ys, int xd, int yd, int qtd_pcts, int qtd_flits, int idleCycles) {
		
		this->qtd_pcts =  qtd_pcts;
		this->origem = std::make_tuple (xs,ys);
		this->destino = std::make_tuple (xd,yd);
		this->qtd_flits = qtd_flits;
		this->idleCycles = idleCycles;


		for (int i = 0; i < qtd_pcts; ++i) {
			for (int j = 0; j < qtd_flits; ++j) {

				this->flit.data = 0;
				// Caso seja o flit trailer, escreve 1;
				if(i == (qtd_flits-1)) {
					this->flit.data = 1;
				}

				fila_flits.push(flit);
			}
		}
	};
};

#endif