#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef NEW_H
#define NEW_H

void* new(const void* type);
void delete(void* item);

#endif
