//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#include "mux.h"

void mux::action() {
	if (enable.read() == 1)
	{
		switch (seletor.read()) {
			case 0:
				saida.write(entrada_1.read());
			break;
			case 1:
				saida.write(entrada_2.read());
			break;
			case 2:
				saida.write(entrada_3.read());
			break;
			case 3:
				saida.write(entrada_4.read());
			break;												
		}
	}
}
