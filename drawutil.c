#include "drawutil.h"

/*
The utility helps to draw on a screen size of 100x100 using screen buffer technique

100x100 canvas is represented by a double dimension char array and each element array
represents a point on screen using buffer[y][x]

Top left (x=0, y=0)
Bottom right (x=100, y=100)

initBuffer - to initialize the canvas
drawBuffer - to draw on screen

drawPoint - add a point
drawCircle - draw a circle
drawRect - draw a Rectangle.

While drawing a shape, use center point and dimension of shape to calculate x and y coordinate
and replace that in the buffer array created using char.


*/

const int BUFF_SIZE = 102;  //constant buffer size assuming 100x100 screen

//this initializes the canvas
void initBuffer(char linebuff[BUFF_SIZE][BUFF_SIZE])
{
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        for (int j = 0; j < BUFF_SIZE - 2; j++)
        {
            linebuff[i][j] = ' ';
        }
        linebuff[i][BUFF_SIZE - 2] = '\0';
    }
}

//this draws the buffer as it is stored
void drawBuffer(char linebuff[BUFF_SIZE][BUFF_SIZE])
{
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        // printf(linebuff[1]);
        printf("%s\n", linebuff[i]);
    }
}

//locates a point on the canvas(buffer) ; point is denoted by a '+'
void drawPoint(char linebuff[BUFF_SIZE][BUFF_SIZE], int x, int y)
{
    linebuff[y][x] = '+';
}

//locates a circle on the canvas(buffer) ; circle is denoted by a '.'
void drawCircle(char linebuff[BUFF_SIZE][BUFF_SIZE], int centerx, int centery, int radius)
{
    int x, y;
    for (int i = 0; i < 360; i++) //traversing angles in degrees
    {
        double rad = 3.14 / 180 * i;  //conversion to radians

        //calculating coordinates via sin and cos
        x = centerx + (int)(round(((float)radius * cos(rad))));
        y = centery + (int)(round(((float)radius * sin(rad))));

        //drawing circumference
        if (x >= 0 && y >= 0)
            linebuff[y][x] = '.';
    }
}

//locates a circle on the canvas(buffer) ; circle is denoted by a '-' or '|'
void drawRect(char linebuff[BUFF_SIZE][BUFF_SIZE], int centx, int centy, int sideA, int sideB)
{

    int startx = centx - sideA / 2;
    int starty = centy - sideB / 2;
    int newSideA = startx + sideA;
    int newSideB = starty + sideB;

    for (int i = starty; i < newSideB; i++)  //y coordinate
    {
        for (int j = startx; j < newSideA; j++)  //x coordinate
        {

            if (i >= 0 && j >= 0)
            {

                if (i == starty || i == newSideB - 1 || j == startx || j == newSideA - 1)
                {
                    if (linebuff[i][j] != '+') //lets the point remain
                        linebuff[i][j] = '-';
                }
                else
                {
                    // linebuff[i][j] = ' ';
                }
            }
        }
    }
    
}

//code used for debugging
#if 0
int main()
{
    int num;
    char linebuffer[BUFF_SIZE][BUFF_SIZE];

    initBuffer(linebuffer);
    drawRect(linebuffer, 50, 50, 100, 100);

    printf("Enter number of points : ");
    scanf("%d", &num);

    Point **points = (Point **)malloc(sizeof(Point *) * (num));

    for (int i = 0; i < num; i++)
    {
        points[i] = (Point *)malloc(sizeof(Point));
        scanf("%f, %f", &points[i]->x, &points[i]->y); // user input for choice
        drawPoint(linebuffer, (int)points[i]->x, (int)points[i]->y);
    }

    drawRect(linebuffer, 25, 25, 49, 49);

    drawRect(linebuffer, 36, 52, 25, 25);
    drawPoint(linebuffer, 36, 52);

    // drawRect(linebuffer, 26,26, 24, 24);

    drawCircle(linebuffer, 6, 52, 25);
    drawPoint(linebuffer, 6, 52);

    // drawPoint(linebuffer,5,5);
    // drawPoint(linebuffer,25,25);
    printf("\n");
    drawBuffer(linebuffer);

    return 0;
}

#endif
