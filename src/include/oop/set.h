
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifndef SET_H
#define SET_H

const void* Set;

void* new(void* type);
void* add(void* set, const void* element);
void* find(const void* set, const void* element);
void* drop(void* set, const void* element);
int contains(const void* set, const void* element);

#endif