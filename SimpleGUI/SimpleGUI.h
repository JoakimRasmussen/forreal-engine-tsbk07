#ifndef _SIMPLEGUI_
#define _SIMPLEGUI_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

// if Cocoa and M1
// I do not want this dependency!
#include "../ext/Linux/MicroGlut.h"

typedef void (*NoArgProcPtr)();

typedef struct Item
{
	int itemType;
	int state;
	int intData;
	float sliderMin, sliderMax, r, g, b;
	char *s;
	int x, y; // , w, h;
	int hx, hy, hw, hh; // hot box!
	void *var1,*var2,*var3; // Pointers to controlled variables
	int wasHit;
} Item;

// Create controls
int sgCreateSlider(int x, int y, int w, float *variable, float min, float max);
int sgCreateColorBox(int x, int y, float *r, float *g, float *b);
int sgCreateColorDrop(int x, int y, float *r, float *g, float *b);
void sgCreateSliderColorGroup(int x, int y, int w, float *r, float *g, float *b);
int sgCreateCheckBox(int x, int y, const char *s, int *variable);
int sgCreateStaticString(int x, int y, const char *s);
int sgCreateDynamicString(int x, int y, char *s);
int sgCreateDisplayInt(int x, int y, const char *s, int *variable);
int sgCreateDisplayFloat(int x, int y, const char *s, float *variable);
int sgCreateRadio(int x, int y, const char *s, int *variable, int index);
int sgCreateButton(int x, int y, const char *s, NoArgProcPtr callback);
void sgCreateSliderVec3Group(int x, int y, int w, float *v, float min, float max);

int sgCreateMenu(int x, int y, const char *s);
int sgCreateMenuItem(const char *s, NoArgProcPtr callback);

int sgCreateColorClicker(int x, int y, float r, float g, float b, float *dr, float *dg, float *db);
int sgCreateSmallColorClicker(int x, int y, float r, float g, float b, float *dr, float *dg, float *db);
void sgCreateColorPalette(int x, int y, float *dr, float *dg, float *db);
void sgCreateSmallColorPalette(int x, int y, float *dr, float *dg, float *db);

int sgCreateLeftStepper(int x, int y, int *variable);
int sgCreateRightStepper(int x, int y, int *variable);
void sgCreateSteppers(int x, int y, int *variable);

void sgRemoveItem(int i);

// Mandatory calls from host program
void sgDraw();
int sgMouse(int state, int x, int y);
int sgMouseDrag(int x, int y);

// Optional configurations
void sgSetFrameColor(float r, float g, float b);
void sgSetFillColor(float r, float g, float b);
void sgSetSliderFillColor(int sliderID, float r, float g, float b);
void sgSetSliderDefaultFillColor(float r, float g, float b);
void sgSetBackgroundColor(float r, float g, float b, float a);
void sgSetBackgroundBorder(int b);
void sgSetTextColor(float r, float g, float b);
void sgSetPosition(int x, int y);
void sgSetSpacing(int spacing);
void sgSetScale(int scale);


// OWN IMPLEMENTATION
void myCreateGrayScaledColorPalette(int x, int y, float *dr, float *dg, float *db);
Item* myCreateColorClicker(int x, int y, float r, float g, float b, float *dr, float *dg, float *db);

#ifdef __cplusplus
}
#endif

#endif
