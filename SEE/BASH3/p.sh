#!/bin/bash

while true; do
    echo ""
    echo "====== File Handling Menu ======"
    echo "1. Copy File"
    echo "2. Remove File"
    echo "3. Rename File"
    echo "4. Create Link to File"
    echo "5. Exit"
    echo "================================"
    read -p "Enter your choice [1-5]: " choice

    case $choice in
        1)
            read -p "Enter the source file to copy: " src
            if [ -e "$src" ]; then
                read -p "Enter the destination file name: " dest
                cp "$src" "$dest"
                echo "File copied from $src to $dest"
            else
                echo "Source file does not exist!"
            fi
            ;;
        2)
            read -p "Enter the file to remove: " file
            if [ -e "$file" ]; then
                rm "$file"
                echo "File '$file' removed successfully."
            else
                echo "File does not exist!"
            fi
            ;;
        3)
            read -p "Enter the current file name: " oldname
            if [ -e "$oldname" ]; then
                read -p "Enter the new file name: " newname
                mv "$oldname" "$newname"
                echo "File renamed from $oldname to $newname"
            else
                echo "File does not exist!"
            fi
            ;;
        4)
            read -p "Enter the file to link: " target
            if [ -e "$target" ]; then
                read -p "Enter the name of the link to be created: " linkname
                ln -s "$target" "$linkname"
                echo "Symbolic link '$linkname' created for file '$target'"
            else
                echo "Target file does not exist!"
            fi
            ;;
        5)
            echo "Exiting..."
            break
            ;;
        *)
            echo "Invalid choice. Please select between 1-5."
            ;;
    esac
done
