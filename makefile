VPATH = src
# Se buscan los archivos .cpp en src y en src/vendor
SRCS = $(wildcard src/*.cpp) $(wildcard src/vendor/*.cpp)

# Directorio de objetos
BUILD_DIR = bin/build
# Transformamos cada fuente (con ruta relativa a "src/") a su correspondiente objeto en "bin/build/"
OBJS = $(patsubst src/%, $(BUILD_DIR)/%, $(SRCS:.cpp=.o))

TARGET = bin/application
CXX = g++
CXXFLAGS = -Wall -g
LIBS = -lglfw -lGLEW -lGL -lGLU -ldl -lX11 -lwayland-client -lwayland-egl -ldecor-0 -pthread
INCLUDES = -Ilibs/GLFW/include -Ilibs/Glew/include
DEFINES = -DGLEW_STATIC

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $(TARGET))
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(OBJS) $(LIBS) -o $(TARGET)

# Regla patrón para compilar cada archivo fuente de "src" en su objeto en "bin/build"
$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: all
	env SDL_VIDEODRIVER=wayland QT_QPA_PLATFORM=wayland GDK_BACKEND=wayland _GLFW_WAYLAND=1 GTK_THEME=Adwaita:dark ./$(TARGET)
