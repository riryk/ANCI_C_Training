#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#ifndef NEW_H
#define NEW_H

void* nnew(const void* _class, ...);
void ddelete(void* self);
void* clone(const void* self);
int differ(const void* self, const void* b);
size_t sizeOf(const void* self);


#endif
