//
//  main.c
//  QuadTrees
//
//  Created by Siddhi Apraj on 16/10/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "point.h"
#include "drawutil.h"

#define MAXPOINTS 4    // maximum number of points in a node of the quad tree
#define POINTSIZE 1024 // maximum size in bytes

void mergeSort(float[], int, int);
void merge(float[], int, int, int);

int maxdepth = 0; // to store max depth of the tree

// structs used are: point, circle, quadtree, leafnodes

typedef struct CIRCLE
{
    Point *center;
    float radius;
} Circle;

typedef struct QUADTREE
{
    Point *center;       // centre of the node of the quadtree square
    float halfdim;       // half the side of the node of the quad tree (square)
    Point **points;      // array of points contained in the node of the quad tree
    int depth;           // depth of the node of the quad tree
    struct QUADTREE *NW; // north west child
    struct QUADTREE *SW; // south west child
    struct QUADTREE *NE; // north east child
    struct QUADTREE *SE; // south east child
} QuadTree;

bool QTInsert(QuadTree *root, Point *point);

// this is a linked list of the leaf nodes
typedef struct leaf_nodes
{
    QuadTree *ln;
    struct leaf_nodes *next;
} leaf_nodes;

// global pointers for the leaf nodes linked list
leaf_nodes *first = NULL;
leaf_nodes *head = NULL;
leaf_nodes *temp = NULL;

// functions:

// to clear the linked list
void LL_clear()
{
    first = NULL;
    head = NULL;
    temp = NULL;
}

// to insert node in the linked list
void LL_insert(QuadTree *p)
{
    temp = (leaf_nodes *)malloc(sizeof(leaf_nodes));
    temp->ln = p;
    temp->next = NULL;
    if (first == NULL)
    {
        first = temp;
        head = first;
        // head->next = temp;
    }
    else
    {
        head->next = temp;
        head = temp;
    }
}

// to assign a new point
Point *newPoint(float x, float y)
{
    Point *p = (Point *)malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

// to print point
void printPoint(Point *point)
{
    printf("(%2.2f, %2.2f)\n", point->x, point->y);
}

// to calculate distance between two points
float distance(Point *p1, Point *p2)
{
    float x1, y1, d2;
    x1 = (p1->x) - (p2->x);
    y1 = (p1->y) - (p2->y);
    d2 = (x1 * x1) + (y1 * y1);
    return sqrt(d2);
}

// to form a new circle
Circle *newCircle(Point *center, float radius)
{
    Circle *c = (Circle *)malloc(sizeof(Circle));
    c->center = center;
    c->radius = radius;
    return c;
}

// to check if a circle contains a point
bool circleContainsPoint(Circle *c, Point *p)
{
    if (distance(c->center, p) > (c->radius))
        return false;
    else
        return true;
}

// to check if a quadrant contains a point
bool quadrantContainsPoint(Point *center, float halfdim, Point *p)
{
    bool x = false, y = false;

    if ((p->x) > ((center->x) - halfdim) && (p->x) < ((center->x) + halfdim))
        x = true;
    if ((p->y) > ((center->y) - halfdim) && (p->y) < ((center->y) + halfdim))
        y = true;

    if (x == true && y == true)
        return true;
    else
        return false;
}

// to check if 2 circles intersect
bool circleIntersection(Circle *c1, Circle *c2)
{
    if (distance(c1->center, c2->center) > ((c1->radius) + (c2->radius)))
        return false;
    else
        return true;
}

// to check if circle crosses a quadrant
bool circleCrossesQuadrant(Circle *c, Point *centerofquadrant, float halfsize)
{
    float newhalfdim = halfsize + c->radius;
    float newrad = (halfsize * sqrt(2)) + c->radius;
    Circle *ref = newCircle(centerofquadrant, newrad);

    if (quadrantContainsPoint(centerofquadrant, newhalfdim, c->center))
    {
        if (circleContainsPoint(ref, c->center))
        {
            return true;
        }
    }

    return false;
}

// to form/initialize a new quadtree node
QuadTree *newQT(Point *center, float size)
{
    QuadTree *qt = (QuadTree *)malloc(sizeof(QuadTree));

    qt->NW = NULL;
    qt->SW = NULL;
    qt->NE = NULL;
    qt->SE = NULL;
    // qt->height = 0;
    qt->depth = 0;
    qt->center = center;
    qt->halfdim = size;
    qt->points = (Point **)malloc(sizeof(Point *) * (MAXPOINTS));

    for (size_t i = 0; i < MAXPOINTS; i++)
    {
        qt->points[i] = NULL;
    }

    return qt;
}

// to find number of points in the node of the quad tree
size_t QTPointSize(Point *points[])
{
    size_t i = 0;
    for (; i < MAXPOINTS; i++)
    {
        if (points[i] == NULL)
            return i;
    }
    return i;
}

void LL_print()
{
    leaf_nodes *temp = first;
    printf("Printing of node list \n");
    while (temp != NULL)
    {
        printf("Node : depth %d points size %zu\n", temp->ln->depth, QTPointSize(temp->ln->points));
        temp = temp->next;
    }
}

// to subdivide the node into its 4 children

QuadTree *QTSubdivide(QuadTree *root)
{
    float newHalfdim = root->halfdim / 2;

    Point *nwCenter = newPoint((root->center->x) - newHalfdim, (root->center->y) - newHalfdim);
    root->NW = newQT(nwCenter, newHalfdim);
    root->NW->depth = root->depth + 1;

    Point *neCenter = newPoint((root->center->x) + newHalfdim, (root->center->y) - newHalfdim);
    root->NE = newQT(neCenter, newHalfdim);
    root->NE->depth = root->depth + 1;

    Point *swCenter = newPoint((root->center->x) - newHalfdim, (root->center->y) + newHalfdim);
    root->SW = newQT(swCenter, newHalfdim);
    root->SW->depth = root->depth + 1;

    Point *seCenter = newPoint((root->center->x) + newHalfdim, (root->center->y) + newHalfdim);
    root->SE = newQT(seCenter, newHalfdim);
    root->SE->depth = root->depth + 1;

    if (root->depth > maxdepth)
    {
        maxdepth = root->depth + 1;
    }

    // if you do not copy then points and subdivision gives wrong picture
    //  points remain with originial node and only new points get added to new node but the bounds of node are different.
    for (int i = 0; i < MAXPOINTS; i++)
    {
        QTInsert(root->NW, root->points[i]);
        QTInsert(root->NE, root->points[i]);
        QTInsert(root->SW, root->points[i]);
        QTInsert(root->SE, root->points[i]);
    }

    return root;
}

// to insert node into the quadtree
bool QTInsert(QuadTree *root, Point *point)
{

    // checking if quadrant contains point
    if (!quadrantContainsPoint(root->center, root->halfdim, point))
        return false;

    size_t pointsSize = QTPointSize(root->points);

    if (pointsSize < MAXPOINTS && root->NW == NULL)
    {
        root->points[pointsSize] = point;
        return true;
    }

    // subdividing if node is already full
    if (root->NW == NULL)
    {
        QTSubdivide(root);
        /*if(root->depth>maxdepth)
         {
             maxdepth=root->depth+1;
         }*/
        // root->height++;
    }

    // inserting after subdivision
    if (QTInsert(root->NW, point))
        return true;
    else if (QTInsert(root->NE, point))
        return true;
    else if (QTInsert(root->SW, point))
        return true;
    else if (QTInsert(root->SE, point))
        return true;

    return false;
}

// range query function - checks if points lie within a given circle
Point **QTQueryRange(QuadTree *root, Circle *range)
{
    Point **result;
    result = (Point **)malloc(sizeof(Point *) * POINTSIZE);

    size_t index = 0;
    // initializing array to NULL
    for (size_t i = 0; i < POINTSIZE; i++)
    {
        result[i] = NULL;
    }

    if (!circleCrossesQuadrant(range, root->center, root->halfdim))
    {
        return result;
    }

    if (root->NW == NULL)
    {
        size_t pointSize = QTPointSize(root->points);
        for (size_t i = 0; i < pointSize; i++)
        {
            if (circleContainsPoint(range, root->points[i]))
            {
                result[index++] = root->points[i];
            }
        }
    }

    if (root->NW == NULL)
    {
        return result;
    }

    size_t i;

    // checking for children nodes
    i = 0;
    Point **nwr = QTQueryRange(root->NW, range);
    while (nwr[i] != NULL && i < POINTSIZE)
    {
        result[index++] = nwr[i++];
    }
    i = 0;
    Point **ner = QTQueryRange(root->NE, range);
    while (ner[i] != NULL && i < POINTSIZE)
    {
        result[index++] = ner[i++];
    }
    i = 0;
    Point **swr = QTQueryRange(root->SW, range);
    while (swr[i] != NULL && i < POINTSIZE)
    {
        result[index++] = swr[i++];
    }
    i = 0;
    Point **ser = QTQueryRange(root->SE, range);
    while (ser[i] != NULL && i < POINTSIZE)
    {
        result[index++] = ser[i++];
    }

    return result;
}

// to create a linked list of all leaf nodes at max depth
void findQTNodesWithMaxDepth(QuadTree *root)
{
    // As every node maintains depth, this is easy to search node with depth and append to linked list
    if (root == NULL)
        return;

    if (root->depth >= (maxdepth))
        LL_insert(root);

    findQTNodesWithMaxDepth(root->NW);
    findQTNodesWithMaxDepth(root->NE);
    findQTNodesWithMaxDepth(root->SW);
    findQTNodesWithMaxDepth(root->SE);

    return;
}

// display distances of point from a source point
void display(float arr[], int size)
{
    int i;
    int m;
    if (size >= 3)
    {
        m = 3;
    }
    else
    {
        m = size;
    }
    for (i = 0; i < m; i++)
    {
        printf("%f ", arr[i]);
    }
    printf("\n");
}

// finds neighbouring points and their distances from the source point
void Neighbour(Circle *cquery, QuadTree *root)
{
    Point **points_array = QTQueryRange(root, cquery);

    int i = 0;
    while (points_array[i] != NULL)
    {
        i++;
    }

    float points_array_dist[i];
    i = 0;
    while (points_array[i] != NULL)
    {
        points_array_dist[i] = distance(cquery->center, points_array[i]);
        i++;
    }
    mergeSort(points_array_dist, 0, i - 1);

    printf("Nearest Neighbor distances from circle center x: %f, y: %f: \n", cquery->center->x, cquery->center->y);
    display(points_array_dist, i);
}

// merge sort function
void mergeSort(float a[], int left, int right)
{
    int mid;
    if (left < right)
    {

        mid = (left + right) / 2;
        mergeSort(a, left, mid);
        mergeSort(a, mid + 1, right);
        merge(a, left, mid, right);
    }
}

// merge function
void merge(float a[], int left, int mid, int right)
{
    int i = left, j = mid + 1, p, index = left;
    int temp[10];

    while (i <= mid && j <= right)
    {
        if (a[i] < a[j])
        {
            temp[index] = a[i];
            i = i + 1;
        }
        else
        {
            temp[index] = a[j];
            j = j + 1;
        }
        index++;
    }
    if (i > mid)
    {
        while (j <= right)
        {
            temp[index] = a[j];
            index++;
            j++;
        }
    }
    else
    {
        while (i <= mid)
        {
            temp[index] = a[i];
            index++;
            i++;
        }
    }
    p = left;
    while (p < index)
    {
        a[p] = temp[p];
        p++;
    }
}

// returns array of points common to 2 circles
Point **Intersection(Circle *c1, Circle *c2, QuadTree *root)
{
    Point **pc1 = QTQueryRange(root, c1);

    if (circleIntersection(c1, c2))
    {
        Point **common_points;
        common_points = NULL;

        common_points = (Point **)malloc(sizeof(Point *) * POINTSIZE);

        size_t index = 0;
        for (size_t i = 0; i < POINTSIZE; i++)
        {
            common_points[i] = NULL;
        }

        int i = 0;
        int j = 0;
        while (pc1[i] != NULL)
        {
            if (circleContainsPoint(c2, pc1[i]))
                common_points[j++] = pc1[i];
            i++;
        }
        return common_points;
    }
    else
    {
        return NULL;
    }
}

// to identify smallest node with maximum number of points
leaf_nodes *Traffic(QuadTree *root)
{
    leaf_nodes *present, *prev;
    size_t maxpoints;
    present = head = NULL;

    LL_clear();

    findQTNodesWithMaxDepth(root);

    // LL_print();

    present = first;
    maxpoints = QTPointSize(first->ln->points);

    // traversing linked list to find maximum number of points in any given node of the linked list
    while (present != NULL)
    {
        if (QTPointSize(present->ln->points) > maxpoints)
        {
            maxpoints = QTPointSize(present->ln->points);
        }
        present = present->next;
    }

    // modifying linked list to only contain nodes with maximum number of points
    present = prev = first;
    while (present != NULL)
    {
        if (QTPointSize(present->ln->points) < maxpoints)
        {
            if (present == first)
            {
                first = present->next;
                present = present->next;
                prev = present;
            }
            else
            {
                prev->next = present->next;
                // free(present);
                present = prev->next;
            }
        }
        else
        {
            if (present == first)
            {
                present = present->next;
            }
            else
            {
                prev = prev->next;
                present = present->next;
            }
        }
    }

    return first;
}

/*
void testDrawUtil()
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
}
*/

// to draw a node of the quad tree
void drawQTreeNode(char linebuffer[BUFF_SIZE][BUFF_SIZE], QuadTree *tree)
{

    if (tree == NULL || linebuffer == NULL)
        return;

    if (tree->center != NULL)
        drawRect(linebuffer, tree->center->x, tree->center->y, tree->halfdim * 2, tree->halfdim * 2);

    if (tree->points != NULL)
    {
        size_t i = 0;
        for (; i < MAXPOINTS; i++)
        {
            if (tree->points[i] != NULL)
            {
                drawPoint(linebuffer, (int)tree->points[i]->x, (int)tree->points[i]->y);
            }
        }
    }

    // recursively calling function for the children
    if (tree->NW != NULL)
    {
        drawQTreeNode(linebuffer, tree->NW);
    }

    if (tree->NE != NULL)
    {
        drawQTreeNode(linebuffer, tree->NE);
    }

    if (tree->SW != NULL)
    {
        drawQTreeNode(linebuffer, tree->SW);
    }

    if (tree->SE != NULL)
    {
        drawQTreeNode(linebuffer, tree->SE);
    }
}

// draw the whole quadtree
void drawQTree(QuadTree *tree)
{
    char linebuffer[BUFF_SIZE][BUFF_SIZE];
    int centerx = 50, centery = 50, height = 100, width = 100;
    initBuffer(linebuffer);
    drawRect(linebuffer, centerx, centery, 100, 100);
    drawQTreeNode(linebuffer, tree);
    printf("\n");
    drawBuffer(linebuffer);
}

void drawRangeQueryResult(Circle *c, Point **rpoints)
{
    char linebuffer[BUFF_SIZE][BUFF_SIZE];
    int centerx = 50, centery = 50, height = 100, width = 100;
    initBuffer(linebuffer);
    drawRect(linebuffer, centerx, centery, 100, 100);
    drawCircle(linebuffer, c->center->x, c->center->y, c->radius);

    for (int i = 0; i < POINTSIZE; i++)
    {
        if (rpoints[i] != NULL)
            drawPoint(linebuffer, rpoints[i]->x, rpoints[i]->y);
        else
            break;
    }

    printf("\n Drawing RangeQuery Result \n");
    drawBuffer(linebuffer);
}

// to show the points that lie in the range of 2 circles
void drawIntersectionQueryResult(Circle *c, Circle *c2, Point **rpoints)
{
    char linebuffer[BUFF_SIZE][BUFF_SIZE];
    int centerx = 50, centery = 50, height = 100, width = 100;
    initBuffer(linebuffer);
    drawRect(linebuffer, centerx, centery, 100, 100);
    drawCircle(linebuffer, c->center->x, c->center->y, c->radius);
    drawCircle(linebuffer, c2->center->x, c2->center->y, c2->radius);

    for (int i = 0; i < POINTSIZE; i++)
    {
        if (rpoints[i] != NULL)
            drawPoint(linebuffer, rpoints[i]->x, rpoints[i]->y);
    }

    printf("\n Drawing IntersectionQueryResult Result \n");
    drawBuffer(linebuffer);
}

// draw buffer for Traffic/Tower Query
void drawTowerQueryResult(leaf_nodes *ll)
{
    char linebuffer[BUFF_SIZE][BUFF_SIZE];
    int centerx = 50, centery = 50, height = 100, width = 100;
    initBuffer(linebuffer);
    drawRect(linebuffer, centerx, centery, 100, 100);

#if 0
    size_t maxpoints=0;
    leaf_nodes* present = ll;

    while(present!=NULL)
    {
        int pointSize = QTPointSize(present->ln->points);
        printf("Point Size %d\n", pointSize);
        if(pointSize>=maxpoints)
        {
            maxpoints = pointSize;
           
        }
         present=present->next;
    }

   

    leaf_nodes * temp = ll;
    while (temp != NULL) {
        if (QTPointSize(temp->ln->points) == maxpoints )
        drawQTreeNode(linebuffer, temp->ln);

        temp = temp->next;
    }
#endif

    // drawQTreeNode(linebuffer, ll->ln);

    leaf_nodes *temp = ll;
    while (temp != NULL)
    {
        drawQTreeNode(linebuffer, temp->ln);

        temp = temp->next;
    }

    printf("\n Drawing Tower Query Result\n");
    drawBuffer(linebuffer);
}

// simple showcase of construction of quad tree and visual representation of its applications
void testQuadTree()
{
    int num;

    int centerx = 50, centery = 50, height = 100, width = 100;
    Point *center = newPoint(centerx, centery);

    printf("Enter number of points : ");
    scanf("%d", &num);
    Point **points = (Point **)malloc(sizeof(Point *) * (num));

    QuadTree *myTree = newQT(center, height / 2);

    float x, y, radius;

    for (int i = 0; i < num; i++)
    {

        scanf("%f, %f", &x, &y); // user input for choice
        Point *pt = newPoint(x, y);
        points[i] = pt;
        // drawPoint(linebuffer, (int)points[i]->x, (int)points[i]->y);
        QTInsert(myTree, pt);
    }

    printf("Enter circle coordinates and radius : ");

    scanf("%f, %f, %f", &x, &y, &radius);
    Point *ccenter = newPoint(x, y);
    Circle *c = newCircle(ccenter, radius);

    printf("Enter 2nd circle coordinates and radius : ");
    float x2, y2, radius2;

    scanf("%f, %f, %f", &x2, &y2, &radius2);
    Point *ccenter2 = newPoint(x2, y2);
    Circle *c2 = newCircle(ccenter2, radius2);

    Point **rpoints = QTQueryRange(myTree, c);

    drawQTree(myTree);

    drawRangeQueryResult(c, rpoints);

    leaf_nodes *ll = Traffic(myTree);
    drawTowerQueryResult(ll);

    Point **rpoints2 = Intersection(c, c2, myTree);

    drawIntersectionQueryResult(c, c2, rpoints2);

    Neighbour(c2, myTree);

}

int main()
{

    // testDrawUtil();

    testQuadTree();

    return 0;
}
