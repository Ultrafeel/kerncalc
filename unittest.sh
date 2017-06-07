#!/bin/sh

arg_class=kerncalc_arg-class
echo + op 
echo 12 > /sys/class/$arg_class/argument1
echo 23 > /sys/class/$arg_class/argument2
echo + > /dev/operand
res=`cat /dev/result`
if [ "$res" -eq 35 ]
then
	echo right
else
	echo wrong
fi

echo - op 
echo 1 > /sys/class/$arg_class/argument1
echo 15 > /sys/class/$arg_class/argument2
echo - > /dev/operand
res=`cat /dev/result`
if [ "$res" -eq -14 ]
then
	echo right
else
	echo wrong
fi
