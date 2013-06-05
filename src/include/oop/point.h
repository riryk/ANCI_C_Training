
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifndef POINT_H
#define POINT_H

struct Point
{
   const void* class;
   int x;
   int y;
};

struct Circle
{
   const struct Point;
   int rad;
}

void move(void* point, int dx, int dy);
void draw(const void* self);

#endif