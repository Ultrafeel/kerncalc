#!/bin/bash

arg_class=kerncalc_arg-class
echo 12 > /sys/class/$arg_class/argument1
echo 23 > /sys/class/$arg_class/argument2
echo + > /dev/operand/
res=`cat /dev/result`
if [ "$res" -eq 35 ]
then
	echo all right
else
	echo wrong
fi
