# Compiler and flags
CC = g++
CFLAGS = -g -Wall -DGL_GLEXT_PROTOTYPES -MMD -MP
LIBS = -lX11 -lGL -lm -lz

# Set the directory in which you saved the common files
COMMON_DIR = ext/
CPP_DIR = src/cpp/
GUI_DIR = SimpleGUI/
OS_SUBDIR = Linux

# Explicitly set the include directories
INCLUDES = -I$(COMMON_DIR) -I$(COMMON_DIR)$(OS_SUBDIR) -I$(GUI_DIR)

# Find source files
CPP_SOURCES = $(wildcard $(CPP_DIR)*.cpp)
COMMON_SOURCES = $(wildcard $(COMMON_DIR)*.c) $(wildcard $(COMMON_DIR)$(OS_SUBDIR)/*.c)
GUI_SOURCES = $(wildcard $(GUI_DIR)*.c)

# Define object files
OBJECTS = $(CPP_SOURCES:.cpp=.o) $(COMMON_SOURCES:.c=.o) $(GUI_SOURCES:.c=.o)

# Define dependency files
DEPS = $(OBJECTS:.o=.d)

# Target executable name
TARGET = run

# Default target
all: $(TARGET)

# Linking the target with object files
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

# Rule to compile C source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile C++ source files to object files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Include the dependency files
-include $(DEPS)

# Clean the build
clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)

.PHONY: all clean



# OLD

# # set this variable to the director in which you saved the common files
# commondir = ext/
# cppdir = src/cpp/
# guidir = SimpleGUI/

# all : run

# run : $(cppdir)run.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
# 	g++ -g -Wall -o run -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES $(cppdir)Camera.cpp $(cppdir)GameMode.cpp $(cppdir)GameObject.cpp $(cppdir)GUI.cpp $(cppdir)InputController.cpp $(cppdir)Picking.cpp $(cppdir)run.cpp $(cppdir)Terrain.cpp $(cppdir)Utils.cpp $(guidir)SimpleGUI.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm

# clean :
# 	rm run