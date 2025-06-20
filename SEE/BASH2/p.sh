#!/bin/bash

# Clear or create the file
> marks.txt

# Input loop
while true; do
    read -p "Enter student name: " name
    read -p "Enter title (subject): " title
    read -p "Enter marks: " mark

    echo "$name|$title|$mark" >> marks.txt

    read -p "Do you want to add another record? (y/n): " choice
    if [ "$choice" != "y" ]; then
        break
    fi
done

# Find highest mark and student
highest_line=$(sort -t '|' -k3 -nr marks.txt | head -n 1)

name=$(echo "$highest_line" | cut -d '|' -f1)
title=$(echo "$highest_line" | cut -d '|' -f2)
mark=$(echo "$highest_line" | cut -d '|' -f3)

echo
echo "Highest Scorer:"
echo "Name: $name"
echo "Title: $title"
echo "Marks: $mark"
