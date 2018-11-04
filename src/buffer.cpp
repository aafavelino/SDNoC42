//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#include "buffer.h"

// Adiciona o flit na fila
void Buffer::add() {
	data_flit.push(data.read());
}

// Remove o flit na fila
void Buffer::remove() {
	data_flit.pop();
}

//Se estiver Vazio retorna true, senão retorna false
bool Buffer::isEmpty() {
	return (this->data_flit.empty()) ? true: false;
}
