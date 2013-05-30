
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifndef SET_H
#define SET_H

const void* Set;

void* new_set(const void* type);
void delete_set(void* item);
void* add_set(void* set, const void* element);
void* find_set(const void* set, const void* element);
void* drop_set(void* set, const void* element);
int contains_set(const void* set, const void* element);
int differ_set(const void* a, const void* b);

void set_test();

#endif



