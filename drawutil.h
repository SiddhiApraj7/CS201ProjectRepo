#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "point.h"

/**
The utility helps to draw on a screen size of 100*100 using screen buffer technique

100*100 canvas is represented by a double dimension char array and each element array
represents a point on screen using buffer[y][x]

Top left (x =0 , y=0)
Bottom right ( X=100, y=100)

initBuffer - to initialize the canvass
drawBuffer - to draw on screen

drawPoint - add a point
drawCircle - draw a circle
drawRect - draw a Rectangle.

While drawing a shape, use center point and dimension of shape to calculate x and y coordinate
and replace that in the buffer array created using char.


*/

#ifndef DRAWUTIL_H
#define DRAWUTIL_H
extern const int BUFF_SIZE;
// = 102;
//function declarations
void initBuffer(char linebuff[BUFF_SIZE][BUFF_SIZE]);
void drawBuffer(char linebuff[BUFF_SIZE][BUFF_SIZE]);
void drawPoint(char linebuff[BUFF_SIZE][BUFF_SIZE], int x, int y);
void drawCircle(char linebuff[BUFF_SIZE][BUFF_SIZE], int centerx, int centery, int radius);
void drawRect(char linebuff[BUFF_SIZE][BUFF_SIZE], int centx, int centy, int sideA, int sideB ) ;

#endif
