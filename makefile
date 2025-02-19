# Makefile
SOURCE=src/Application.cpp
TARGET=bin/application
CXX=g++
CXXFLAGS=-Wall -g
LIBS=-lglfw -lGLEW -lGL -lGLU -ldl -lX11 -lwayland-client -lwayland-egl -ldecor-0 -pthread
INCLUDES=-Ilibs/GLFW/include -Ilibs/Glew/include
DEFINES=-DGLEW_STATIC

all: $(SOURCE)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(SOURCE) $(LIBS) -o $(TARGET)

run: all
	env SDL_VIDEODRIVER=wayland QT_QPA_PLATFORM=wayland GDK_BACKEND=wayland _GLFW_WAYLAND=1 GTK_THEME=Adwaita:dark ./$(TARGET)
