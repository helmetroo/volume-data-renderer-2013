SOURCE = ddsbase.cpp fullquad.cpp shader.cpp light.cpp texture.cpp buffer.cpp boundingbox.cpp trackball.cpp scene.cpp ui.cpp main.cpp
OBJECTS = $(SOURCE:.cpp=.o)
EXE = output
CC = g++
CFLAGS = -c -Wall -g -DGL_GLEXT_PROTOTYPES -std=c++0x

# GLUI isn't on the CADE by default, so it comes bundled here
GLUI_INCLUDE=.
GLM_INCLUDE=.

# Compilation for Mac and Linux
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	GLUT_LIB=/usr/lib
	GLUT_INCLUDE=/usr/include
	GLUI_LIB=lib

	FLAGS = -I$(GLUI_INCLUDE) -I$(GLUT_INCLUDE) -I$(GLM_INCLUDE) -L$(GLUT_LIB)  -L$(GLUI_LIB) \
	-lglui -lglut -lXext -lX11 -lGL -lGLU -lm

else
	CC = clang++
	FLAGS = -I$(GLUI_INCLUDE) -framework GLUT -framework GLUI -framework OpenGL
endif


all: $(SOURCE) $(EXE)

$(EXE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(FLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXE)
