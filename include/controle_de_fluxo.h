//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#ifndef _CONTROLE_DE_FLUXO_H_
#define _CONTROLE_DE_FLUXO_H_ 

#include <iostream>
#include <queue>
#include "systemc.h"


SC_MODULE (controle_de_fluxo){

public:
	sc_in<sc_uint<32> > entrada;
	sc_out<sc_uint<32> > saida;
	
	void action(){
		saida.write(entrada.read());
	}

	SC_CTOR(controle_de_fluxo) {
        SC_METHOD(action);
        	sensitive << entrada;
	}
};
#endif