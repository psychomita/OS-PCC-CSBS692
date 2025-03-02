#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Error: No numbers provided. Please pass the numbers as command-line arguments."
  exit 1
fi

sum=0

for num in "$@"; do
  # Check if the argument is a valid number
  if [[ ! "$num" =~ ^[0-9]+$ ]]; then
    echo "Error: '$num' is not a valid number. Only positive integers are allowed."
    exit 1
  fi
  sum=$((sum + num))
done

echo "The sum of the given numbers is: $sum"

