#! /bin/sh

mkdir -p recursive

for i in ~/pizzachili/*; do 

        input=${i##*/};
        date >> recursive/${input}
        echo "####";
        echo ${input};

        for j in 0 1 2 3 4 5; do #1 2 4 6 7 8 9 10 11; do 
                echo "./test $i -P $j"
                ./recursive.o $i -P $j  >> recursive/${input};
        done;
done;

