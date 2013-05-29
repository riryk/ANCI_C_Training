

struct Class
{
	size_t size;
	void* (*ctor)(void* self, va_list* app);
	void* (*dtor)(void* self);
	void* (*clone)(const void* self);
	int (*differ)(const void* self, const void* b);
};

void* new_class(const void* _class, ...)
{
    const struct Class* class = _class;
	void* p = calloc(1, class->size);
    
	assert(p);
    *(const struct Class**)p = class;
} 


