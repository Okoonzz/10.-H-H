#!/bin/sh

check(){
    if [ $1 -lt 10 ]; then
    echo 1
    else
    echo 0
    fi
}

sumN(){
    n=$1
    sum=0
    i=1
    while [ $i -le $n ]
    do
    sum=$(($sum+$i))
    i=$(($i+1))
    done
    echo $sum
}

while true
do
echo "Enter number: "
read n
if [ $(check $n) -eq 1 ]; then
echo "The number must be greater than 10. Enter again"
else
sum=$(sumN $n)
echo "Your sum: $sum"
exit 0
fi
done
