doublets: main.o doublets.o
	g++ -g -Wall -O3 main.o doublets.o -o doublets

main.o: main.cpp doublets.h
	g++ -g -Wall -c -O3 main.cpp

doublets.o: doublets.cpp doublets.h dictionary.h
	g++ -g -Wall -c - O3 doublets.cpp

clean:
	rm -f *.o doublets