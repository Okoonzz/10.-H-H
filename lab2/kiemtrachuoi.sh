#!/bin/sh
echo -n "Enter your string: "
read input
if grep "$input" test1.txt > /dev/null; then
echo "Your input '$input' is in test1.txt"
else
echo "Your input '$input' is not in test1.txt"
fi
exit 0