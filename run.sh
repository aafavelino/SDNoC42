echo "RT0 - Signalling - 10 flits - 10 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 3x3"


cd constantes/
./uniform 10 10 8 3
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 3 3
	sleep 1
done


echo "RT0 - Signalling - 10 flits - 100 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 3x3"

cd constantes/
./uniform 10 100 8 3
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 3 3
	sleep 1
done


echo "RT0 - Signalling - 10 flits - 10 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 4x4"

cd constantes/
./uniform 10 10 8 4
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 4 4
	sleep 1
done


echo "RT0 - Signalling - 10 flits - 100 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 4x4"

cd constantes/
./uniform 10 100 8 4
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 4 4
	sleep 1
done


echo "\n\n"



echo "RT0 - Signalling - 10 flits - 20 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 3x3"


cd constantes/
./uniform 10 20 8 3
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 3 3
	sleep 1
done


echo "RT0 - Signalling - 10 flits - 110 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 3x3"

cd constantes/
./uniform 10 110 8 3
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 3 3
	sleep 1
done


echo "RT0 - Signalling - 10 flits - 20 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 4x4"

cd constantes/
./uniform 10 20 8 4
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 4 4 
	sleep 1
done


echo "RT0 - Signalling - 10 flits - 110 pcts - 10% da largura de banda requerida - 8 Idlecycles - NOC 4x4"

cd constantes/
./uniform 10 110 8 4
cd ../
for (( j = 0; j < 30; j++ )); do
	./bin/exe 1 4 4 
	sleep 1
done





