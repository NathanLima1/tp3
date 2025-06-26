echo "\n\nShift-And\n\n" >> resultados.txt

./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 2 -k 0 >> resultados.txt
./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 2 -k 1 >> resultados.txt
./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 2 -k 2 >> resultados.txt
./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 2 -k 3 >> resultados.txt

echo "\n\nLevenshtein\n\n" >> resultados.txt

./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 1 -k 0 >> resultados.txt
./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 1 -k 1 >> resultados.txt
./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 1 -k 2 >> resultados.txt
./tp3_parte1 -a entrada1.txt -b entrada2.txt -o saida_shift_and.txt -f 1 -k 3 >> resultados.txt