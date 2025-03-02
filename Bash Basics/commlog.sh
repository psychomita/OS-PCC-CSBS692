#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Error: No argument provided. Please provide an integer."
  exit 1
fi

N=$1

if ! [[ "$N" =~ ^[0-9]+$ ]] || [ "$N" -le 0 ]; then
  echo "Error: Please provide a valid positive integer."
  exit 1
fi

log_value=$(echo "scale=4; l($N)/l(10)" | bc -l)

rounded_value=$(printf "%.2f" $log_value)

echo "The common logarithm (log base 10) of $N is: $rounded_value"

