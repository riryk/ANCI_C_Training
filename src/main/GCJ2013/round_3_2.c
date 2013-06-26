
/*


Problem


You have recently purchased a nice big farmyard, 
and you would like to build a fence around it. 
There are already N fence posts in your farmyard.

You will add lengths of fence in straight lines 
connecting the fence posts. 

Unfortunately, for reasons you don't fully understand, 
your lawyers insist you actually have to use all the fence posts, 
or things will go bad.

In this problem, the posts will be represented 
as points in a 2-dimensional plane. 

You want to build the fence by ordering the posts in some order, 
and then connecting the first with the second, second with third, 
and finally the last one with the first. 

The fence segments you create should 
be a polygon without self-intersections. 

That is, at each fence-post there are only two fence segments, 
and at every other point there is at most one fence segment.

Now that's easy, but you also actually want 
to preserve the fact your farmyard is big! 

It's not really fun to wall off most of your farmyard with the fences. 
So you would like to create the fence in such a way 
that the enclosed area is more than half 
of the maximum area 
you could enclose if you were allowed not to use all the posts.



Input



The first line of the input gives the number of test cases, T. 
T test cases follow. 
The first line of each test case contains the number N of posts. 
The posts are numbered from 0 to N - 1. 
Each of the next N lines contains two integers Xi and Yi 
separated by a single space: the coordinates of the i-th post.



Output



For each test case, output one line containing "Case #x: ", 
where x is the case number (starting from 1), 
followed by N distinct integers from 0 to N - 1, 
separated by spaces. 
They are the numbers of the posts, 
in either clockwise or counter-clockwise direction, 
that you will use to build the fence. 
Note that the first and last posts are connected. 


If there are multiple solutions, print any of them.

Limits

The posts will be at N unique points, 
and will not all lie on the same line.

Small dataset

1 ≤ T ≤ 100
3 ≤ N ≤ 10
-100 ≤ Xi, Yi ≤ 100
Large dataset

1 ≤ T ≤ 30
3 ≤ N ≤ 1000
-50000 ≤ Xi, Yi ≤ 50000
Sample


Input 
 	
Output 
 
3
4
1 2
2 0
0 0
1 1
5
0 0
1 1
2 2
0 2
2 0
3
0 0
1 0
0 1
Case #1: 0 1 2 3
Case #2: 0 1 4 2 3
Case #3: 0 2 1

In the first test case, 
there are three polygons we can construct, 
and two of them have a large enough area 
— the ones described by sequences 0 1 2 3 and 0 2 1 3. 
The polygon described by 0 1 3 2 would be too small. 
In the second test case, 
we have make sure the polygon does not intersect itself, 
so, for instance, 0 1 2 3 4 or 0 1 3 4 2 would be bad. 
In the third case, any order describes the same triangle and is fine.





Introduction


In this problem, we are given a set of points on a 2D plane. 
The goal is to construct a single simple polygon that uses all of the points. 
To make things interesting, the area of the resulting polygon has an additional constraint; 
it must be strictly more than half of the area of the largest polygon 
constructed from any subset of points. 

A polygon that takes up maximum area 
but can use a subset of the points is always 
the convex hull of the points.



Small


For the small data case, we choose an exhaustive approach 
that finds the polygon with maximal area. 
Assuming that it is possible to construct a polygon 
with a large enough area 
(see the explanation of the large case for justification), 
the maximal area polygon will be a solution. 
The question now becomes how to construct a polygon of largest area. 
We permute the order of the fence posts. 
For each ordering, we verify the polygon has no intersecting edges 
(being careful of edges that pass through fence posts). 
We keep the polygon with the largest area. 
With N=10 fence posts, this takes no more than O(N! * T), 
where T is how long it takes to validate a polygon and compute its area. 
Verification can be implemented easily in O(N2) time. 
Polygon area can be done in O(N) time. 
So the resulting runtime is O(N! * N^2). 
For N=10, this is around 500 million basic operations, 
which is relatively quick.


Large


For the large case, N = 1000, 
exhausting even a subset of fence post permutations it too costly. 
An algorithm over O(N^3) may take too long to compute. 
We explore a more direct method.


As mentioned earlier, 
the polygon with the largest possible area 
using a subset of the fence posts is the convex hull. 
Suppose we break the convex hull into two polygons, 
one of these polygons must contain at least half of the convex hull’s area. 
Notice that regardless of how the convex hull is split, 
one of the resulting polygons is always at least half of the area.


Suppose we have a way to split the convex hull using the interior fence posts.
This results in a polygon with at least half of the area of the convex hull, 
and at least one isolated vertex on the exterior of the polygon. 
We just need to connect the isolated vertices 
to the polygon without introducing intersecting edges. 
This process could only possibly add more area to the polygon, 
hence the final polygon is strictly more than half of the convex hull’s area.

Making the above observation as the basis of our solution, 
we can arbitrarily split the convex hull into the upper half and lower half.


Note that the union of upper and lower polygon 
contains the full area of the convex hull. 
Furthermore the interior path cannot cause intersecting edges, 
because we chose to sort the points from left to right 
(note, special care must be taken when ties are involved, see special cases). 
One of these polygons has at least half the area of the convex hull, as explained above.


As mentioned, the larger of the two polygons will not contain all of the points. 
However, we can extend the polygon to use all the points. 
This is done by iteratively adding a point to the polygon. 
Because all exterior points are on the convex hull, 
the polygon must increase in area as we do this. 
The points are added to the polygon by taking an exterior point 
and “connecting” it to the point left and right of it 
on the polyline dividing the convex hull. 
For an optimization, 
the exterior points can be added at the same time 
as forming the polyline if you know the exterior points when constructing the polyline.


Special cases


On the left example, the leftmost and rightmost points are connected. 
In this case, the convex hull is split into the full polygon and a line segment. 
The full polygon, a.k.a. the convex hull, can simply be used as the final answer.

The second special case, above on the right, 
can occur when points share the same x coordinate 
and “left to right” is not well defined. 
An elegant way to handle this case is to project all of the points onto 
a “nearly horizontal” line and retain the scalar project values. 
If we pick a line that is not parallel 
or orthogonal to the line formed by any two points in our input, 
then each projected point will have a unique position on the “horizontal” line. 
A line that passes through points (0,0) and (200000,1) will work.


Once all points are projected onto the line, 
the sort order of the points is determined by the projected value. 
If we take a collection of points 
and go through them in this order we form a polyline.


We can show that running this algorithm 
including the two endpoints of one of our half hulls 
and all the middle points will create a closed polygon. 
Notice that when you also expand the middle set 
to also contain all points that are in the half hulls that the polygon grows in size. 
This is because the points added are outside our current polygon 
as a property of the half hull. 
A line coming from the middle points 
will move away from the first polygon towards the hull 
and back to the middle points when following our order. 
This technique ensures that the area of our polygon 
grows bigger than the polygon with just the middle points.

Putting it all together

To sum up the solution, we will reiterate the main steps of our algorithm:

Take the input and turn it into three sets of points cut using line (0,0) to (200000,1):
upper hull
lower hull
middle points
Run the closing algorithm on using upper hull and combining lower hull and middle points.
Run the closing algorithm on using lower hull and combining upper hull and middle points.
Print the solution with the larger polygon.



*/


//#pragma comment(linker, "/STACK:134217728")

//#include <iostream>
//#include <sstream>
//#include <cstdio>
//#include <cstdlib>
//#include <cmath>
//#include <cctype>
//#include <cstring>
//#include <vector>
//#include <list>
//#include <queue>
//#include <deque>
//#include <stack>
//#include <map>
//#include <set>
//#include <algorithm>
//#include <numeric>
//using namespace std;
//
//typedef long long Int;
//typedef pair<int,int> PII;
//typedef vector<int> VInt;
//
//#define FOR(i, a, b) for(i = (a); i < (b); ++i)
//#define RFOR(i, a, b) for(i = (a) - 1; i >= (b); --i)
//#define CLEAR(a, b) memset(a, b, sizeof(a))
//#define SIZE(a) int((a).size())
//#define ALL(a) (a).begin(),(a).end()
//#define PB push_back
//#define MP make_pair
//
//int X[1 << 10];
//int Y[1 << 10];
//int Z[1 << 10];
//int I[1 << 10];
//int B[1 << 10];
//int S[1 << 10];
//int S1[1 << 10];
//int S2[1 << 10];
//
//int dist2(Int x, Int y)
//{
//	return x*x + y*y;
//}
//
//int cross(Int x1, Int y1, Int x2, Int y2)
//{
//	return x1*y2 - y1*x2;
//}
//
//int Cmp(int a, int b)
//{
//	int c = cross(X[a], Y[a], X[b], Y[b]);
//	if(c != 0)
//		return c > 0;
//
//	return dist2(X[a], Y[a]) < dist2(X[b], Y[b]);
//}
//
//Int area(int S[], int n)
//{
//	int i;
//	Int res = 0;
//	FOR(i, 0, n)
//	{
//		int a = S[i];
//		int b = S[(i + 1) % n];
//
//		res += cross(X[a], Y[a], X[b], Y[b]);
//	}
//
//	if(res < 0)
//		res = -res;
//
//	return res;
//}
//
//struct Pair
//{
//	int first;
//	int second;
//};
//
//struct Pair* CreatePair(int ifirst, int isecond)
//{
//    struct Pair* newPair = (struct Pair*)malloc(sizeof(struct Pair));
//	newPair->first = ifirst;
//	newPair->second = isecond;
//	return newPair;
//}
//
//int SolveTest(int test)
//{
//	int n, i, pos;
//	struct Pair* Min;
//	scanf("%d", &n);
//    
//    for (i = 0; i < n; i++)  
//		scanf("%d%d", &X[i], &Y[i]);
//    
//	Min = CreatePair(X[0], Y[0]);
//
//	for (i = 0; i < n; i++)
//		;//Min = min(Min, PII(X[i], Y[i]));
//
//	for (i = 0; i < n; i++)
//	{
//		X[i] -= Min->first;
//		Y[i] -= Min->second;
//	}
//
//	for (i = 0; i < n; i++)
//        I[i] = i;
//
//    for (i = 0; i < n; i++)
//        if (X[i] == 0 && Y[i] == 0)
//			break;
//
//	if (i != 0)
//		;//swap(I[0], I[i]);
//
//	//sort(I + 1, I + n, Cmp);
//
//	pos = n - 1;
//	while(true)
//	{
//		int a = I[n - 1];
//		int b = I[pos];
//
//		if(cross(X[a], Y[a], X[b], Y[b]) != 0)
//			break;
//
//		--pos;
//	}
//
//	reverse(I + pos + 1, I + n);
//
//	S[0] = I[0];
//	S[1] = I[1];
//	int cnt = 2;
//	FOR(i, 2, n)
//	{
//		while(true)
//		{
//			if(cnt < 2)
//				throw 0;
//
//			int a = S[cnt - 1];
//			int b = S[cnt - 2];
//			int c = I[i];
//
//			Int x1 = X[a] - X[b];
//			Int y1 = Y[a] - Y[b];
//			Int x2 = X[c] - X[a];
//			Int y2 = Y[c] - Y[a];
//
//			if(cross(x1, y1, x2, y2) >= 0)
//				break;
//
//			--cnt;
//		}
//
//		S[cnt] = I[i];
//		++cnt;
//	}
//
//	CLEAR(B, 0);
//	FOR(i, 0, cnt)
//		B[ S[i] ] = 1;
//
//	int cnt1 = 0;
//	FOR(i, 0, cnt)
//	{
//		S1[cnt1] = S[i];
//		++cnt1;
//	}
//
//	RFOR(i, n, 0)
//	{
//		int a = I[i];
//		if(B[a] != 0)
//			continue;
//
//		S1[cnt1] = a;
//		++cnt1;
//	}
//
//	FOR(i, 0, n)
//		S2[i] = I[i];
//
//	Int a1 = area(S1, n);
//	Int a2 = area(S2, n);
//	Int a3 = area(S, cnt);
//
//	if(a1 + a2 <= a3)
//		throw 0;
//
//	if(a2 > a1)
//	{
//		FOR(i, 0, n)
//			S1[i] = S2[i];
//	}
//
//	printf("Case #%d: ", test + 1);
//	FOR(i, 0, n)
//		printf("%d%c", S1[i], i == n - 1 ? '\n' : ' ');
//
//	return 0;
//}
//
//int main()
//{
//	//freopen("b.in", "r", stdin);
//	//freopen("b.out", "w", stdout);
//
//	int T, t;
//	char buf[1 << 7];
//	gets(buf);
//	sscanf(buf, "%d", &T);
//
//	for (t = 0; t < T; t++)
//	{
//		//fprintf(stderr, "Solving %d/%d\n", t + 1, T);
//		SolveTest(t);
//	}
//
//	return 0;
//};
