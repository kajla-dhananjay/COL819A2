#!/bin/bash
while [ $? -eq 0 ] 
do
  rm -rf *.txt
  echo "Yo"
  ./ghs < failcase2 > tmp.txt
done
