minesweeper: main.o minesweeper.o
	g++ -g -Wall main.o minesweeper.o -o minesweeper

main.o: main.cpp minesweeper.cpp
	g++ -g -Wall -c main.cpp

minesweeper.o: minesweeper.cpp minesweeper.h
	g++ -g -Wall -c minesweeper.cpp

clean:
	rm -f *.o minesweeper