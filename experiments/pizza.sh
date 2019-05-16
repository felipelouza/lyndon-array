#! /bin/sh

mkdir -p results 

for i in ~/pizzachili/*; do 

	test=${i##*/}; 
	date >> results/${test}
	echo "####";
	echo ${test}; 

	for j in 11; do #1 2 4 6 7 8 9 10 11; do 
		echo -A $j; 
		./main $i -A $j >> results/${test};
	done;
done;
