CC = g++ -framework GLUT -framework OpenGL -framework Cocoa -Wdeprecated-declarations
all: program
program: program.o
program.o: program.cpp tinyxml/tinyxml.h tinyxml/tinyxml.cpp tinyxml/tinystr.h tinyxml/tinystr.cpp
clean:
	rm -f program program.o
run: program
	./program