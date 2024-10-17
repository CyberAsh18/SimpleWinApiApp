CC=g++
CFLAGS=-I.

MAIN_NAME = simpleDemo.exe
ARGS = -lgdi32 -m64 -mwindows

main: main.cpp
	$(CC) main.cpp $(ARGS) -o $(MAIN_NAME) 

clean:
	del /Q $(MAIN_NAME)