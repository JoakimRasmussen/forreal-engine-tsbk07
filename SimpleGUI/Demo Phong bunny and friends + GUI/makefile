# set this variable to the director in which you saved the common files
commondir = ../../common/

all : phongbunnyandfriendsGUI

phongbunnyandfriendsGUI : phongbunnyandfriendsGUI.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o phongbunnyandfriendsGUI -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES phongbunnyandfriendsGUI.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm
clean :
	rm phongbunnyandfriendsGUI

