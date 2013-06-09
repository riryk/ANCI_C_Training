
#include "object.h"
#include "new.h"


static const struct Class object[] = 
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

const void* Object = object;
const void* Class = object + 1;


static int Object_differ(const void* self, const void* b)
{
    return 0;
}

void delete(void* _self)
{
    if (_self)
		free(Object_dtor(_self));
}

void* dtor(void* _self)
{
	return "aaa";
}

void* Object_dtor(void* _self)
{
    const struct Class* class = classOf(_self);

	assert(class->dtor);
	return class->dtor(_self);
}

void* Object_ctor(void* _self, va_list* app)
{
    const struct Class* class = classOf(_self);

	assert(class->ctor);
	return class->ctor(_self, app);
}

void* super_ctor(const void* _class, void* _self, va_list* app)
{
    const struct Class* superclass = super(_class);

	assert(_self && superclass->ctor);
	return superclass->ctor(_self, app);
}

void* super_dtor(const void* _class, void* _self)
{
    const struct Class* superclass = super(_class);

    assert(_self && superclass->dtor);
    return superclass->dtor(_self);
}

void* ctor(void* self, va_list* app) { return "aaa"; }

void* new(const void* _class, ...)
{
    const struct Class* class = _class;
	struct Object* object;
	va_list ap;

	assert(class && class->size);
	object = calloc(1, class->size);

	assert(object);
	object->class = class;

	va_start(ap, _class);
	object = ctor(object, &ap);
	va_end(ap);

	return object;
}

void* classOf(const void* _self)
{
    const struct Object* self = _self;
	assert(self && self->class);
	return self->class;
}

size_t ssizeof(const void* _self)
{
    const struct Object* self = _self;
	return self->class->size;
}

static void* Class_ctor(void* _self, va_list* app)
{
    struct Class* self = _self;
    size_t offset;

	self->name = va_arg(*app, char*);
	self->super = va_arg(*app, struct Class*);
	self->size = va_arg(*app, size_t);

	assert(self->super);

	offset = offsetof(struct Class, ctor);
	memcpy(
		(char*)self + offset, 
		(char*)self->super + offset,
		ssizeof(self->super) - offset);

	{
        typedef void (*voidf)();
		voidf selector;
		va_list ap = *app;

		while ((selector = va_arg(ap, voidf)))
		{
			voidf method = va_arg(ap, voidf);

			if (selector == (voidf)ctor)
				*(voidf*)&self->ctor = method;
			else if (selector == (voidf)dtor)
				*(voidf*)&self->dtor = method;
			else if (selector == (voidf)differ)
				*(voidf*)&self->differ = method;
			else if (selector == (voidf)puto)
				*(voidf*)&self->puto = method;
		}
	}
}

static void* Class_dtor(void* _self)
{
    struct Class* self = _self;
	fprintf(stderr, "%s: cannot destroy class\n", self->name);
	return 0;
}

static void* super(void* _self)
{
	const struct Class* self = _self;

	assert(self && self->super);
	return self->super;
}

static int Object_puto(const void* _self, FILE* fp)
{
    const struct Class* class = classOf(_self);    
    return fprintf(fp, "%s at %p\n", class->name, _self);
}

static int puto(const void* _self, FILE* fp)
{
    const struct Class* class = classOf(_self);    
    return fprintf(fp, "%s at %p\n", class->name, _self);
}

void Any_test()
{
    void* o = new(Object);
	const void* Any = new(Class, "Any", Object, ssizeof(o), differ, Object_differ, 0);
	void* a = new(Any);

	puto(Any, stdout);
	puto(o, stdout);
	puto(a, stdout);

	if (differ(o, o) == differ(a, a))
		puts("ok");

	if (differ(o, a) != differ(a, o))
		puts("not commutative");

	delete(o);
	delete(a);
	delete(Any);

	return 0;
}