/* GCJ F 13 4

Problem
 

"I say we must move forward, not backward;
 upward, not forward;
 and always twirling, twirling, twirling towards freedom!"
 — Former U.S. Presidential nominee Kodos. 

After hearing this inspirational quote from America's 
first presidential nominee from the planet Rigel VII, 
you have decided that you too would like to twirl (rotate) towards freedom. 

For the purposes of this problem, you can think of "freedom" as being 
as far away from your starting location as possible.
 
The galaxy is a two-dimensional plane. 
Your space ship starts at the origin, position (0, 0). 
There are N stars in the galaxy. 

Every minute, you can choose a star and rotate your space ship 
90 degrees clockwise around the star. You may also choose to stay where you are.
 
How far away can you move from the origin after M minutes? 

The image illustrates the first 3 rotations for a possible path in sample case 1. 
Note that this path is not necessarily a part of any optimal solution.

Input
 
The first line of the input gives the number of test cases, T. 
T test cases follow, beginning with two lines containing integers N and M. 
The next N lines each contain two integers, Xi and Yi, representing the locations of stars.


Output

 
For each test case, output one line containing "Case #x: D", 
where x is the case number (starting from 1) 
and D is the distance from the origin to the optimal final position. 
Answers with absolute or relative error no larger than 10-6 will be accepted.


The Small Input


On most Google Code Jam problems, 
the small input can be solved without worrying about the running time. 
That was not the case here though. 
Even if N = 10 and M = 10, there are still 1010 different rotation patterns you could try. 
That is a lot!

There are various ways you could try to bring the running time down to a more manageable amount, 
but here is one fact that makes it easy:

•The 1st star you rotate around, 
 the 5th star you rotate around, 
 and the 9th star you rotate around should all be the same.

•The 2nd star you rotate around, 
 the 6th star you rotate around, 
 and the 10th star you rotate around should all be the same.

•The 3rd star you rotate around and the 
 7th star you rotate around should be the same.

•The 4th star you rotate around and the 
 8th star you rotate around should be the same.

 If you realize this, there are only 104 possibilities to try. 
 But why is this fact true? Read on and you'll find out!



 Understanding Rotations



 One of the biggest challenges here is just wrapping your head around the problem. 
 How can you intuitively understand what it means to rotate something 100,000 times? 
 The best way to do this is to roll up your sleeves and write down a couple formulas.
 
 When dealing with rotations and translations of points on a plane, 
 complex numbers provide an excellent notation:

 • A point (x, y) can be represented as x + iy. 
 • Complex numbers can be added and subtracted just like vectors. 
 • Complex numbers can be rotated clockwise 90 degrees about 
   the origin simply by multiplying them by -i.

 It is this last property that makes them so clean to work with, 
 and it is what we will use for the analysis. 
 If you prefer, you could also imagine replacing everything with matrices.
 
 We know that rotating a point P0 by 90 degrees about the origin 
 will send it to -i * P0. 
 However, what happens if we rotate it about a different point Q0? 
 
 There is a standard formula for this situation that you may have already seen. 
 The resulting point P1 satisfies the following:

 P1 - Q0 = -i * (P0 - Q0)

 This is our previous formula applied to the fact 
 that rotating P0 - Q0 by 90 degrees about the origin must give you P1 - Q0. 
 (Do you see why that is true? It's really just a coordinate change.) 
 In our case, it will be helpful to group things a little differently in the formula:
 
 P1 = -iP0 + Q0 * (1 - i)

 Now, suppose we rotate P1 by 90 degrees about another point Q1, 
 then by 90 degrees about another point Q2, and so on. 
 What would happen to this formula? 
 Let's write out a few examples:

 • P2 = -iP1 + Q1 * (1 - i) 
      = -P0 + Q1 * (1 - i) + Q0 * (-1 - i). 

 • P3 = -iP2 + Q2 * (1 - i) 
      = iP0 + Q2 * (1 - i) + Q1 * (-1 - i) + Q0 * (-1 + i).

 • P4 = -iP3 + Q3 * (1 - i) 
      = P0 + Q3 * (1 - i) + Q2 * (-1 - i) + Q1 * (-1 + i) + Q0 * (1 + i).

 • P5 = -iP4 + Q4 * (1 - i) 
      = -iP0 + (Q4 + Q0) * (1 - i) + Q3 * (-1 - i) + Q2 * (-1 + i) + Q1 * (1 + i).
 • etc. 

 From here, it's not too hard to guess the general formula. 
 If the origin is rotated by 90 degrees about Q0, then Q1, and so on, 
 all the way through Qm-1, then the final resulting point Pm is given by:

    (1 - i) * (Qm-1 + Qm-5 + Qm-9 + ...) 
 + (-1 - i) * (Qm-2 + Qm-6 + Qm-10 + ...) 
 + (-1 + i) * (Qm-3 + Qm-7 + Qm-11 + ...) 
 + (1 + i) * (Qm-4 + Qm-8 + Qm-12 + ...)

 And once the formula is written down, it's not too hard to check that it always works. 
 So anyway, is this simpler than the original problem formulation? 

 It may look pretty complicated, but for the most part, 
 you're now just adding points together here, and addition is easier than rotation!



 Pick a Direction



 We want to choose stars Q0, Q1, ..., Qm-1 in such a way 
 as to make the following point as far away from the origin as possible:

   (1 - i) * (Qm-1 + Qm-5 + Qm-9 + ...) 
 + (-1 - i) * (Qm-2 + Qm-6 + Qm-10 + ...) 
 + (-1 + i) * (Qm-3 + Qm-7 + Qm-11 + ...) 
 + (1 + i) * (Qm-4 + Qm-8 + Qm-12 + ...)

 There are different ways of thinking about things from here, but the key is always convex hulls.

 Let X be the point furthest from the origin that we can attain, and more generally, 
 let Xv be the point that is furthest in the direction of v that we can attain. 
 Certainly, X = Xv for some v. 
 
 (This is because X is surely the point that is furthest in the direction of X.) 
 Therefore, it suffices to calculate Xv for all v, 
 and we can then measure which of these is furthest from the origin to get our final answer.

 So how do we calculate Xv for some given v? 
 We want (1 - i) * Qm-1 to be as far as possible in the v direction, 
 or equivalently, we want Qm-1 to be the star that is furthest in the (1 + i)*v direction. 
 Of course, the same thing is true for Qm-5, Qm-9, etc. 
 
 We should choose the same star for all of them. 
 (This is the fact that we used for the small input solution!) 
 Similarly, for Qm-2, Qm-6, etc., we want to choose the star is furthest in the (-1 + i)*v direction. 
 
 In general, we want to choose stars from the original set that 
 are as far as possible in the following directions: (1 + i)*v, (-1 + i)*v, (-1 - i)*v, (1 - i)*v.

 We are now almost done (at least conceptually). 
 First we find the convex hull of the stars and solve for one particular v. 
 Now what happens when we rotate v? 
 
 For a while, nothing will change, but eventually, 
 one of the four directions we are trying to optimize 
 will be perpendicular to an edge of the convex hull, and as a result, 
 the optimal star will switch to the next point on the convex hull. 

 This simulation can be done in constant time at each step, 
 and there will be only O(N) steps since each star choice will rotate once around the convex hull.

 At this point, we are done! 
 We have found every Xv, and we can manually check each of them to see which one is best. 

 The implementation here is tricky, 
 but it is an example of a general technique called rotating calipers. 
 You can find plenty more examples of it online. 
 Or you can always download solutions from our finalists!

 Bonus Question!

 When we generated test data for this problem, 
 we were surprised to find that we could not come up with a case 
 to break solutions which misread "clockwise" as "counter-clockwise" in the problem statement. 
 
 In fact, there is no such case! Can you see why?
*/

#include <math.h>

int INF4 = (int)1e9;

struct Pr4Point
{
	int x;
	int y;
};

struct Pr4Vector
{
	int x;
	int y;
};

/* Vectors A, B are collinear if A = kB, ax = k*bx, ay = k*by */
int IsCollinear(struct Pr4Point prevPoint, struct Pr4Point currPoint, struct Pr4Point nextPoint)
{
	return (currPoint.x - prevPoint.x)/(currPoint.y - prevPoint.y) ==
		   (nextPoint.x - prevPoint.x)/(nextPoint.y - prevPoint.y);
}
/* Removes all collinear points from polygon */
void RemoveCollPoints(
   struct Pr4Point SourcePoints[], 
   int SN,
   struct Pr4Point DestPoints[],
   int* DN)
{
	int i;
	int isCollinear;
    if ((isCollinear = IsCollinear(SourcePoints[SN - 1], SourcePoints[0], SourcePoints[1])) == 0)
           DestPoints[(*DN)++] = SourcePoints[0];
    for (i = 1; i < SN - 1; i++)
	{
	   if ((isCollinear = IsCollinear(SourcePoints[i - 1], SourcePoints[i], SourcePoints[i + 1])) == 0)
           DestPoints[(*DN)++] = SourcePoints[i];
	}
    if ((isCollinear = IsCollinear(SourcePoints[SN - 2], SourcePoints[SN - 1], SourcePoints[0])) == 0)
           DestPoints[(*DN)++] = SourcePoints[SN - 1];
}

int GetNext(int current, int N)
{
	return (current == N) ? current++ : 0;
}
/* cos(a2-a1) = cos(a2)cos(a1) + sin(a1)sin(a2)
 * 
 */
double GetAngleBetweenVectors(struct Pr4Vector Vector1, struct Pr4Vector Vector2)
{
    float Norm1 = sqrt(Vector1.x*Vector1.x + Vector1.y*Vector1.y);
    float Norm2 = sqrt(Vector2.x*Vector2.x + Vector2.y*Vector2.y);

    return acos((Vector1.x*Vector2.x + Vector1.y*Vector2.y) / (Norm1 * Norm2));
}

/* Califfer is a vector with length of 1. 
 * For example: (-1, 0), (1, 0), (0, -1), (0, 1)
 * (cx, cy) - is a califfer. cx*cx + cy*cy = 1
 * Let a is an angle between (cx, cy) and x (0, 1)
 * a = acos(cx / sqrt(cx*cx + cy*cy));
 * If we want to rotate for an angle of b we receive new angle:
 * c_new = a + b
 * c_newx = cos(a + b) = cosacosb - sinasinb;
 * c_newy = sin(a + b) = sinacosb + cosasinb;
 */
void Rotate(struct Pr4Vector* califfer, double angle)
{
	int new_x = califfer->x * cos(angle) - califfer->y * sin(angle);
	int new_y = califfer->y * cos(angle) + califfer->x * sin(angle);
	califfer->x = new_x;
	califfer->y = new_y;
}

double Distance(struct Pr4Vector calipher, struct Pr4Point pointA, struct Pr4Point pointB)
{
    struct Pr4Vector ab;
	double collABAngle;
	double califfAngle;
	ab.x = pointB.x - pointA.x;
	ab.y = pointB.y - pointA.y;
	collABAngle = GetAngleBetweenVectors(calipher, ab);
	return sqrt(ab.x*ab.x + ab.y*ab.y) * sin(collABAngle);
}

int MinimumWidthConvexPolygon(struct Pr4Point ArrPoints[], int N)
{
    struct Pr4Point NewArrPoints[1000];
	int NewN = 0;
	double pi = 4.0 * atan( 1.0 );
	double rotated_angle = 0;
    double min_width = INF4;
	/* Caliper A points along the positive x-axis */
	struct Pr4Vector caliper_a = { 1, 0 };
    /* Caliper B points along the negative x-axis */
    struct Pr4Vector caliper_b = { -1, 0 };
	/* index of vertex with minimum y-coordinate */
	int p_a = 0;
	int p_a_min = INF4;
    /* index of vertex with maximum y-coordinate */
	int p_b = 0;
	int p_b_min = -INF4;
	struct Pr4Vector edge_a;
	struct Pr4Vector edge_b;
	double angle_a;
    double angle_b;
	double angle_min;
	double width = 0;
	int i;

    RemoveCollPoints(ArrPoints, N, NewArrPoints, &NewN);   
    
    for (i = 0; i < NewN; i++)
	{
		if (NewArrPoints[i].y < p_a_min)
		{
            p_a_min = NewArrPoints[i].y;
			p_a = i;
		}

		if (NewArrPoints[i].y > p_b_min)
		{
            p_b_min = NewArrPoints[i].y;
			p_b = i;
		}
	}
    
	while (rotated_angle < pi)
	{
        /* Determine the angle between each caliper 
		 * and the next adjacent edge in the polygon
		 */
		edge_a.x = NewArrPoints[GetNext(p_a, NewN)].x - NewArrPoints[p_a].x;
        edge_a.y = NewArrPoints[GetNext(p_a, NewN)].y - NewArrPoints[p_a].y;

		edge_b.x = NewArrPoints[GetNext(p_b, NewN)].x - NewArrPoints[p_b].x;
        edge_b.y = NewArrPoints[GetNext(p_b, NewN)].y - NewArrPoints[p_b].y;
        
        angle_a = GetAngleBetweenVectors(edge_a, caliper_a);
        angle_b = GetAngleBetweenVectors(edge_b, caliper_b);
		angle_min = (angle_a > angle_b) ? angle_b : angle_a;
        
		/* Rotate the calipers by the smaller of these angles */
		Rotate(&caliper_a, angle_min);
        Rotate(&caliper_b, angle_min);
        
		if (angle_a < angle_b)
		{
			width = Distance(caliper_a, NewArrPoints[p_a], NewArrPoints[p_b]);
			p_a = GetNext(p_a, NewN);
		}
		else
		{
			width = Distance(caliper_b, NewArrPoints[p_b], NewArrPoints[p_a]);
			p_b = GetNext(p_b, NewN);
		}

		rotated_angle = rotated_angle + angle_min;
        if (width < min_width)
          min_width = width;
	}
	return min_width;
}


typedef long double LD;
typedef long long LL;

struct Event 
{
    int index;
    LD ang;
    struct Pr4Point *choice;
};

void Pr4Swap(struct Event Events[], int i, int j)
{
   struct Event temp;
   temp = Events[i];
   Events[i] = Events[j];
   Events[j] = temp;
}

void Pr4HeapSort(struct Event Events[], int start, int end)
{
	int medium, i, j;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
	Pr4Swap(Events, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
		if (Events[i].ang < Events[medium].ang)
	    {
		   Pr4Swap(Events, i, j++);
	    }    
	}
  
    Pr4Swap(Events, start, j - 1);
    Pr4HeapSort(Events, start, medium - 1);
    Pr4HeapSort(Events, medium + 1, end);
}

struct Helper
{
    struct Pr4Point points[100];
	int n;
};

void addPoint(int x, int y, struct Helper* helper) 
{
    helper->points[helper->n].x = x;
	helper->points[helper->n].y = y;
    helper->n++;
}

void goHull(struct Helper* helper) 
{
}

/* We have a point p0 = x + iy and we want to rotate it for 90 degrees
 * ip0 = -i(x + iy) = y - ix.
 * If I want to rotate according to a point q0, I need to move to the coordinate system
 * tied with point q0. p1 - q0 = -i(p0 - q0), p1 - q0 is coordinates of the p1 point in 
 * coordinate system with the center in q0.
 * p1 = q0 - ip0 + iq0 = -ip0 + (1 + i)q0
 * p2 = -ip1 + (1 + i)q1 = -i(-ip0 + (1 + i)q0) + (i + i)q1 = -p0 - i(1 + i)q0 + (i + i)q1 = 
 * -p0 + (1 - i)q0 + (1 + i)q1...
 * Finally we can get a formula:
 *   (1 - i) * (Qm-1 + Qm-5 + Qm-9 + ...) 
 * + (-1 - i) * (Qm-2 + Qm-6 + Qm-10 + ...) 
 * + (-1 + i) * (Qm-3 + Qm-7 + Qm-11 + ...) 
 * + (1 + i) * (Qm-4 + Qm-8 + Qm-12 + ...)
 * If we have point x + iy. Then after applying (1 - i) multiplication 
 * we can get a new formula
 * (1 - i) * (x + iy) = x + iy - i(x + iy) = x + iy - ix - iiy = x + y + i(y - x)
 * (1 + i) * (x + iy) = x + iy + i(x + iy) = x - y + i(y + x)
 * (-1 - i)* (x + iy) = -x - iy - ix - iiy = y - x + i(-x - y)
 * (-1 + i)* (x + iy) = -x - iy + ix - y   = -x - y + i(x - y)
 * (1-i)Q1+(-1-i)Q2+(-1+i)Q3+(1+i)Q4
 * (1-i)(x1+iy1)+(-1-i)(x2+iy2)+(-1+i)(x3+iy3)+(1+i)(x4+iy4)=
 * x1 + y1 + i(y1 - x1) + x2 - y2 + i(y2 + x2) + y3 - x3 + i(-x3 - y3) + -x4 - y4 + i(x4 - y4)
 * We have 4 points: q1, q2, q3, q4.
 * And we need to find maximum of sqrt((q1x+q2x+q3x+q4x)^2 + (q1y+q2y+q3y+q4y)^2) -> max
 * The maximum sum of 4 vectors can be achieved by adding the same vector several times.
 * to choose the longest vector and multiple it 4 times.
 * But in the case when we need to choose 4 different vectors, for example, from n = 20 
 * possible variants.
 * How could we manage it? We could sort all points by angle and take consequently 4 points.
 * And choose the best answer. a0, a1, a2, a3, a4, ...., an-1, an
 * We choose a0, a1, a2, a3
 *               a1, a2, a3, a4
 *                   a2, a3, a4, a5, 
 * Lets prove it. If there is an optimal decision ai, ai+1, ai+2, ai+3
 * Suppose there is a decision  ai, ai+1, ai+3, am  and it is better.
 * then || ai + ai+1 + ai+3 + am || > || ai + ai+1 + ai+2 + ai+3 ||
 */
void Alg()
{
    struct Helper helpers[4];
	struct Event events[1000];
	struct Pr4Point best[4];
	struct Pr4Point NULL = { -666, -666 };
	int x, y, v, w, i, j, t, k;
	int eventN = 0;
	int convhullsize = 0;
	int prev;
	LD pi = 2 * acosl(0);

	int eventIndex;
    LD eventAng;
    struct Pr4Point *eventChoice;
	LD result;
	struct Event MEvent;

	LL sx;
    LL sy;
    int bad = 0;
	int used;
    int m = 0;
    int n = 0;
	int max;

    for (i = 0; i < n; ++i) 
	{
		scanf("%d %d", &x, &y);

        v = x + y;
        w = x - y;
		/* Build convex hull for every of 4 directions:
		 * 1 - i, 1 + i, -1 - i, -1 + i
		 */
        for (j = 0; j < 4; ++j) 
		{
            addPoint(w, v, &helpers[j]);
			helpers[j].n = 1;
            w = -w;
			Pr4Swap(events, &v, &w);
        }

		for (j = 0; j < 4; ++j) 
		{
            goHull(&helpers[j]);
        }

        for (j = 0; j < 4; ++j) 
		{
			convhullsize = helpers[j].n;
			/* For example:
			 * let convhullsize = 4. t = 0,1,2,3
			 * t = 0, t - 1 = -1, (4 - 1)%4 = 3
			 * t = 1, t - 1 = 0, (4 + 0)%4 = 0
			 * t = 2, t - 1 = 1, (4 + 1)%4 = 1
			 * t = 3, t - 1 = 2, (4 + 2)%4 = 2
			 * if t == 0 then pr = (convhullsize - 1) % convhullsize = convhullsize - 1
			 * if t > 0 and t <= convhullsize then pr = t - 1
			 * t = t - 1 impossible. pr == t is possible only when we have convhullsize - 1 == 0
			 * convhullsize == 1 when our convex hull consists only one point.
			 * If we have only one point the angle will be null.
			 * 
			 */
            for (t = 0; t < convhullsize; ++t) 
			{
                prev = (convhullsize + t - 1) % convhullsize;
				if (prev == t) 
				{
					eventAng = 0;
				}
				else
				{
					eventAng = 
						atan2l(helpers[j].points[j].y - helpers[j].points[prev].y,
                               helpers[j].points[j].x - helpers[j].points[prev].x);
				}
				eventChoice = &helpers[i].points[j];
                eventIndex = i;
				/* ang = A. i = -2 
				 * ang1 = A - 4pi;
				 * ang2 = A - 2pi;
				 * ang3 = A - 0 = A;
				 * ang4 = A + 2pi;
				 * ang5 = A + 4pi;
				 */
				for (i = -2; i <= 2; ++i) 
				{
                   eventAng += i * 2 * pi;

				   events[eventN].ang = eventAng;
				   events[eventN].choice = eventChoice;
				   events[eventN].index = eventIndex;

                   eventN++;
                }
			}
		}
    }

    Pr4HeapSort(events, 0, eventN);

    for (i = 0; i < 4; ++i) 
	{
        best[i] = NULL;
    }
    
	result = 0;
    for (i = 0; i < eventN; i++)
	{
		MEvent = events[i];
        best[MEvent.index] = *MEvent.choice;
        
		max = 1;
		if (m - 3 > max)
		   max = m - 3;
        for (k = max; k <= m; ++k) 
		{
            sx = 0;
            sy = 0;
            bad = 1;
            /* m = 8
			 * k = m - 3 = 5
			 * k / 4 = 1, k % 4 = 1
			 * used = 1 + (1 > 0) = 2
			 * used = 1 + (1 > 1) = 2
			 * used = 1 + (1 > 2) = 1
			 * used = 1 + (1 > 3) = 1
			 *
			 * m = 3
			 * 
			 */
			for (i = 0; i < 4; ++i) 
			{
                used = k / 4 + k % 4 > i;
				if (used > 0 && best[i].x == NULL.x && best[i].y == NULL.y) 
				{
                    bad = 0;
                    break;
                }
                if (used > 0) 
				{
                    sx += used * (LL) best[i].x;
                    sy += used * (LL) best[i].y;
                }
			}
			if (bad) 
			{
                continue;
            }
			if (sqrtl(sx * (LD) sx + sy * (LD) sy) > result)
			{
                result = sqrtl(sx * (LD) sx + sy * (LD) sy);
			}
		}
	}
}
