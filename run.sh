echo "#define NORTE 0
#define SUL 1
#define LESTE 2
#define OESTE 3
#define LOCAL 4

// Dimensões da Rede
#define ALTURA_REDE 4
#define LARGURA_REDE 4


// Flit terminador
#define TRAILER 4000000000


// Quantidade de núcleos mestres
#define NUCLEOS 2


// Tipo de roteamento 
// 0 XY
// 1 djikstra
// 2 west first
// 3 dfs
#define TIPO_ROTEAMENTO 3

// Topologia
// 0 Mash 2D
// 1 Torus 2D
//
#define TOPOLOGIA 1" > constantes/constantes.h

#g++ -std=c++11 -o bin/exe src/main.cpp src/buffer.cpp src/roteador.cpp src/roteamentos.cpp src/sd.cpp src/mux.cpp -I include/ -I/Users/adelinofernandes/Documents/systemc-2.3.1/include -L/Users/adelinofernandes/Documents/systemc-2.3.1/lib-macosx64 -lsystemc
g++ -std=c++11 -o bin/exe src/main.cpp src/buffer.cpp src/roteador.cpp src/roteamentos.cpp src/sd.cpp src/mux.cpp -I include/ -I/usr/local/systemc-2.3.1/include -L/usr/local/systemc-2.3.1/lib-linux64 -Wl,-rpath=/usr/local/systemc-2.3.1/lib-linux64 -lsystemc
cd constantes/
./uniform 10 100 4 4
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 4 4 
	sleep 1
done

