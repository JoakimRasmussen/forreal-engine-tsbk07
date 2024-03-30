# set this variable to the director in which you saved the common files
commondir = ext/
cppdir = src/cpp/
guidir = SimpleGUI/

all : run

run : $(cppdir)run.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -g -Wall -o run -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES $(cppdir)Picking.cpp $(cppdir)GUI.cpp $(cppdir)run.cpp $(cppdir)Camera.cpp $(cppdir)InputController.cpp $(cppdir)Terrain.cpp $(guidir)SimpleGUI.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm

clean :
	rm run