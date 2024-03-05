#!/bin/bash

for file in $(ls *.c *.cpp | grep '[0-9]'); do
	echo "Имя файла: $file"
	cat "$file"
	echo "-----------------------------"
done



