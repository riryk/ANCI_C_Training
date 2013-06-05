
#include "point.h"
#include "new.h"

static const struct Class _Point = 
{
    sizeof(struct Point),
	Point_ctor,
	0
};

const void* Point;

static void* Point_ctor(void* _self, va_list* app)
{
    struct Point* self = _self;
	self->x = va_arg(*app, int);
	self->y = va_arg(*app, int);
	return self;
}

void Point_draw(const void* _self)
{
    const struct Point* self = _self;
	printf("\".\" at %d,%d\n", self->x, self->y);
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
	draw(p);
	move(p, 10, 20);
	draw(p);
	delete(p);
}
