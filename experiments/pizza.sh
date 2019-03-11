#! /bin/sh

#sources
#A=(7065006 4210907 1866 43894765 1 545356 1 9550966 296135875)
A=(7065006 4210907 1866 21894765 1 545356 1 9550966 7619949)
D=("sources" "proteins" "dna" "english" "cere" "einstein" "fib41" "kernel" "dblp")

host="shy"
dir="/mnt/disk1/louza/pizzachili"
check=1
exp="time"

#######################################################################

mkdir -p pizzachili
cp lib/utils_32.h lib/utils.h -f
make clean
make

for i in {0..8}
do

        test=${D[$i]}
        echo "${test}"

        for j in 2
        do
#               echo "MODE ${j}"

                mkdir -p pizzachili/${test}/
                date >> pizzachili/${test}/${host}.db.${exp}.txt

                echo " " >> pizzachili/${test}/${host}.db.${exp}.txt
                make run DIR=${dir} INPUT=${test}.txt K=${A[$i]} CHECK=${check} MODE=$j >> pizzachili/${test}/${host}.db.${exp}.txt
        done

done


#########################################################################

