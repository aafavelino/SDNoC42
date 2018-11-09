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
#include "controle_de_fluxo.h"


SC_MODULE (roteador){
	// Canais de entrada.
	buffer *buffer_leste, *buffer_oeste, *buffer_norte, *buffer_sul, *buffer_local;

	// Multiplexadores dos canais
	mux *mux_leste, *mux_oeste, *mux_norte, *mux_sul, *mux_local;

	// ACKs de cada buffer, indicando se existe algo ou não
	sc_in<bool> ack_leste, ack_oeste, ack_norte, ack_sul, ack_local;

	sc_signal<bool> bufferACKL_sig_ackL, bufferACKO_sig_ackO, bufferACKN_sig_ackN, bufferACKS_sig_ackS,
					bufferACKLoc_sig_ackLoc;

	sc_signal<sc_uint<32> > bufferL_sig_mux, bufferO_sig_mux, bufferN_sig_mux, bufferS_sig_mux, bufferLoc_sig_mux;

	void buffer_ack();

	// Construtor padrão.
	SC_CTOR(roteador) {
		buffer_leste = new buffer("buffer_leste");
		buffer_oeste = new buffer("buffer_oeste");
		buffer_norte = new buffer("buffer_norte");
		buffer_sul = new buffer("buffer_sul");
		buffer_local = new buffer("buffer_local");

		mux_leste = new mux("mux_leste");
		mux_oeste = new mux("mux_oeste");
		mux_norte = new mux("mux_norte");
		mux_sul = new mux("mux_sul");
		mux_local = new mux("mux_local");


		buffer_leste->ack(bufferACKL_sig_ackL);
		ack_leste(bufferACKL_sig_ackL);

		buffer_oeste->ack(bufferACKO_sig_ackO);
		ack_oeste(bufferACKO_sig_ackO);

		buffer_norte->ack(bufferACKN_sig_ackN);
		ack_norte(bufferACKN_sig_ackN);

		buffer_sul->ack(bufferACKS_sig_ackS);
		ack_sul(bufferACKS_sig_ackS);

		buffer_local->ack(bufferACKLoc_sig_ackLoc);
		ack_local(bufferACKLoc_sig_ackLoc);


		buffer_leste->data_out(bufferL_sig_mux);
		mux_oeste->entrada_4(bufferL_sig_mux);
		mux_norte->entrada_3(bufferL_sig_mux);
		mux_sul->entrada_3(bufferL_sig_mux);
		mux_local->entrada_3(bufferL_sig_mux);

		buffer_oeste->data_out(bufferO_sig_mux);
		mux_leste->entrada_4(bufferO_sig_mux);
		mux_norte->entrada_4(bufferO_sig_mux);
		mux_sul->entrada_4(bufferO_sig_mux);
		mux_local->entrada_4(bufferO_sig_mux);

		buffer_norte->data_out(bufferN_sig_mux);
		mux_leste->entrada_2(bufferN_sig_mux);
		mux_oeste->entrada_2(bufferN_sig_mux);
		mux_sul->entrada_2(bufferN_sig_mux);
		mux_local->entrada_1(bufferN_sig_mux);

		buffer_sul->data_out(bufferS_sig_mux);
		mux_leste->entrada_3(bufferS_sig_mux);
		mux_oeste->entrada_3(bufferS_sig_mux);
		mux_norte->entrada_2(bufferS_sig_mux);
		mux_local->entrada_2(bufferS_sig_mux);

		buffer_local->data_out(bufferLoc_sig_mux);
		mux_leste->entrada_1(bufferLoc_sig_mux);
		mux_oeste->entrada_1(bufferLoc_sig_mux);
		mux_norte->entrada_1(bufferLoc_sig_mux);
		mux_sul->entrada_1(bufferLoc_sig_mux);


		

		SC_METHOD(buffer_ack);
			sensitive << ack_leste, ack_oeste, ack_norte, ack_sul, ack_local;
	}	
};
#endif