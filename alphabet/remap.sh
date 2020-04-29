#! /bin/sh

dir="remap-results"

D="pizzachili"
ext=(".txt" ".freq.most.txt" ".freq.less.txt" ".rle.most.txt" ".rle.less.txt")

mkdir -p ${dir}

A=("aaa.10MB" "aaab" "aaa" "cere" "dblp" "dna" "einstein-de" "english.1GB" "fib41" "kernel" "proteins" "sources")

for i in {0..11}
do

  test=${A[$i]}
  echo "####";
  echo ${test};
  date >> ${dir}/${test}.out
  echo "####";

  for j in {3..4}
  do
    echo ${D}/${A[$i]}${ext[$j]}
    echo "./lyndon -A 2 -f ${D}/${A[$i]}${ext[$j]}" >> remap-results/${test}.out;
    ./lyndon -A 2 -s  -f ${D}/${A[$i]}${ext[$j]} >> remap-results/${test}.out;
  done;

done;
