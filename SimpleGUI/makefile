# set this variable to the director in which you saved the common files
commondir = ../ext/

all : SimpleGUIDemo SimpleGUIDemoCPP

SimpleGUIDemo : SimpleGUIDemo.c SimpleGUI.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o SimpleGUIDemo -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES SimpleGUIDemo.c SimpleGUI.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm

SimpleGUIDemoCPP : SimpleGUIDemo.cpp SimpleGUI.c $(commondir)Linux/MicroGlut.c
	g++ -Wall -o SimpleGUIDemoCPP -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES SimpleGUIDemo.cpp SimpleGUI.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm

clean :
	rm SimpleGUIDemo
	rm SimpleGUIDemoCPP
