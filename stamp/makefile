stamp: main.o stamp.o
	g++ -g -Wall main.o stamp.o -o stamp -lcrypto

main.o: main.cpp stamp.h
	g++ -g -Wall -c main.cpp

stamp.o: stamp.cpp stamp.h
	g++ -g -Wall -c stamp.cpp

clean:
	rm -f *.o stamp
