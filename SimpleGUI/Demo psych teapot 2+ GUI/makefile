# set this variable to the director in which you saved the common files
commondir = ../../common/

all : psychteapot2

psychteapot2 : psychteapot2.c ../SimpleGUI.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o psychteapot2 ../SimpleGUI.c -I.. -I$(commondir) -I$(commondir)/Linux -DGL_GLEXT_PROTOTYPES psychteapot2.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm psychteapot2

