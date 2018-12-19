//
// Created by Adelino, Raul and Yuri on 13/11/18.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <queue>
#include <ctime>    
#include <stdlib.h>  
#include <time.h>   
#include <systemc.h>
#include <../constantes/constantes.h>
#include "sd.h"

using namespace std;


std::function<int (int, int)> func = [](int i, int j) {
    return rand() % i + j;
};

int sc_main (int argc, char* argv[]) {

    std::srand (unsigned(std::time(0)));

    // Instância do software defined
    sd *software_defined = new sd("SD");
    // Geração de sinal de clock para métodos sensíveis a mudança de clock
    sc_clock clock("clock", 1, SC_NS, 1, 1, SC_NS);
    software_defined->Clk(clock);


    double **padrao_tfg;
    int size_pct = 0;
    int size_fluxo = 0;

    std::vector<std::tuple<int, int> > map_roteadores;

    for(int i = ALTURA_REDE - 1; i >= 0; i--){
        for(int j = 0; j < LARGURA_REDE; j++){
            map_roteadores.push_back(std::make_tuple(i,j));
        }
    }

    int qtd_de_roteadores = ALTURA_REDE * LARGURA_REDE;
    int qtd_de_fluxos = 240;
    std::vector<int> roteadores;

    // Leitura do Arquivo de tráfego
    FILE *traffic;
    traffic = fopen("traffic.tcf","r");


    padrao_tfg = (double**)malloc(qtd_de_fluxos * sizeof(double*));

    for (int i = 0; i < qtd_de_fluxos; i++) 
    padrao_tfg[i] = (double*) malloc(15 * sizeof(double));



    int indice = 0;



    for (int j = 0; j < qtd_de_roteadores; ++j){
        fscanf(traffic,"\ntg_%i",&size_pct);
        fscanf(traffic,"\n%i",&size_fluxo);    
        roteadores.push_back(size_fluxo);

        for (int i = 0; i < roteadores[j]; ++i,indice++){
            fscanf(traffic,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &padrao_tfg[indice][0], &padrao_tfg[indice][1], &padrao_tfg[indice][2], &padrao_tfg[indice][3], &padrao_tfg[indice][4], &padrao_tfg[indice][5], &padrao_tfg[indice][6], &padrao_tfg[indice][7], &padrao_tfg[indice][8], &padrao_tfg[indice][9], &padrao_tfg[indice][10], &padrao_tfg[indice][11], &padrao_tfg[indice][12], &padrao_tfg[indice][13], &padrao_tfg[indice][14]);
        }   
    }

    fclose(traffic);


    /**
     * Aqui é feito o sorteio dos fluxos para o padrão uniforme.
     * caso queira fazer o sorteio sete o primeiro argumento para
     * 1 e os respectivos argumentos serão a largura e altura da NoC.
     */
    if (atoi(argv[1]) == 1) {
        int fluxos = atoi(argv[2]) * atoi(argv[3]);
        int value = fluxos-1;
        int topo = value;
        for (int i = 0; i < fluxos; ++i) {
            for (int j = i*value; j < topo; ++j) {
                swap(padrao_tfg[j],padrao_tfg[(j + (rand() % (int)((topo-1) - j + 1)))]);
            }
            topo += value;
        }
    }

    int foda = 0;
    int x_ant = std::get<0>(map_roteadores[0]);
    int y_ant = std::get<1>(map_roteadores[0]);
    int contador = 0;
    int posicao = 0;
    std::deque<pacote> deque_auxiliar;
    std::deque<long long int> deque_auxiliar_clock_inicial;
    std::deque<long long int> deque_auxiliar_clock_final;

    for (int j = 0; j < qtd_de_roteadores; ++j){   
        for (int i = 0; i < roteadores[j]; ++i,indice++){
            deque_auxiliar.push_back(pacote(std::get<0>(map_roteadores[j]),std::get<1>(map_roteadores[j]), std::get<0>(map_roteadores[padrao_tfg[foda][1]]), std::get<1>(map_roteadores[padrao_tfg[foda][1]]),  padrao_tfg[foda][8],  padrao_tfg[foda][5],  padrao_tfg[foda][9],j));
            foda++;    
        }
        if (roteadores[j] != 0) {
            software_defined->deque_pacotes.push_back(deque_auxiliar);
            deque_auxiliar.erase(deque_auxiliar.begin(),deque_auxiliar.end());
        }
        software_defined->deque_clock_inicial.push_back(deque_auxiliar_clock_inicial);
        software_defined->deque_clock_final.push_back(deque_auxiliar_clock_final);
    }

    // Roda a simulação até encontrar um sc_stop();
    sc_start(); 


    double media = 0;

    for (int i = 0; i < software_defined->deque_clock_inicial.size(); ++i) {
        for (int j = 0; j < software_defined->deque_clock_inicial[i].size(); ++j) {
            media += software_defined->deque_clock_inicial[i][j];
        }
    }

    int divisao_interna = 0;
    for (int i = 0; i < software_defined->deque_clock_inicial.size(); ++i) {
        divisao_interna += software_defined->deque_clock_inicial[i].size();
    }

    cout << "Latência média da simulação " << media/divisao_interna << endl;
    return 0;
}