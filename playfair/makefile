playfair: main.o playfair.o
	g++ -g -Wall main.o playfair.o -o playfair

main.o: main.cpp playfair.h
	g++ -g -Wall -c main.cpp

playfair.o: playfair.cpp playfair.h
	g++ -g -Wall -c playfair.cpp

clean:
	rm -f *.o playfair