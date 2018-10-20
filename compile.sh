#!/bin/sh
sfml="-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio"
g++ -Wall -Wextra -c ColorConverter.cpp $sfml
g++ -Wall -Wextra -c main.cpp $sfml
g++ -Wall -Wextra -c FFT.cpp $sfml
g++ -Wall -Wextra -g -o main FFT.o main.o ColorConverter.o $sfml
