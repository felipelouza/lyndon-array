#! /bin/sh

dir="remap-results"

D="pizzachili"
ext=(".txt")

mkdir -p ${dir}

A=("aaa.10MB" "aaab" "aaa" "cere" "dblp" "dna" "einstein-de" "english.1GB" "fib41" "kernel" "proteins" "sources")

for i in {0..11}
do

  test=${A[$i]}
  echo "####";
  echo ${test};
  date >> ${dir}/${test}.out
  echo "####";

  j = 0

  for a in {0..6}
  do
    echo ${D}/${A[$i]}${ext[$j]}
    echo "./remap -A $a  ${D}/${A[$i]}${ext[$j]}" >> remap-results/${test}.out;
    ./remap -A $a  ${D}/${A[$i]}${ext[$j]} >> remap-results/${test}.out;
  done;

done;
