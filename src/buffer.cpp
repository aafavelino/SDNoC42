//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#include "buffer.h"

// Adiciona o flit na fila
void Buffer::add() {
	if (this->data_flit.empty())
	{
		data_flit.push(data.read());
	}
	
}

// Remove o flit na fila
sc_uint <32> Buffer::consome() {
	sc_uint <32> data =  data_flit.front();
	data_flit.pop();
	return data;
}

//Se estiver Vazio retorna true, senão retorna false
bool Buffer::isEmpty() {
	return (this->data_flit.empty()) ? true: false;
}
