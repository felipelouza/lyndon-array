#! /bin/sh

mkdir -p results 

#for i in /mnt/data/bio/projects/gpt/text/pizzachili/*; do 
for i in /mnt/data/bio/projects/gpt/text/pizzachili/english.txt; do 

	test=${i##*/}; 
	date >> results/${test}
	echo "####";
	echo ${test}; 

	for j in 11; do #1 2 4 6 7 8 9 10 11; do 
		echo -A $j; 
#		./main $i -A $j >> results/${test};
		./main $i -A $j -d 21894765 >> results/${test};
	done;
done;
