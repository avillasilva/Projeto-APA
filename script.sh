#!/bin/bash

rm output.txt

FILES=instancias_teste/*
for filename in $FILES
do
    ./projeto $filename >> output.txt
done
