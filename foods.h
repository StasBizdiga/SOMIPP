#ifndef FOODS_H
#define FOODS_H
#include <stdio.h>
#include <string.h>

struct food{
   int  id;
   char  name[50];
   float preptime;
   int   complexity; // low 1 - 3 high
   char  tool[25];   // restrictions
   int   toolID;     // util to make things simpler: 0-none, 1-stove, 2-oven
};

#endif
