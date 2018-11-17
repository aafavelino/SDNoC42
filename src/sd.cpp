//
// Created by Adelino, Raul and Yuri on 11/11/18.
//
#include "sd.h"


void sd::injeta_pacote() {

	cout<< sc_time_stamp()<<endl;

	// cout << "true "<< deque_pacotes[0].front().fila_flits.size() << endl;
	for (int i = 0; i < deque_pacotes.size(); ++i) {
		deque_pacotes[i].front().contador_idleCycles++;
		/** Se o pacote não está vazio e não possuir ainda uma rota
		 *  então o mesmo irá solicitar ao mestre uma rota;
		 */
		if (!deque_pacotes[i].empty() and (deque_pacotes[i].front().possui_rota == false) and (deque_pacotes[i].front().solicitou_rota == false) and (deque_pacotes[i].front().contador_idleCycles >= deque_pacotes[i].front().idleCycles)) {
			deque_pacotes[i].front().solicitou_rota = true;
			solicitacoes_de_rota.push(i);
			deque_pacotes[i].front().contador_idleCycles = 0;
		} else if(!deque_pacotes[i].empty() and (deque_pacotes[i].front().possui_rota == true)) {
			// Caso seja o último flit deve-se tirar a flag que diz que possui uma rota


			if (deque_pacotes[i].front().fila_flits.front().data == 30)
			{
				deque_pacotes[i].front().possui_rota = false;
			}
			cores[std::get<0>(deque_pacotes[i].front().origem)][std::get<1>(deque_pacotes[i].front().origem)].write(deque_pacotes[i].front().fila_flits.front().data);
			// cout << deque_pacotes[i].front().fila_flits.front().data << endl;
			deque_pacotes[i].front().fila_flits.pop();
			if (deque_pacotes[i].front().fila_flits.empty())
			{
				deque_pacotes[i].pop_front();
			}
		}
	}
	// cout << noc42->network[0][1]->mux_local->saida<<" " << noc42->network[1][0]->mux_local->saida << " "<<noc42->network[2][1]->mux_local->saida<< " "<<noc42->network[0][0]->mux_local->saida << endl;
	
}

void sd::solicita_rota() {

	next_trigger();
	cout<< sc_time_stamp()<<endl;

  	
	if (!solicitacoes_de_rota.empty())
	{
		switch(solicitacoes_de_rota.front()) {
			case 0:
				deque_pacotes[0].front().possui_rota = true;	
				set_seletor[0][0][LESTE].write(tabela_mux[LESTE][LOCAL]);
				set_enables[0][0][LESTE].write(1);
				set_enables[0][1][LOCAL].write(1);
				set_seletor[0][1][LOCAL].write(tabela_mux[LOCAL][OESTE]);
				solicitacoes_de_rota.pop();
			break;
			case 1:
				deque_pacotes[1].front().possui_rota = true;
				set_seletor[0][1][OESTE].write(tabela_mux[OESTE][LOCAL]);
				set_enables[0][1][OESTE].write(1);
				set_enables[0][0][SUL].write(1);
				set_seletor[0][0][SUL].write(tabela_mux[SUL][LESTE]);
				set_enables[1][0][LOCAL].write(1);
				set_seletor[1][0][LOCAL].write(tabela_mux[LOCAL][NORTE]);
				solicitacoes_de_rota.pop();
			break;
			case 2:
				deque_pacotes[2].front().possui_rota = true;
				set_seletor[2][0][LESTE].write(tabela_mux[LESTE][LOCAL]);
				set_enables[2][0][LESTE].write(1);
				set_enables[2][1][LOCAL].write(1);
				set_seletor[2][1][LOCAL].write(tabela_mux[LOCAL][OESTE]);
				solicitacoes_de_rota.pop();
			break;
			case 3:
				deque_pacotes[3].front().possui_rota = true;
				set_seletor[2][2][OESTE].write(tabela_mux[OESTE][LOCAL]);
				set_enables[2][2][OESTE].write(1);
				set_enables[2][1][OESTE].write(1);
				set_seletor[2][1][OESTE].write(tabela_mux[OESTE][LESTE]);
				set_enables[2][0][NORTE].write(1);
				set_seletor[2][0][NORTE].write(tabela_mux[NORTE][LESTE]);		
				set_enables[1][0][NORTE].write(1);
				set_seletor[1][0][NORTE].write(tabela_mux[NORTE][SUL]);	
				set_enables[0][0][LOCAL].write(1);
				set_seletor[0][0][LOCAL].write(tabela_mux[LOCAL][SUL]);									
				solicitacoes_de_rota.pop();
			break;
		}
	}



}
