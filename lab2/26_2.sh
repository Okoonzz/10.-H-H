#!/bin/sh 

echo -n "Enter your name: "
read user_name
mkdir "$user_name"
echo "Done create $user_name"

while IFS= read -r line
do
mkdir -p "$user_name/$line"
done < "monhoc.txt"