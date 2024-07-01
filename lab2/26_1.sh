#!/bin/sh 
user_dir=$HOME
if [ ! -d "$user_dir/PNG" ]; then
mkdir "$user_dir/PNG"
echo "Done create PNG folder."
else
echo "PNG has been created."
fi

if [ ! -d "$user_dir/JPG" ]; then
mkdir "$user_dir/JPG"
echo "Done create JPG folder."
else
echo "JPG has been created."
fi

sleep 5

mv "OS_LAB2_IMG"/*.png "$user_dir/PNG/"
countPNG=$(ls -l $user_dir/PNG/*.png 2> /dev/null | wc -l)
echo "PNG folder has $countPNG items."

mv "OS_LAB2_IMG"/*.jpg "$user_dir/JPG/"
countJPG=$(ls -l $user_dir/JPG/*.jpg 2> /dev/null | wc -l)
echo "JPG folder has $countJPG items."

exit 0

