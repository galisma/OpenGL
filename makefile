# makefile
SOURCE=src/Application.cpp
TARGET=bin/application
CXX=g++
CXXFLAGS=-Wall -g
LIBS=-Llibs/GLFW/obj -Llibs/Glew/obj -lglfw3 -lGLEW -lGL
INCLUDES=-Ilibs/GLFW/include -Ilibs/Glew/include -Ilibs/Stb/include
DEFINES=-DGLEW_STATIC

all: $(SOURCE)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(SOURCE) $(LIBS) -o $(TARGET)

run: all
	./$(TARGET)