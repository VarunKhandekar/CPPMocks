piglatin: main.o piglatin.o
	g++ -g -Wall main.o piglatin.o -o piglatin

main.o: main.cpp piglatin.h
	g++ -g -Wall -c main.cpp

piglatin.o: piglatin.cpp piglatin.h
	g++ -g -Wall -c piglatin.cpp

clean:
	rm -f *.o piglatin