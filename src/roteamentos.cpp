//
// Created by Adelino, Raul and Yuri on 11/11/18.
//
#include "sd.h"


bool sd::dijkstra(tuple<int, int> origem, tuple<int, int> destino, int pos_solicitante) { 
    

	for(int i = 0; i < rotas.size(); i++){
		if(rotas[i][0] == origem){
			return false;
		}

		if(rotas[i][rotas[i].size()-1] == destino){
			return false;
		}
	}

	std::vector<tuple<int, int> > rota;
	int src = tupleToInt(origem);
	int tgt = tupleToInt(destino);
    int dist[V];     
    bool visitados[V]; 
    int prev[V];
    
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX; 
        visitados[i] = false; 
        prev[i] = -1;  
    }

    dist[src] = 0; 
   

   	for (int count = 0; count < V-1; count++) { 
       
       	int u = min_distancia(dist, visitados); 

       	visitados[u] = true; 
       
       for (int v = 0; v < V; v++) 
         if (!visitados[v] && grafo_de_rotas[u][v] && dist[u] != INT_MAX && dist[u]+grafo_de_rotas[u][v] < dist[v]) {
            	dist[v] = dist[u] + grafo_de_rotas[u][v]; 
        		prev[v] = u;
        		
        		if (v == tgt) {
	 	 			v = tgt;
  					if (prev[v] != -1 or v == src) {          
      				while (v != -1){
          				rota.push_back(intToTuple(v));
          				v = prev[v];
          			}
          			std::reverse(rota.begin(), rota.end());
          		}
      			break;
   			}
        }
     } 
      
     if (rota.size() == 0)
     {
     	return false;
     }


	for (int i = 0; i < (rota.size()-1); ++i) {
		if (grafo_de_rotas[((std::get<0>(rota[i])*ALTURA_REDE)+(std::get<1>(rota[i])))][((std::get<0>(rota[i+1])*ALTURA_REDE)+(std::get<1>(rota[i+1])))] != 1) {
			return false;
		}
	}

	close_circuit(rota);
	rotas.push_back(rota);
	solicitacoes_de_rota.pop();
	deque_pacotes[pos_solicitante].front().possui_rota = true;
	return true;    
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
			return false;
		}
		
	}

	close_circuit(rota);
	rotas.push_back(rota);
	solicitacoes_de_rota.pop();
	deque_pacotes[pos_solicitante].front().possui_rota = true;
	return true;
}



bool sd::roteamento_west_first(tuple<int, int> origem, tuple<int, int> destino, int pos_solicitante) {

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

		if (std::get<0>(destino) < x_atual)
		{
			x_atual--;
		} else if (std::get<0>(destino) > x_atual)
		{
			x_atual++;
		} else {
			if (y_atual < std::get<1>(destino)) {
				y_atual++;
			} else if (y_atual > std::get<1>(destino)) {
				y_atual--;
			} else {
				break;
			} 			
		}

		rota.push_back(std::make_tuple (x_atual,y_atual));
	}



	for (int i = 0; i < (rota.size()-1); ++i) {

		if (grafo_de_rotas[((std::get<0>(rota[i])*ALTURA_REDE)+(std::get<1>(rota[i])))][((std::get<0>(rota[i+1])*ALTURA_REDE)+(std::get<1>(rota[i+1])))] != 1) {
			return false;
		}
		
	}

	close_circuit(rota);
	rotas.push_back(rota);
	solicitacoes_de_rota.pop();
	deque_pacotes[pos_solicitante].front().possui_rota = true;
	return true;
}
