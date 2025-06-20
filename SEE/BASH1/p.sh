#!/bin/bash

# Create student.dat
cat > student.dat <<EOF
alice:104:9876543210:Kolkata
bob:102:9123456789:Mumbai
charlie:103:9000012345:Delhi
david:101:9321456780:Chennai
alice:105:9988776655:Bangalore
EOF

# (i) Sort by roll number in reverse
sort -t ':' -k2,2r student.dat > sorted.dat
echo "Sorted roll numbers saved to sorted.dat"

# (ii) Convert to uppercase using redirection
tr 'a-z' 'A-Z' < student.dat > uppercase_student.dat
echo "Uppercase conversion saved to uppercase_student.dat"

# (iii) Find unique names
echo "Unique names:"
cut -d ':' -f1 student.dat | sort | uniq
