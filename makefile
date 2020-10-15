CC = g++
CPPFLAGS = -I.
LDFLAGS = -Wl,--no-as-needed
main: main.o 
main.o: main.cpp main.h
clean:
	rm *.o
	rm main
