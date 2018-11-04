//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#include "buffer.h"

// Adiciona o flit na fila
void buffer::add() {
	if (this->data_flit.empty()){
		data_flit.push(data.read());
		ack.write(true);
	}
}

// Remove o flit na fila
void buffer::consome() {
	data_out.write(data_flit.front());

	data_flit.pop();
	ack.write(false);
}
