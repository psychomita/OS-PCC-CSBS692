#!/bin/bash
while [ 1 ]
  echo "MENU:"
  echo "1.Check if file is regular or special"
  echo "2.number of files and directories under current directory"
  echo "3.show last modification time of a file"
  echo "4.exit"
do
  read -p "Enter your choice: " choice
  case $choice in
  1)
    read -p "Enter the filename: " filename
            if [ -e "$filename" ]; then
                if [ -f "$filename" ]; then
                    echo "$filename is a regular file."
                elif [ -b "$filename" ]; then
                    echo "$filename is a block special file."
                elif [ -c "$filename" ]; then
                    echo "$filename is a character special file."
                elif [ -s "$filename" ]; then
                    echo "$filename is a special file (non-zero size)."
                else
                    echo "$filename is a special file."
                fi
            else
                echo "Error: File does not exist."
            fi
            ;;
  2)
    files=$(ls -l | grep "^-" | wc -l)
    directory=$(ls -l | grep "^d" | wc -l)
    echo "The files existing under current directory are: $files"
    echo "The directiories existing under current directory are: $directory"
    ;;
  3)
    read -p "Enter the filename: " filename
    if [ -e $filename ]; then
      modify=$(ls -l | grep "$filename" | tr -s " " | cut -d" " -f8)
      echo "The last modification time of $filename in current directory is $modify"
    else
      echo "$filename doesnot exist under current directory."
    fi
    ;;
  4)
    echo "Program exited"
    exit
    ;;
  *)
    echo "Invalid choice"
    ;;
  esac
done
