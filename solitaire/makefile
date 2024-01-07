solitaire: main.o solitaire.o
	g++ -g -Wall main.o solitaire.o -o solitaire

main.o: main.cpp solitaire.h
	g++ -g -Wall -c main.cpp

solitaire.o: solitaire.cpp solitaire.h
	g++ -g -Wall -c solitaire.cpp

clean:
	rm -f *.o solitaire