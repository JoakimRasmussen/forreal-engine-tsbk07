# set this variable to the director in which you saved the common files
commondir = ext/
cppdir = src/cpp/
guidir = SimpleGUI/

all : run

run : $(cppdir)run.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -g -Wall -o run -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES $(cppdir)Camera.cpp $(cppdir)GameMode.cpp $(cppdir)GameObject.cpp $(cppdir)GUI.cpp $(cppdir)InputController.cpp $(cppdir)Picking.cpp $(cppdir)run.cpp $(cppdir)Terrain.cpp $(cppdir)Utils.cpp $(guidir)SimpleGUI.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm

clean :
	rm run