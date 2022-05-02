#################################################
#	Makefile For project timber
#	Author:	Eric Muuo
#################################################

c++ = g++	#use the gnu compiler
libs = -lsfml-graphics -lsfml-window -lsfml-system
flags = -g -Wall -Wextra -std=c++17

timber: main.o
	$(c++) main.o -o timber $(libs)

main.o: main.cpp
	$(c++) main.cpp -c -o main.o $(flags)