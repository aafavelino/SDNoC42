# SD Network on chip 42  

## Como executar o simulador?
### Instalando e configurando o Systemc
Primeiramente você precisa ter o systemc instalado em sua máquina, você pode encontrar em [accellera.org](http://www.accellera.org/downloads/standards/systemc). Siga as instruções de instalação do site.

Caso não tenha conseguido esse vídeo é bem didático: https://www.youtube.com/watch?v=rLBScPm_bis

Após a instalação, sete a variável de ambiente SC_SIGNAL_WRITE_CHECK para DISABLE no bash_profile 
````export SC_SIGNAL_WRITE_CHECK=DISABLE````

### Compilando a NOC
Existe um arquivo com um exemplo de como compilar tanto no mac quanto no linux. Em suma é só um exemplo pois pode variar de acordo com o modo de como você instalou o systemc. O arquivo é o ~~**compiler.sh**~~.

Em suma lá você poderá encontrar os arquivos.cpp que devem ser compilados... Sugiro que coloque a flag -std=c++11 pois utilizamos algumas funcionalidades desta versão. 

## Executando uma simulação

 1. Você pode definir o Algoritmo de Roteamento (Que por enquanto temos o XY baseado na busca em profundidade DFS e o algoritmo de Dijkstra)
 2. Em traffic.txt é definido o padrão de tráfego (Que você pode gerar o tráfego pelo uniform.cpp ou setar manualmente... Ainda não implementamos o gerador de todos os padrões) Você também pode gerar o padrão manualmente pela ferramenta REDSCARF.
 3. Por fim é só executar o arquivo gerado ao compilar a noc. 

 A execução é feita da seguinte forma ````./bin/compilado 1 4 4 ````

 O primeiro argumento é referênte ao sorteio dos fluxos, caso queira sortear sete 1 senão, sete 0. Obs.: Só sorteie se utilizar o padrão uniforme.

 Os outros dois argumentos são referêntes às dimensões da NoC, esse do exemplo é para uma NoC Mesh 4x4.

 Para mais informações entre em contato com Adelino Afonso no email abaixo ou pelo telefone 084 986.348.534.


### Projeto de Pesquisa em Redes em chip

|  Alunos                       | 		Email			  |			 			Curso				  |
|-------------------------------|-------------------------|-------------------------------------------|
| Adelino Avelino			    | aafavelino@icloud.com   | Tecnologia da Informação BTI-UFRN		  |
| Raul Silveira 				| raul95@gmail.com        | Mestrado Sistemas e Computação PPGSC-UFRN |
| Yuri Reinaldo 				| yuri.wrlk@gmail.com     | Tecnologia da Informação BTI-UFRN		  |



|  Professores                  |             	Email				|
|-------------------------------|-----------------------------------|
|  Marcio Eduardo Kreutz		|  kreutz@dimap.ufrn.br             |
|  Silvia Maria Diniz M. Maia	|  silvia@dimap.ufrn.br 			|
|  Departamento 				|  DIMAp - UFRN 					|



