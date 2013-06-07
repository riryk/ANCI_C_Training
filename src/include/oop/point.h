
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

struct Point_
{
   const char _
	   [  
		   sizeof(struct 
		   {
               const void* class;
               int x;
               int y;             
		   })
	   ];
};

struct Circle
{
   const struct Point _;
   int rad;
};

#define x(p) (((const struct Point*)(p))->x)
#define y(p) (((const struct Point*)(p))->y)

#define set_x(p,v) ((const struct Point*)(p))->x = (v)

void move(void* point, int dx, int dy);
void draw(const void* self);
void* Point_ctor(void* _self, va_list* app);
void* Circle_ctor(void* _self, va_list* app);

#endif