river: main.o river.o
	g++ -g -Wall main.o river.o -o river

main.o: main.cpp river.h
	g++ -g -Wall -c main.cpp

river.o: river.cpp river.h
	g++ -g -Wall -c river.cpp

clean:
	rm -f *.o river