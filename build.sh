#!/bin/bash

mkdir bin
echo "building project\n"
gcc dsh.c -o bin/dsh -I.
if [ $? -eq 0 ]; then 
	echo "ready to run\n"
else 
	echo "something went wrong\n"
fi


