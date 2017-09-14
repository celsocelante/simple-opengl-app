all: compile
compile:
	g++ -Wno-deprecated-declarations -framework OpenGL -framework GLUT -lm trabalhocg.cpp Circle.cpp Window.cpp tinyxml2/tinyxml2.cpp -o trabalhocg
clean:
	rm -f trabalhocg
run: compile
	./trabalhocg