#!/bin/bash

# Read basic salary
read -p "Enter the basic salary: " basic

# Validate numeric input
if ! [[ "$basic" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
    echo "Invalid input. Please enter a numeric value."
    exit 1
fi

# Calculate gross salary
if (( $(echo "$basic < 1500" | bc -l) )); then
    hra=$(echo "0.10 * $basic" | bc)
    da=$(echo "0.90 * $basic" | bc)
else
    hra=500
    da=$(echo "0.98 * $basic" | bc)
fi

gross=$(echo "$basic + $hra + $da" | bc)

# Output
echo "Basic Salary  : ₹$basic"
echo "HRA           : ₹$hra"
echo "DA            : ₹$da"
echo "---------------------------"
echo "Gross Salary  : ₹$gross"
