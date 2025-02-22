# Makefile
SOURCE=src/Application.cpp src/IndexBuffer.cpp src/VertexBuffer.cpp src/Renderer.cpp src/VertexArray.cpp src/Shader.cpp
TARGET=bin/application
CXX=g++
CXXFLAGS=-Wall -g
LIBS=-lglfw -lGLEW -lGL -lGLU -ldl -lX11 -lwayland-client -lwayland-egl -ldecor-0 -pthread
INCLUDES=-Ilibs/GLFW/include -Ilibs/Glew/include
DEFINES=-DGLEW_STATIC

all: $(SOURCE)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(SOURCE) $(LIBS) -o $(TARGET)

run: all
	env SDL_VIDEODRIVER=wayland QT_QPA_PLATFORM=wayland GDK_BACKEND=wayland _GLFW_WAYLAND=1 GTK_THEME=Adwaita:dark ./$(TARGET)
