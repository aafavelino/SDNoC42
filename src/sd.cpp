//
// Created by Adelino, Raul and Yuri on 11/11/18.
//
#include "sd.h"

void sd::solicita_rota() {
	set_seletor[1][0][LESTE].write(tabela_mux[LESTE][LOCAL]);
	set_seletor[1][1][LOCAL].write(tabela_mux[LOCAL][OESTE]);
}