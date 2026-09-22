#!/bin/bash
mkdir bin
echo -e "building project\n"
gcc dsh.c -o bin/dsh -I.
if [ $? -eq 0 ]; then 
	echo -e "output binary: bin/dsh\n"
else 
	echo -e "something went wrong\n"
fi


