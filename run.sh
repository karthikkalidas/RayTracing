#!/bin/bash
g++ src/main.cpp -I include -O3 -o main.out
./main.out > image.ppm
xdg-open image.ppm