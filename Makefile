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
	g++ -std=c++11 $(LDFLAGS) -lm trabalhocg.cpp Window.cpp Stuff.cpp Circle.cpp Robot.cpp Enemy.cpp Bullet.cpp TextureLoader.cpp tinyxml2/tinyxml2.cpp -o $(EXE) $(LDFLAGS)

clean:
	rm -f trabalhocg
	
run: all
	./trabalhocg ./