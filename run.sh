#!/bin/bash

g++ cvrp.cpp -o cvrp
rm output.txt

FILES=test_instances/*
for filename in $FILES
do
    ./cvrp $filename >> output.txt
done