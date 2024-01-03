maze: main.o maze.o
	g++ -g -Wall main.o maze.o -o maze

main.o: main.cpp maze.h
	g++ -g -Wall -c main.cpp

maze.o: maze.cpp maze.h
	g++ -g -Wall -c maze.cpp

clean:
	rm -f *.o maze