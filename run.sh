#!/bin/bash
g++ src/main.cpp -I include -o main.out
./main.out > image.ppm
xdg-open image.ppm