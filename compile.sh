#!/bin/bash

g++ src/main.cpp -o bin/run -lm

cd bin
./run
cd ..
cd img
display image.ppm
cd ..

