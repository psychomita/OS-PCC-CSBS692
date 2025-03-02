#!/bin/bash

while true; do
    echo "\nMenu-Driven Integer Calculator"
    echo "1. Addition"
    echo "2. Subtraction"
    echo "3. Multiplication"
    echo "4. Division"
    echo "5. Modulo"
    echo "6. Exit"
    read -p "Enter your choice: " choice

    if [ "$choice" -eq 6 ]; then
        echo "Exiting the calculator."
        exit 0
    fi

    read -p "Enter first number: " num1
    read -p "Enter second number: " num2

    case $choice in
        1) result=$((num1 + num2))
           echo "Result: $num1 + $num2 = $result" ;;
        2) result=$((num1 - num2))
           echo "Result: $num1 - $num2 = $result" ;;
        3) result=$((num1 * num2))
           echo "Result: $num1 * $num2 = $result" ;;
        4) if [ "$num2" -eq 0 ]; then
               echo "Error: Division by zero is not allowed."
           else
               result=$((num1 / num2))
               echo "Result: $num1 / $num2 = $result"
           fi ;;
        5) if [ "$num2" -eq 0 ]; then
               echo "Error: Modulo by zero is not allowed."
           else
               result=$((num1 % num2))
               echo "Result: $num1 % $num2 = $result"
           fi ;;
        *) echo "Invalid choice. Please try again." ;;
    esac

done

