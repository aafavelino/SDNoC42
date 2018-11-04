//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#ifndef _ROTEADOR_H_
#define _ROTEADOR_H_

#include <iostream>
#include <systemc.h>
#include <time.h>
#include "buffer.h"
#include "mux.h"

SC_MODULE (roteador){
	// Canais de entrada.
	sc_in< sc_uint <32> > entrada_leste, entrada_oeste, entrada_norte, entrada_sul, entrada_local;
	// Seletores dos multiplexadores.
	sc_in< sc_uint <2> > seletor_leste, seletor_oeste, seletor_norte, seletor_sul, seletor_local;
	// Enables dos multiplexadores.
	sc_in< sc_uint <2> > enable_leste, enable_oeste, enable_norte, enable_sul, enable_local;

	// Canais de saída.
	sc_out< sc_uint <32> > saida_leste, saida_oeste, saida_norte, saida_sul, saida_local;

	// Multiplexadores dos canais
	Mux mux_leste, mux_oeste, mux_norte, mux_sul, mux_local;


	// // Procedimento para selecionar a porta de entrada.
	// void action();

	// // Construtor padrão.
	// SC_CTOR(mux) {
	// 	// O método action será sensível à todas as portas de entrada.
	// 	SC_METHOD(action);
	// 		sensitive << entrada_1, entrada_2, entrada_3, entrada_4, seletor, enable;
	// }	
};
#endif