
#include "point.h"
#include "new.h"
#include "object.h"

static const struct Class _Point = 
{
    sizeof(struct Point),
	Point_ctor,
	0
};

static const struct Class _Circle =
{
    sizeof(struct Circle),
	Circle_ctor,
	0
};

const void* Point = &_Point;
const void* Circle = &_Circle;

static void* Point_ctor(void* _self, va_list* app)
{
    struct Point* self = _self;
	self->x = va_arg(*app, int);
	self->y = va_arg(*app, int);
	return self;
}

static void* Circle_ctor(void* _self, va_list* app)
{
    struct Circle* self = 
		((const struct Class*)Point)->ctor(_self, app);

	self->rad = va_arg(*app, int);
	return self;
}

void Point_draw(const void* _self)
{
    const struct Point* self = _self;
	printf("\".\" at %d,%d\n", self->x, self->y);
}

void Circle_draw(const void* _self)
{
    const struct Circle* self = _self;
	printf("circle at %d,%d rad %d\n", x(self), y(self), self->rad);
}

void Point_move(void* _self, int dx, int dy)
{
    struct Point* self = _self;
	self->x += dx;
	self->y += dy;
}

void point_test()
{
	void* p = new(Point, 1, 2);
	Point_draw(p);
	Point_move(p, 10, 20);
	Point_draw(p);
	delete(p);
}
