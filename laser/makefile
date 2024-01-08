laser: main.o laser.o
	g++ -g -Wall main.o laser.o -o laser

main.o: main.cpp laser.h
	g++ -g -Wall -c main.cpp

laser.o: laser.cpp laser.h
	g++ -g -Wall -c laser.cpp

clean:
	rm -f *.o laser