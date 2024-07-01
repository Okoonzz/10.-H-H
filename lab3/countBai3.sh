#!/bin/sh 
count=0
while [ "$count" -le 120 ]
do
echo "$count"
sleep 1
count=$(($count + 1))
done
exit 0 