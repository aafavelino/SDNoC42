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
			// cout << "solicitou_rota" << endl;
			deque_pacotes[i].front().solicitou_rota = true;
			solicitacoes_de_rota.push(i);
			deque_pacotes[i].front().contador_idleCycles = 0;
			deque_clock_inicial[i].push_back(clock);
		} 
		if(!deque_pacotes[i].empty() and (deque_pacotes[i].front().possui_rota == true)) {
			// Caso seja o último flit deve-se tirar a flag que diz que possui uma rota
			if (deque_pacotes[i].front().fila_flits.front().data >= TRAILER)
			{
				// cout << "Trailer" << endl;
				deque_pacotes[i].front().possui_rota = false;
				deque_pacotes[i].front().solicitou_rota = false;
			}
			cores[std::get<0>(deque_pacotes[i].front().origem)][std::get<1>(deque_pacotes[i].front().origem)].write(deque_pacotes[i].front().fila_flits.front().data);
			// cout << deque_pacotes[i].front().fila_flits.front().data << endl;
			// cout << "injetou " << deque_pacotes[i].front().fila_flits.front().data << " em ["<< std::get<0>(deque_pacotes[i].front().origem) <<"]["<<std::get<1>(deque_pacotes[i].front().origem)<<"]"<< endl;
			deque_pacotes[i].front().fila_flits.pop();
			if (deque_pacotes[i].front().fila_flits.empty())
			{
				deque_pacotes[i].pop_front();
			}
		}
	}

	cout << noc42->network[0][1]->mux_local->saida<<" " << noc42->network[1][1]->mux_local->saida << " "<<noc42->network[2][2]->mux_local->saida<< " "<<noc42->network[0][3]->mux_local->saida<<  endl;
	
}

void sd::solicita_rota() {
	// cout<< sc_time_stamp()<<endl;
	if (!solicitacoes_de_rota.empty())
	{	

		// cout << "SAIDA mux_leste "<< noc42->network[0][0]->mux_leste->saida << endl;
		// cout << solicitacoes_de_rota.size() << endl;
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

	// cout << "XY"<< endl;

	std::vector<tuple<int, int> > rota;

	// cout << rota.size()<< endl;
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
			// rota.erase (rota.begin(), rota.begin()+rota.size());
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
	// cout << "AQUIIII" << endl;
	return true;
}

void sd::remove_rota(tuple<int, int> destino) {
	cout << "remove_rota"<<endl;
	for (int i = 0; i < rotas.size(); ++i) {
		if (rotas[i].back() == destino) {

			if(std::get<0>(rotas[i][1]) > std::get<0>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][SUL].write(0);
				// cout << std::get<0>(rotas[i][0]) << " " << std::get<1>(rotas[i][0])<< "SUL" <<endl;
			}else if(std::get<0>(rotas[i][1]) < std::get<0>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][NORTE].write(0);
				// cout << std::get<0>(rotas[i][0]) << " " << std::get<1>(rotas[i][0])<< "NORTE" << endl;
			}else if(std::get<1>(rotas[i][1]) > std::get<1>(rotas[i][0])){
				// noc42->network[0][0]->mux_leste->saida = 0;
				// noc42->network[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])]->mux_leste->saida.write(0);
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][LESTE].write(0);
				// cout << std::get<0>(rotas[i][0]) << " " << std::get<1>(rotas[i][0])<< "LESTE" << endl;
			}else if(std::get<1>(rotas[i][1]) < std::get<1>(rotas[i][0])){
				set_enables[std::get<0>(rotas[i][0])][std::get<1>(rotas[i][0])][OESTE].write(0);
				// cout << std::get<0>(rotas[i][0]) << " " << std::get<1>(rotas[i][0])<< "OESTE" << endl;
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
		// cout << "rotas " <<  rotas[i].size() << endl;
			// Zerar a rota que foi liberada
			// cout << std::get<0>(rotas[i].front())<<" " << std::get<1>(rotas[i].front()) <<" > " <<  std::get<0>(rotas[i].back())<<" "<< std::get<1>(rotas[i].back()) << endl;
			// cout << (rotas[i].back() == destino) << endl;
			// cout << "ROTAS " << rotas.size() <<endl;
			// rotas[i].erase (rotas[i].begin(),rotas[i].begin()+rotas[i].size());
			rotas.erase (rotas.begin()+i);
			
			// cout << "ROTAS " << rotas.size() <<endl;

			// cout << std::get<0>(rotas[i].front())<<" " << std::get<1>(rotas[i].front()) <<" > " <<  std::get<0>(rotas[i].back())<<" "<< std::get<1>(rotas[i].back()) << endl;
			// cout << (rotas[i].back() == destino) << endl;
		// cout << "rotas " <<  rotas[i].size() << endl;			
		}

	}
}

sc_uint<32> trailer = TRAILER;

void sd::verifica_trailer() { 
	for (int i = 0; i < ALTURA_REDE; ++i) {
		for (int j = 0; j < LARGURA_REDE; ++j) {
			if (noc42->network[i][j]->mux_local->saida >= trailer) {
				sc_uint<32> id_pacote = noc42->network[i][j]->mux_local->saida;
				deque_clock_final[(id_pacote - trailer)].push_back(clock);
				remove_rota(std::make_tuple(i,j));
				noc42->network[i][j]->mux_local->saida.write(0);
				if (rotas.empty() && solicitacoes_de_rota.empty())
					sc_stop();
				
			}	
		}
	}
}