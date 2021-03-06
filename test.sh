#!/bin/sh

arg_class=kerncalc_arg-class
b=_________________

fail="$b fail"
right="$b success"
echo '+' op 
echo 12 > /sys/class/$arg_class/argument1
echo 23 > /sys/class/$arg_class/argument2
echo + > /dev/operand
res=`cat /dev/result`
if [ "$res" -eq 35 ]
then
	echo $right
else
	echo $fail
fi

echo '-' op 
echo 1 > /sys/class/$arg_class/argument1
echo 15 > /sys/class/$arg_class/argument2
echo - > /dev/operand
res=`cat /dev/result`
if [ "$res" -eq -14 ]
then
	echo $right
else
	echo $fail
fi

echo current arguments retrieving 
res=`cat /sys/class/$arg_class/argument1`
if [ "$res" -eq 1 ]
then
	echo $right
else
	echo $fail
fi

res=`cat /sys/class/$arg_class/argument2`
if [ "$res" -eq 15 ]
then
	echo $right
else
	echo $fail
fi
