#!/bin/bash

read -p "Enter username: " user

count=$(grep -c -i $user /etc/passwd)

if [ $count -ne 0 ]; then
	echo "VALID USERNAME"
else
	echo "INVALID USERNAME"
fi
