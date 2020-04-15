#!/bin/bash

g++ projeto-apa.cpp -o projeto
rm output.txt

FILES=instancias_teste/*
for filename in $FILES
do
    ./projeto $filename >> output.txt
done