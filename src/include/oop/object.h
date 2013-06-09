#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>     /* offsetof */

#ifndef OBJECT_H
#define OBJECT_H

struct Object
{
	const struct Class* class;
};

struct Class
{
	const struct Object _;
	const char* name;
	const struct Class* super;
	size_t size;
	void* (*ctor)(void* self, va_list* app);
	void* (*dtor)(void* self);
	void* (*clone)(const void* self);
	int (*differ)(const void* self, const void* b);
	int (*puto)(const void* self, FILE* fp);
};

void* new(const void* class, ...);
void delete(void* _self);
void* super(void* _self);
void* classOf(const void* _self);

void* Object_ctor(void* _self, va_list* app);
void* Object_dtor(void* _self);
int Object_differ(const void* self, const void* b);
int Object_puto(const void* _self, FILE* fp);

void* Class_ctor(void* _self, va_list* app);
void* Class_dtor(void* _self);

void* dtor(void* _self);
void* ctor(void* self, va_list* app);
int differ(const void* self, const void* b);
int puto(const void* self, FILE* fp);


extern const void* Object;
extern const void* Class;

/*
extern struct Class object[] = 
{
	{
	   { object + 1 },
	   "Object", object, sizeof(struct Object),
	   Object_ctor, Object_dtor, Object_differ, Object_puto
	},
	{
	   { object + 1 },
	   "Class", object, sizeof(struct Class),
       Class_ctor, Class_dtor, Object_differ, Object_puto
	}
};

extern void* Object = object;
extern void* Class = object + 1;
*/

#endif