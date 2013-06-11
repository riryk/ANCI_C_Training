
#include "point_class.h"


struct PPoint
{
	const struct Object _; /* Point : Object */
	int x;
    int y;
};

struct CCircle
{
   const struct PPoint _;
   int rad;
};

#define x(p) (((const struct PPoint*)(p))->x)
#define y(p) (((const struct PPoint*)(p))->y)

void super_draw(const void* class, const void* self);

struct PPointClass
{
	const struct Class _; /* PointClass : Class */
	void (*draw)(const void* self);
};

void draw(const void* self) { }

void initPoint(void)
{
	if (!PPointClass)
		PPointClass = new(Class, "PointClass",
		         Class, sizeof(struct PPointClass),
				 ctor, PPointClass_ctor,
				 0);
	if (!PPoint)
		PPoint = new (PPointClass, "Point",
		         Object, sizeof(struct PPoint),
				 ctor, PPoint_ctor,
				 draw, PPoint_draw,
				 0);
}

void initCircle(void)
{
    if (!CCircle)
	{
		initPoint();
        CCircle = new(PPointClass, "Circle",
			     PPoint, sizeof(struct CCircle),
				 ctor, CCircle_ctor,
				 draw, CCircle_draw,
				 0);
	}
}

static void* PPoint_ctor(void* _self, va_list* app)
{
	struct PPointClass* self = super_ctor(PPointClass, _self, app);
	typedef void (*voidf)();
    voidf selector;
	va_list ap = *app;

    while ((selector = va_arg(ap, voidf)))
	{
        voidf method = va_arg(ap, voidf);
		if (selector == (voidf)draw)
			*(voidf*)&self->draw = method;
	}
	return self;
}

static void PPoint_draw(const void* _self)
{
    const struct PPoint* self = _self;
	printf("\".\" at %d,%d\n", self->x, self->y);
}

static void* CCircle_ctor(void* _self, va_list* app)
{
    struct CCircle* self = super_ctor(CCircle, _self, app);

	self->rad = va_arg(*app, int);
	return self;
}

static void* PPointClass_ctor(void* self, va_list* app)
{
	struct PPointClass* pself = super_ctor(PPointClass, self, app);

	return pself;
}

static void CCircle_draw(const void* _self)
{
    const struct CCircle* self = _self;
	printf("circle at %d,%d rad %d\n", x(self), y(self), self->rad);
}

void point_class_test()
{
    initCircle();
}

