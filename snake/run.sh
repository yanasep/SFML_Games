set -e
g++ -c main.cpp
g++ main.o -lsfml-graphics -lsfml-window -lsfml-system
./a.out
