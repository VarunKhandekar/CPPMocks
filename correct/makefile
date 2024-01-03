correct: main.o correct.o
	g++ -g -Wall main.o correct.o -o correct

main.o: main.cpp correct.h
	g++ -g -Wall -c main.cpp

correct.o: correct.cpp correct.h
	g++ -g -Wall -c correct.cpp

clean:
	rm -f *.o correct