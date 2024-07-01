#!/bin/sh

echo "Pleae enter your studentID: "
while true
do
read mssv
if [ "$mssv" -eq "22520167" ]; then
echo "$mssv"
break
else
echo "Enter again"
fi
done
exit 0