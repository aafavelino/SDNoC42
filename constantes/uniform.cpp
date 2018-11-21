#include <iostream>
#include <tuple>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{

    std::tuple<int, int> origem;
    std::tuple<int, int> destino;

    int flits = atoi(argv[1]);
    int pacotes = atoi(argv[2]);
    int idlecycles = atoi(argv[3]);

    int tam_rede = atoi(argv[4]);

    ofstream trafego ("../traffic.txt");
    trafego << "tg " << (tam_rede*tam_rede*tam_rede*tam_rede)-(tam_rede*tam_rede) << endl;

	for (int i = 0; i < tam_rede; ++i)
	{
		for (int j = 0; j < tam_rede; ++j)
		 {
		 	for (int k = 0; k < tam_rede; ++k)
		 	{
		 		for (int l = 0; l < tam_rede; ++l)
		 		{

		 			origem = std::make_tuple (i,j);
		 			destino = std::make_tuple (k,l);
		 			if (origem != destino) 			
		 				trafego << i <<" " << j << " " << k << " " << l << " " << flits <<" " << pacotes << " " << idlecycles << endl;
		 		}
		 	}
		 } 
	}


	trafego << endl << endl << endl <<  "Modelo de tráfego" << endl
	<< "0 0 2 0 23 1 10" << endl << endl 
	<< "Mesh padrão: " << endl
 	<< "xs ys xd yd qtd_flits qtd_pcts idleCycles "<< endl
 	<< "xs: X source;"<< endl
 	<< "xd: X dest"<< endl
 	<< "xs: Y source; "<< endl
 	<< "xd: Y dest "<< endl
 	<< "qtd_flits : Quantos flits haverá dentro de cada pacote"<< endl
 	<< "qtd_pcts : Quantos pacotes serão enviados por fluxo"<< endl
 	<< "idleCycles: De quanto em quanto tempo é enviado" << endl;

 
	return 0;
}