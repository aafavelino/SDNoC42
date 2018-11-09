//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#include "roteador.h"

void roteador::buffer_ack(){
	if(ack_leste->read()){
		buffer_leste->consome();
	}

	if(ack_oeste->read()){
		buffer_oeste->consome();
	}

	if(ack_norte->read()){
		buffer_norte->consome();
	}

	if(ack_sul->read()){
		buffer_sul->consome();
	}

	if(ack_local->read()){
		buffer_local->consome();
	}
}