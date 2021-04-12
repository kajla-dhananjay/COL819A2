#!/bin/bash
testcase=0
tot=50
while [ $testcase -lt $tot ]
do
  ((testcase++))
  rm -rf *.txt
  g++ input_input_generator.cpp -o input_input_generator
  ./input_input_generator > generator_input.txt
  ./input_generator < generator_input.txt > input.txt
  ./kruskal < input.txt > out_kruskal.txt
  echo "Kruskal Done"
  ./ghs < input.txt > out_ghs.txt
  echo "GHS Done"
  N=$(cat generator_input.txt | cut -d ' ' -f1) 
  P=$(cat generator_input.txt | cut -d ' ' -f2) 
  DIFF=$(diff out_ghs.txt out_kruskal.txt)
  if [ "$DIFF" == "" ]; then
    echo "Testcase : $testcase | N : $N | P : $P | Result = PASS"
  else 
    echo "Graph : "
    cat input.txt
    echo "Kruskal Output : "
    cat out_kruskal.txt
    echo "GHS Output : "
    cat out_ghs.txt
    echo "Diff : "
    diff out_ghs.txt out_kruskal.txt 
    echo "Testcase : $testcase | N : $N | P : $P | Result = FAIL"
    exit 1
  fi
done
