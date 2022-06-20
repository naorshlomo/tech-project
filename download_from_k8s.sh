#!/bin/bash
rm -rf workers_results/
mkdir workers_results
NUMBER_OF_ROUNDS=100
HOW_MANY=75
for (( VARIABLE=0; VARIABLE<$HOW_MANY; VARIABLE++ ))
do
  echo $VARIABLE
    kubectl cp default/worker-envars-fieldref-statefulset-${VARIABLE}:worker-envars-fieldref-statefulset-${VARIABLE}_res workers_results/worker-envars-fieldref-statefulset-${VARIABLE}_res.csv &
done
x=0
while [ $x -le $HOW_MANY ]
do
  # shellcheck disable=SC2012
  x=$(ls -l workers_results/ | wc -l)
done
python3 parser.py $HOW_MANY $NUMBER_OF_ROUNDS