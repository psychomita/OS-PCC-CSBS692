#!/bin/bash

read -p "Enter a positive integer: " num

if ! [[ "$num" =~ ^[1-9][0-9]*$ ]]; then
    echo "Invalid input!"
    exit 1
fi

sqrt=$(echo "scale=2; sqrt($num)" | bc)

hex=$(printf "%X\n" "$num")

echo "Square root: $sqrt"
echo "Hexadecimal: $hex"
