UNAME := $(shell uname)

# Linux (default)
EXE = trabalhocg
LDFLAGS = -lGL -lGLU -lglut 

# OS X
ifeq ($(UNAME), Darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -Wno-deprecated-declarations
endif

all: compile

compile:
	g++ -std=c++11 $(LDFLAGS) -lm trabalhocg.cpp Circle.cpp Window.cpp Robot.cpp Bullet.cpp Enemy.cpp tinyxml2/tinyxml2.cpp -o $(EXE) $(LDFLAGS)

clean:
	rm -f trabalhocg

run: compile
	./trabalhocg config.xml