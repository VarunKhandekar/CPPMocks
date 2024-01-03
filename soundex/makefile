soundex: main.o soundex.o
	g++ -g -Wall main.o soundex.o -o soundex

main.o: main.cpp soundex.h
	g++ -g -Wall -c main.cpp

soundex.o: soundex.cpp soundex.h
	g++ -g -Wall -c soundex.cpp

clean:
	rm -f *.o soundex