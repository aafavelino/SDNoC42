//
// Created by Adelino, Raul and Yuri on 11/11/18.
//
#include "sd.h"


void sd::injeta_pacote() {
	clock++;

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
			if (deque_pacotes[i].front().fila_flits.front().data == TRAILER)
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

	cout << noc42->network[0][1]->mux_local->saida<<" " << noc42->network[3][3]->mux_local->saida << " "<<noc42->network[1][0]->mux_local->saida<< " "<<noc42->network[2][1]->mux_local->saida<<" "<<noc42->network[0][0]->mux_local->saida << endl;
	
}

void sd::solicita_rota() {
	// cout<< sc_time_stamp()<<endl;
	if (!solicitacoes_de_rota.empty())
	{	
		roteamento_xy(deque_pacotes[solicitacoes_de_rota.front()].front().origem,deque_pacotes[solicitacoes_de_rota.front()].front().destino,solicitacoes_de_rota.front());
	}
}

bool sd::roteamento_xy(tuple<int, int> origem, tuple<int, int> destino, int pos_solicitante) {

	for(int i = 0; i < rotas.size(); i++){
		if(rotas[i][0] == origem){
			return false;
		}

		if(rotas[i][rotas[i].size()-1] == destino){
			return false;
		}
	}

	std::vector<tuple<int, int> > rota;
	int x_atual = std::get<0>(origem);
	int y_atual = std::get<1>(origem);

	rota.push_back(std::make_tuple (x_atual,y_atual));


	while (true) {
		if (y_atual < std::get<1>(destino)) {	
			y_atual++;
		} else if (y_atual > std::get<1>(destino)) {
			y_atual--;
		} else {
			if (x_atual < std::get<0>(destino)) {
				x_atual++;
			} else if (x_atual > std::get<0>(destino)) {
				x_atual--;
			} else {
				break;
			} 
		}
		rota.push_back(std::make_tuple (x_atual,y_atual));
	}


	for (int i = 0; i < (rota.size()-1); ++i) {

		if (grafo_de_rotas[((std::get<0>(rota[i])*ALTURA_REDE)+(std::get<1>(rota[i])))][((std::get<0>(rota[i+1])*ALTURA_REDE)+(std::get<1>(rota[i+1])))] != 1) {
			// cout << "Não TEM ROTA" << endl;
			return false;
		}
		
	}

	// cout << "TEM ROTA" << endl;
	deque_pacotes[pos_solicitante].front().possui_rota = true;

	if(std::get<0>(rota[1]) > std::get<0>(rota[0])){
		set_seletor[std::get<0>(rota[0])][std::get<1>(rota[0])][SUL].write(tabela_mux[SUL][LOCAL]);
		set_enables[std::get<0>(rota[0])][std::get<1>(rota[0])][SUL].write(1);
	}else if(std::get<0>(rota[1]) < std::get<0>(rota[0])){
		set_seletor[std::get<0>(rota[0])][std::get<1>(rota[0])][NORTE].write(tabela_mux[NORTE][LOCAL]);
		set_enables[std::get<0>(rota[0])][std::get<1>(rota[0])][NORTE].write(1);
	}else if(std::get<1>(rota[1]) > std::get<1>(rota[0])){
		set_seletor[std::get<0>(rota[0])][std::get<1>(rota[0])][LESTE].write(tabela_mux[LESTE][LOCAL]);
		set_enables[std::get<0>(rota[0])][std::get<1>(rota[0])][LESTE].write(1);
	}else if(std::get<1>(rota[1]) < std::get<1>(rota[0])){
		set_seletor[std::get<0>(rota[0])][std::get<1>(rota[0])][OESTE].write(tabela_mux[OESTE][LOCAL]);
		set_enables[std::get<0>(rota[0])][std::get<1>(rota[0])][OESTE].write(1);
	}


	for (int i = 1; i < (rota.size()-1); i++) {
		grafo_de_rotas[((std::get<0>(rota[i])*ALTURA_REDE)+(std::get<1>(rota[i])))][((std::get<0>(rota[i+1])*ALTURA_REDE)+(std::get<1>(rota[i+1])))] = 0;

		if(std::get<0>(rota[i-1]) < std::get<0>(rota[i])){
			if(std::get<0>(rota[i+1]) > std::get<0>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][SUL].write(tabela_mux[SUL][NORTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][SUL].write(1);
			}else if(std::get<1>(rota[i+1]) > std::get<1>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][LESTE].write(tabela_mux[LESTE][NORTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][LESTE].write(1);
			}else if(std::get<1>(rota[i+1]) < std::get<1>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][OESTE].write(tabela_mux[OESTE][NORTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][OESTE].write(1);
			}
		}else if(std::get<0>(rota[i-1]) > std::get<0>(rota[i])){
			if(std::get<0>(rota[i+1]) < std::get<0>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][NORTE].write(tabela_mux[NORTE][SUL]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][NORTE].write(1);
			}else if(std::get<1>(rota[i+1]) > std::get<1>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][LESTE].write(tabela_mux[LESTE][SUL]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][LESTE].write(1);
			}else if(std::get<1>(rota[i+1]) < std::get<1>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][OESTE].write(tabela_mux[OESTE][SUL]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][OESTE].write(1);
			}
		}else if(std::get<1>(rota[i-1]) < std::get<1>(rota[i])){
			if(std::get<0>(rota[i+1]) > std::get<0>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][SUL].write(tabela_mux[SUL][OESTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][SUL].write(1);
			}else if(std::get<0>(rota[i+1]) < std::get<0>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][NORTE].write(tabela_mux[NORTE][OESTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][NORTE].write(1);
			}else if(std::get<1>(rota[i+1]) > std::get<1>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][LESTE].write(tabela_mux[LESTE][OESTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][LESTE].write(1);
			}
		}else if(std::get<1>(rota[i-1]) > std::get<1>(rota[i])){
			if(std::get<0>(rota[i+1]) > std::get<0>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][SUL].write(tabela_mux[SUL][LESTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][SUL].write(1);
			}else if(std::get<0>(rota[i+1]) < std::get<0>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][NORTE].write(tabela_mux[NORTE][LESTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][NORTE].write(1);
			}else if(std::get<1>(rota[i+1]) < std::get<1>(rota[i])){
				set_seletor[std::get<0>(rota[i])][std::get<1>(rota[i])][OESTE].write(tabela_mux[OESTE][LESTE]);
				set_enables[std::get<0>(rota[i])][std::get<1>(rota[i])][OESTE].write(1);
			}
		}
	}

	if(std::get<0>(rota[(rota.size()-2)]) < std::get<0>(rota[(rota.size()-1)])){
		set_seletor[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(tabela_mux[LOCAL][NORTE]);
		set_enables[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(1);
	}else if(std::get<0>(rota[(rota.size()-2)]) > std::get<0>(rota[(rota.size()-1)])){
		set_seletor[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(tabela_mux[LOCAL][SUL]);
		set_enables[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(1);
	}else if(std::get<1>(rota[(rota.size()-2)]) < std::get<1>(rota[(rota.size()-1)])){
		set_seletor[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(tabela_mux[LOCAL][OESTE]);
		set_enables[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(1);
	}else if(std::get<1>(rota[(rota.size()-2)]) > std::get<1>(rota[(rota.size()-1)])){
		set_seletor[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(tabela_mux[LOCAL][LESTE]);
		set_enables[std::get<0>(rota[(rota.size()-1)])][std::get<1>(rota[(rota.size()-1)])][LOCAL].write(1);
	}

	rotas.push_back(rota);

	solicitacoes_de_rota.pop();
	return true;
}

void sd::remove_rota(tuple<int, int> destino) {
	for (int i = 0; i < rotas.size(); ++i) {
		if (rotas[i].back() == destino) {

			if(std::get<0>(rotas[i][1]) > std::get<0>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][SUL].write(0);
			}else if(std::get<0>(rotas[i][1]) < std::get<0>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][NORTE].write(0);
			}else if(std::get<1>(rotas[i][1]) > std::get<1>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][LESTE].write(0);
			}else if(std::get<1>(rotas[i][1]) < std::get<1>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][OESTE].write(0);
			}

			for (int j = 1; j < (rotas[i].size()-1); j++) {
				grafo_de_rotas[((std::get<0>(rotas[i][j])*ALTURA_REDE)+(std::get<1>(rotas[i][j])))][((std::get<0>(rotas[i][j+1])*ALTURA_REDE)+(std::get<1>(rotas[i][j+1])))] = 1;

				if(std::get<0>(rotas[i][j-1]) < std::get<0>(rotas[i][j])){
					if(std::get<0>(rotas[i][j+1]) > std::get<0>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][SUL].write(0);
					}else if(std::get<1>(rotas[i][j+1]) > std::get<1>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][LESTE].write(0);
					}else if(std::get<1>(rotas[i][j+1]) < std::get<1>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][OESTE].write(0);
					}
				}else if(std::get<0>(rotas[i][j-1]) > std::get<0>(rotas[i][j])){
					if(std::get<0>(rotas[i][j+1]) < std::get<0>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][NORTE].write(0);
					}else if(std::get<1>(rotas[i][j+1]) > std::get<1>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][LESTE].write(0);
					}else if(std::get<1>(rotas[i][j+1]) < std::get<1>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][OESTE].write(0);
					}
				}else if(std::get<1>(rotas[i][j-1]) < std::get<1>(rotas[i][j])){
					if(std::get<0>(rotas[i][j+1]) > std::get<0>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][SUL].write(0);
					}else if(std::get<0>(rotas[i][j+1]) < std::get<0>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][NORTE].write(0);
					}else if(std::get<1>(rotas[i][j+1]) > std::get<1>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][LESTE].write(0);
					}
				}else if(std::get<1>(rotas[i][j-1]) > std::get<1>(rotas[i][j])){
					if(std::get<0>(rotas[i][j+1]) > std::get<0>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][SUL].write(0);
					}else if(std::get<0>(rotas[i][j+1]) < std::get<0>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][NORTE].write(0);
					}else if(std::get<1>(rotas[i][j+1]) < std::get<1>(rotas[i][j])){
						set_enables[std::get<0>(rotas[i][j])][std::get<1>(rotas[i][j])][OESTE].write(0);
					}
				}
			}

			if(std::get<0>(rotas[i][(rotas[i].size()-2)]) < std::get<0>(rotas[i][(rotas[i].size()-1)])){
				set_enables[std::get<0>(rotas[i][(rotas[i].size()-1)])][std::get<1>(rotas[i][(rotas[i].size()-1)])][LOCAL].write(0);
			}else if(std::get<0>(rotas[i][(rotas[i].size()-2)]) > std::get<0>(rotas[i][(rotas[i].size()-1)])){
				set_enables[std::get<0>(rotas[i][(rotas[i].size()-1)])][std::get<1>(rotas[i][(rotas[i].size()-1)])][LOCAL].write(0);
			}else if(std::get<1>(rotas[i][(rotas[i].size()-2)]) < std::get<1>(rotas[i][(rotas[i].size()-1)])){
				set_enables[std::get<0>(rotas[i][(rotas[i].size()-1)])][std::get<1>(rotas[i][(rotas[i].size()-1)])][LOCAL].write(0);
			}else if(std::get<1>(rotas[i][(rotas[i].size()-2)]) > std::get<1>(rotas[i][(rotas[i].size()-1)])){
				set_enables[std::get<0>(rotas[i][(rotas[i].size()-1)])][std::get<1>(rotas[i][(rotas[i].size()-1)])][LOCAL].write(0);
			}
		}
	}
}

sc_uint<32> trailer = TRAILER;

void sd::verifica_trailer() { 
	for (int i = 0; i < ALTURA_REDE; ++i) {
		for (int j = 0; j < LARGURA_REDE; ++j) {
			if (noc42->network[i][j]->mux_local->saida == trailer) {
				remove_rota(std::make_tuple(i,j));
				noc42->network[i][j]->mux_local->saida.write(0);
			}	
		}
	}
}