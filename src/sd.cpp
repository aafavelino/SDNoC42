//
// Created by Adelino, Raul and Yuri on 11/11/18.
//
#include "sd.h"

void sd::solicita_rota() {
	set_seletor[0][0][LESTE].write(tabela_mux[LESTE][LOCAL]);
	set_enables[0][0][LESTE].write(1);
	set_enables[0][1][LOCAL].write(1);
	set_seletor[0][1][LOCAL].write(tabela_mux[LOCAL][OESTE]);

	set_seletor[0][1][OESTE].write(tabela_mux[OESTE][LOCAL]);
	set_enables[0][1][OESTE].write(1);

	set_enables[0][0][LESTE].write(1);
	set_seletor[0][0][LESTE].write(tabela_mux[SUL][LESTE]);

	set_enables[0][0][SUL].write(1);
	set_seletor[0][0][SUL].write(tabela_mux[SUL][LESTE]);

	set_enables[1][0][LOCAL].write(1);
	set_seletor[1][0][LOCAL].write(tabela_mux[LOCAL][NORTE]);

}