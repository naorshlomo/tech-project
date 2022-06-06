#!/bin/bash
HOW_MANY=5
for (( VARIABLE=0; VARIABLE<$HOW_MANY; VARIABLE++ ))
do
  echo $VARIABLE
    kubectl cp default/worker-envars-fieldref-statefulset-${VARIABLE}:worker-envars-fieldref-statefulset-${VARIABLE}_res workers_results/worker-envars-fieldref-statefulset-${VARIABLE}_res.csv
done
python3 parser.py $HOW_MANY