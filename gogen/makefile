gogen: main.o gogen.o
	g++ -g -Wall main.o gogen.o -o gogen

main.o: main.cpp gogen.h common.h mask.h
	g++ -g -Wall -c main.cpp

gogen.o: gogen.cpp gogen.h common.h mask.h
	g++ -g -Wall -c gogen.cpp

clean:
	rm -f *.o gogen