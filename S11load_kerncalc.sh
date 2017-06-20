#!/bin/bash
echo ====my load script arg=$1========
if [ "$1" = "start" ]
then
	echo "start"
    insmod /lib/modules/`uname -r`/kerncalc.ko
fi

