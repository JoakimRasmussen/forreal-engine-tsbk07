# set this variable to the director in which you saved the common files
commondir = ../../common/

all : mteapot-demo

mteapot-demo : mteapot-demo.c ../SimpleGUI.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o mteapot-demo ../SimpleGUI.c -I.. -I$(commondir) -I$(commondir)/Linux -DGL_GLEXT_PROTOTYPES mteapot-demo.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm psychteapot2

