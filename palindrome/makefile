words: main.o words.o
	g++ -g -Wall main.o words.o -o words

main.o: main.cpp words.h
	g++ -g -Wall -c main.cpp

words.o: words.cpp words.h
	g++ -g -Wall -c words.cpp

clean:
	rm -f *.o words