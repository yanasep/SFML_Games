set -e
g++ -c main.cpp -I/mnt/c/c++/SFML-2.5.1/include
g++ main.o -L/mnt/c/c++/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=/mnt/c/c++/SFML-2.5.1/lib
./a.out
