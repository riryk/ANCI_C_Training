
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Object.h"

#ifndef POINT_CLASS_H
#define POINT_CLASS_H

const void* PPoint;
const void* CCircle;

void* PPoint_ctor(void* _self, va_list* app);
void PPoint_draw(const void* _self);

void* CCircle_ctor(void* _self, va_list* app);
void CCircle_draw(const void* _self);

void* PPointClass_ctor(void* self, va_list* app);

void draw(const void* self);
void move(void* point, int dx, int dy);

const void* PPointClass;

#endif