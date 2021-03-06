/* GCJ F 13 3
Long ago, an alien civilization built a giant monument. 
The floor of the monument looked like this:
 ###############
#.............#
#.###########.#
#.#.........#.#
#.#.#######.#.#
#.#.#.....#.#.#
#.#.#.###.#.#.#
#.#.#.#.#.#.#.#
#.#.#.###.#.#.#
#.#.#.....#.#.#
#.#.#######.#.#
#.#.........#.#
#.###########.#
#.............#
###############

Each '#' represents a red tile, and each '.' represents a blue tile. 
The pattern went on for miles and miles 
(you may, for the purposes of the problem, assume it was infinite). 

Today, only a few of the tiles remain.
The rest have been damaged by methane rain and dust storms. 
Given the locations and colours of the remaining tiles, 
can you find the center of the pattern?
 
Input
 
The first line of the input gives the number of test cases, T. 
T test cases follow. 

Each one starts with a line containing N, 
the number of remaining tiles. 

The next N lines each contain Xi, Yi, 
and the tile colour (either '#' or '.').


Output
 
For each test case, output one line containing "Case #c: X Y", 
where c is the case number (starting from 1) 
and (X, Y) is the location of the center of the pattern. 

If there is more than one possible answer, 
output the (X, Y) closest to (0, 0) in Manhattan distance 
(the distance in x, plus the distance in y). 

If there are still ties, output the one with the largest X. 
If there are still ties after that, 
output the one with the largest Y. 

If there is no possible answer, output "Case #c: Too damaged".
 
Limits
 
1 ≤ T ≤ 50.
 The list of coordinates in each test case will not contain duplicates. 

Small dataset
 
1 ≤ N ≤ 100.
 -100 ≤ Xi ≤ 100.
 -100 ≤ Yi ≤ 100. 

Large dataset
 
1 ≤ N ≤ 1000.
 -1015 ≤ Xi ≤ 1015.
 -1015 ≤ Yi ≤ 1015. 

Sample

Input 
Output 

6
 1
 0 0 .
 1
 0 0 #
 3
 0 0 #
 0 1 #
 1 0 #
 5
 50 30 #
 49 30 #
 49 31 #
 49 32 #
 50 32 #
 2
 -98 0 #
 99 50 .
 4
 88 88 .
 88 89 .
 89 88 .
 89 89 .
 

Case #1: 0 0
 Case #2: 1 0
 Case #3: 1 1
 Case #4: 50 31
 Case #5: 1 0
 Case #6: Too damaged
 

Solution:

To begin with, let's try to formalize the rules of forming the pattern of tiles. 

.    - Blue
#, + - Red

If the center is at some position x, y, 
then the red tiles are those in positions x', y' 
for which the number max(|x - x'|, |y - y'|) is odd, 
while the blue tiles are those for which this number is even. 

For x', y' we have: max(|x - x'|, |y - y'|) = 2k + 1

This is because the formula max(|x - x'|, |y - y'|) = C for any C 
describes a square ring around x, y, and the rings alternate color with the parity of C

For the small problem, we can prove that if there exists a solution, 
there exists one with |X| + |Y| < 202. 
Thus, we can check all candidates for the center, 
for each one check whether all the tiles have the right colors, and output the best candidate. 

This will not run in time for the large data, of course, 
as we will have over 1030 candidate centers to check.

Single tile analysis

We may now assume we know the parity of x and y. 
We will simply check all four possibilities, 
finding the best possible choice of the center for each of the four assumptions, 
and then pick the one specified by the tie-breaking rules 
(or output "Too damaged" if none of the four assumptions 
led to finding a viable center for the pattern). 

This makes it easier to analyse the information gained 
by knowing the color of a single tile. 

Suppose the tile at some position x', y' is, say, red. 
This means max(|x - x'|, |y - y'|) has to be odd. 

For x', y' we have: max(|x - x'|, |y - y'|) = 2k + 1

Now, we know the parities of x, y, x' and y', and so:

1. if both x - x' and y - y' are odd, 

   max(|x - x'|, |y - y'|) = 2k + 1
   |x - x'| = 2n + 1  
   |y - y'| = 2n + 1

   then any choice of a center (satisfying the parity requirements for x and y) 
   is going to fit our knowledge;

2. if both x - x' and y - y' are even, 
   
   max(|x - x'|, |y - y'|) = 2k + 1
   |x - x'| = 2n 
   |y - y'| = 2p

   then there is no solution satifying the parity requirements;

3. if, say, x - x' is odd, while y - y' is even, 

   max(|x - x'|, |y - y'|) = 2k + 1
   |x - x'| = 2n + 1
   |y - y'| = 2p 

   we have to have |x - x'| ≥ |y - y'|

If there is any tile of the second type, 
we can immediately return "Too damaged" for these parity assumptions.

We can ignore of tiles of the first type, 
and now we are left only with tiles of the second type.

Note that in the third case, 
since the parities of x - x' and y - y' differ, 
it doesn't matter whether we use a strict inequality, 
as the equality case is eliminated from consideration by the parity assumptions. 

Thus, when considering regions defined by these inequalities, 
we can ignore problems related to "what happens on the edges of these regions", 
as - by the reasoning above - 
the edges will necessarily be eliminated from consideration by the parity assumptions.

The first and second cases are easy to analyse; 

the trick is to find out whether a solution exists 
(and if yes, find the best one) satisfying the set of conditions 
of the type |x - x'| ≥/≤ |y - y'| for various x' and y'. 

Transforming the condition |x - x'| ≥ |y - y'| 
we see it is equivalent to saying that one of the following has to hold:

x + y ≥ x' + y' and x - y ≥ x' - y', or
x + y ≤ x' + y' and x - y ≤ x' - y'.


Dividing the plane

The lines x + y = xi + yi and x - y = xi - yi 
(which are the boundaries of the constraint-satisfaction region for the input tiles) 
divide the plane into at most (N + 1)2 rectangles. 

The idea of our algorithm will be as follows:

1. Iterate over the four sets of parity assumptions about the center.
2. Iterate over all rectangles formed by the boundary lines, 
and for each of them check whether it satisfies the constraints posed by all input tiles.
3. For each rectangle satisfying the constraints, find the best 
   (according to the tie-resolution conditions) center candidate within it (if any)
output the best of all center candidates found.
4. A fun fact is that there will be at most N+1 rectangles that satisfy the constraints; 
so we need not worry overly about the performance of the 
"find the optimal within the rectangle" phase 
(as long as it is independent of the size of the rectangle). 

The naive approach to the second phase is O(N3) 
(for each rectangle check all tiles), 
which with N up to a thousand and 50 testcases risks being too slow,

so we'll need to speed it up a bit.
There are many ways to trim down the runtime of the constraint-checking phase for rectangles. 
One sample way is to process the rectangles "row-by-row", as follows: 

Take the set of rectangles with A ≤ x+y ≤ B, 
with A and B being some two adjacent boundary values. 

For each input tile (out of those that set any constraints on the center position),
we have two areas of constraint satisfaction; 

but only one of them is compatible with A ≤ x+y ≤ B, 
because one of the areas satisfies the constraint x+y ≥ C, 
while the other has x+y ≤ C. 

This means that we know which area is the interesting one for this row; 
so we obtain a constraint on x - y that has to be satisfied by all the rectangles in this row. 
This will be either of the form x - y ≤ D, or x - y ≥ D. 
We take the largest of the lower bounds, 
the largest of the upper bounds,
and obtain a rectangle that we have to check. 

This algorithm runs in O(N2) time, which will be easily fast enough.

A more advanced algorithm (using the sweep line technique) 
can be used to obtain a runtime of O(N logN) runtime. 

We will not describe it (
as it is not necessary to obtain a fast enough program with the constraints given), 
but we encourage the reader to figure it out.


Finding the best point within a rectangle


This was the part of the problem that seemed to cause most difficulties for our contestants. 
There are two cases to consider here. 

Let's assume our rectangle is defined by A ≤ x+y ≤ B and C ≤ x-y ≤ D.

Let us define 
g(k, l) = min(|k|, |l|) if k and l are of the same sign, 0 otherwise.

1. If g(A, B) = 0 and g(C, D) = 0, then the point (0, 0) is within our rectangle. 
   If min(|A|,|B|) = 0 and min(|C|,|D|) = 0

   In this case it suffices to check the near vicinity of the origin. Specifically:

   If both x and y are supposed to be even, 
   (0, 0) is obviously the optimal solution.

   If both x and y are supposed to be odd, 
   then the best four points, in order, 
   are (1, 1), (1, -1), (-1, 1) and (-1, -1). 
   
   If B ≥ 2 we can take (1, 1) and we're done. 
   Otherwise, if D ≥ 2, we take (1, -1); and so on. 
   
   If all the four points are infeasible, 
   the rectangle contains no points satisfying the parity constraints.

   If, say, x is supposed to be odd, while y is even, 
   the first eight candidates are 
   (1, 0), (-1, 0), (3, 0), (1, 2), (1, -2), (-1, 2), (-1, -2), (-3, 0). 
   
   Again, one can check that if none of them is feasible, 
   the rectangle contains no points satisfying the parity constraints. 
   
   The same happens when x is even and y is odd.

   Thus, if (0, 0) is within the rectangle, 
   we can check a constant number of points and take the best feasible one of them.

2. When (0, 0) is not within the rectangle, 
   we first look for the smallest Manhattan distance of any point within the rectangle. 
   
   It is equal to M := max(g(A, B), g(C, D)). 
   As all the boundaries have parities disagreeing with the parity assumptions, 
   the smallest Manhattan distance we can hope for is M + 1. 
   
   We now have an interval of points with Manhattan distance M + 1 in our rectangle, 
   the best one of them is the one with the highest X coordinate 
   (out of the ones fulfilling the parity conditions).
   
   The one last special case to deal with here 
   is when the interval contains only one point, 
   and it has the wrong parities - 
   
   in this case we need to look at distance M + 3 
   (the fact that one never needs to look at M + 5 is left as an exercise).

   It was also possible to solve this problem in a number of other ways. 
   
   A pretty standard one was to identify a number 
   of "suspicious points" within a rectangle 
   (the vicinity of (0, 0), 
    the vicinity of the corners, 
	and the vicinity of places 
	where the coordinate axes intersect the edges of the rectangle) 
	and check them all, taking the best solution.



My own comments:
 . . . .
 + + + .   +  max(|x-x0|, |y-y0|) is odd
 + . + .   .  max(|x-x0|, |y-y0|) is even
 + + + .
*/


/* 3
   0 0 #
   0 1 #
   1 0 #  */

int INF = (int)1e9;
/* For small dataset 
   1 ≤ N ≤ 100.
   -100 ≤ Xi ≤ 100.
   -100 ≤ Yi ≤ 100. 
   Lets suppose that we have found a solution in (x, y) than 
   X must be more closely to the center according to the picture
   |X| + |Y| <= |Xi| + |Yi|
*/
void Small()
{
	int x, y, i, dist, N = 3;
	char expe;
    int X[100] = { 0, 0, 1 };
	int Y[100] = { 0, 1, 0 };
	/* # means max(|xi-x|,|yi-y|) = 2k + 1 odd
	 * . means max(|xi-x|,|yi-y|) = 2k even
	 */
	char C[100] = { '#', '#', '#' }; 
	int cx = INF;
    int cy = INF;
    for (x = -500; x <= 500; ++x) 
	{
        for (y = -500; y <= 500; ++y) 
		{
			for (i = 0; i < N; i++)
			{
				dist = (X[i] >= Y[i]) ? X[i] : Y[i];
				expe = (dist % 2) ? '#' : '.';
				if (C[i] != expe)
				{
                   goto bad;
				}
			}

			if (abs(x) + abs(y) < abs(cx) + abs(cy) 
				|| (abs(x) + abs(y) == abs(cx) + abs(cy) && (x > cx || (x == cx && y > cy)))) 
			{
                cx = x;
                cy = y;
            }

            bad:{}
		}
	}
}

int IsInArray(int Arr[], int n, int Key)
{
    int start = 0;
	int end = n;
    int mid = (start + end) / 2;

	while (start < end)
	{
		if (Arr[mid] < Key)
			start = mid;
		else if (Arr[mid] > Key)
			end = mid;
		else return 1;
	}

	return 0;
}

void Pr3Swap(int Arr[], int i, int j)
{
    int temp = Arr[i];
    Arr[i] = Arr[j];
    Arr[j] = temp;
}

void Pr3HeapSort(int Array[], int start, int end)
{
	int medium, i, j;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
    Pr3Swap(Array, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
       if (Array[i] < Array[j])
	   {
		   Pr3Swap(Array, i, j++);
	   }    
	}
  
    Pr3Swap(Array, start, j - 1);
    Pr3HeapSort(Array, start, medium - 1);
    Pr3HeapSort(Array, medium + 1, end);
}

int IsOdd(char c)
{
   return c == '#';
}

int IsOdd_(int Num)
{
   return Num%2 == 1;
}

int IsEven(char c)
{
   return c == '.';
}

int IsEven_(int Num)
{
   return Num%2 == 0;
}

/* |y - yi| > |x - xi|  
 *
 *  x + y > xi + yi and y - x > yi - xi   or   
 *  x + y < xi + yi and y - x < yi - xi
 *
 *  x + y > sum  and  y - x > diff 
 *  x + y < sum  and  y - x < diff
 *
 *  sum < x + y < sumNext   and  diff < y - x <  diffNext
 *
 */
int RectSutisfy_Y_More_X(int xi, int yi, int sum, int sumNext, int diff, int diffNext)
{
   if (xi + yi < sum && yi - xi < diffNext)
   {
	   /* Desision exists */
       return 1;
   }

   if (xi + yi > sum && yi - xi > diffNext)
   {
       /* Exists desision */
	   return 1;
   }
   /* Rectangle does not sutisfy */
   return 0;
}

/* -1: yi - xi > diffNext
 *  1: yi - xi < diffNext
 */
int DiffRestriction_Y_More_X(int xi, int yi, int sum)
{
   if (xi + yi < sum)
      return 1;
   return -1;
}

/* |x - xi| < |y - yi|
 *
 *  x + y > xi + yi and y - x < yi - xi   or   
 *  x + y < xi + yi and y - x > yi - xi
 *
 *  x + y > sum  and  y - x < diff 
 *  x + y < sum  and  y - x > diff
 *
 *  sum < x + y < sumNext   and  diff < y - x <  diffNext
 *
 */
int RectSutisfy_X_More_Y(int xi, int yi, int sum, int sumNext, int diff, int diffNext)
{
   if (xi + yi < sum && yi - xi > diffNext)
   {
	   /* Desision exists */
       return 1;
   }

   if (xi + yi > sum && yi - xi < diffNext)
   {
       /* Exists desision */
	   return 1;
   }
   /* Rectangle does not sutisfy */
   return 0;
}

/* -1: yi - xi > diffNext
 *  1: yi - xi < diffNext
 */
int DiffRestriction_X_More_Y(int xi, int yi, int sum)
{
   if (xi + yi < sum)
      return -1;
   return 1;
}

//1 - many
//0 - none
//2 - otherwise
int Many_Or_None_Solution_When_X_Odd_Y_Odd(int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsOdd_(yn))  // max(|x - xn|,|y - yn|) are even.
		  return 0; 
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsOdd_(yn) ) // max(|x - xn|,|y - yn|) are even
		  return 1; 
   return 2;
}

//1 - many
//0 - none
//2 - otherwise
int Many_Or_None_Solution_When_X_Even_Y_Even(int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsOdd_(yn))  // |x - xn|, |y - yn| are odd 
		  return 1;
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsOdd_(yn) ) // |x - xn|, |y - yn| are odd 
		  return 0; 
   return 2;
}

//1 - many
//0 - none
//2 - otherwise
int Many_Or_None_Solution_When_X_Odd_Y_Even(int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsEven_(yn))  // |x - xn| is even, |y - yn| is even 
		  return 0;
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsEven_(yn)) // |x - xn|, |y - yn| are odd 
		  return 1; 
   return 2;
}

//1 - many
//0 - none
//2 - otherwise
int Many_Or_None_Solution_When_X_Even_Y_Odd(int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsEven_(yn))  // |x - xn| is odd, |y - yn| is odd
		  return 1;
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsEven_(yn)) // |x - xn| is odd, |y - yn| is odd
		  return 1; 
   return 2;
}

int CheckRectangle_X_Odd_Y_Odd(int Sum[], int i, int Diff[], int j, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsEven_(yn))// |x - xn| is even, |y - yn| is odd  
       return RectSutisfy_Y_More_X(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsEven_(yn)) // |x - xn| is even, |y - yn| is odd  
       return RectSutisfy_X_More_Y(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   return -1; //Some unpredictable error
}

int DiffRestriction_X_Odd_Y_Odd(int Sum[], int i, int xn, int yn, char c)
{ 
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsEven_(yn))// |x - xn| is even, |y - yn| is odd  
       return DiffRestriction_Y_More_X(xn, yn, Sum[i]);
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsEven_(yn)) // |x - xn| is even, |y - yn| is odd  
       return DiffRestriction_X_More_Y(xn, yn, Sum[i]);
   return 0; //Some unpredictable error
}

int CheckRectangle_X_Even_Y_Even(int Sum[], int i, int Diff[], int j, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsEven_(yn)) // |x - xn| is odd, |y - yn| is even
       return RectSutisfy_X_More_Y(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsEven_(yn))// |x - xn| is odd, |y - yn| is even 
       return RectSutisfy_Y_More_X(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   return -1; //Some unpredictable error
}

int DiffRestriction_X_Even_Y_Even(int Sum[], int i, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsEven_(yn)) // |x - xn| is odd, |y - yn| is even
       return DiffRestriction_X_More_Y(xn, yn, Sum[i]);
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsEven_(yn))// |x - xn| is odd, |y - yn| is even 
       return DiffRestriction_Y_More_X(xn, yn, Sum[i]);
   return 0; //Some unpredictable error
}

int CheckRectangle_X_Odd_Y_Even(int Sum[], int i, int Diff[], int j, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsOdd_(yn))// |x - xn| even, |y - yn| are odd 
       return RectSutisfy_Y_More_X(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsOdd_(yn)) // |x - xn| even, |y - yn| are odd 
	   return RectSutisfy_X_More_Y(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   return -1; //Some unpredictable error
}

int DiffRestriction_X_Odd_Y_Even(int Sum[], int i, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsOdd_(yn))// |x - xn| even, |y - yn| are odd 
       return DiffRestriction_Y_More_X(xn, yn, Sum[i]);
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsOdd_(yn)) // |x - xn| even, |y - yn| are odd 
	   return DiffRestriction_X_More_Y(xn, yn, Sum[i]);
   return 0; //Some unpredictable error
}

int CheckRectangle_X_Even_Y_Odd(int Sum[], int i, int Diff[], int j, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsOdd_(yn)) // |x - xn| odd, |y - yn| even 
       return RectSutisfy_X_More_Y(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsOdd_(yn)) // |x - xn| odd, |y - yn| even
       return RectSutisfy_Y_More_X(xn, yn, Sum[i], Sum[i + 1], Diff[j], Diff[j + 1]); 
   return -1; //Some unpredictable error
}

int DiffRestriction_X_Even_Y_Odd(int Sum[], int i, int xn, int yn, char c)
{
   // max(|x - xn|,|y - yn|) is odd
   if (IsOdd(c) && IsOdd_(xn) && IsOdd_(yn)) // |x - xn| odd, |y - yn| even 
       return DiffRestriction_X_More_Y(xn, yn, Sum[i]);
   // max(|x - xn|,|y - yn|) is even
   if (IsEven(c) && IsOdd_(xn) && IsOdd_(yn)) // |x - xn| odd, |y - yn| even
       return DiffRestriction_Y_More_X(xn, yn, Sum[i]);
   return 0; //Some unpredictable error
}

struct Rectangle
{
	int sum;
	int sumNext;
	int diff;
	int diffNext;
};

int FindTheBestRectangles_Naive(
    int SumSet[], 
	int SumSetCount, 
	int DiffSet[], 
	int DiffCount,
	int X[], int Y[], char C[], int N,
	struct Rectangle Rectangles[], int* RectN,
	int (*CheckRectangle)(int Sum[], int i, int Diff[], int j, int xn, int yn, char c))
{
	int isRectangleValid;
	int i;
	int j;
	int n;
     /*
	  * And now we need to go through all rectanges 
	  * Go through all rows. Each row can be identifieed by two adjacent 
	  * sums: A <= x + y <= B So loop through all SumSet
	  * diffs: C <= y - x <= D So loop through all DiffSet
	  */
     for (i = 0; i < SumSetCount - 1; i++)
	 {
        for (j = 0; j < DiffCount - 1; j++)
		{
			/* Here we have found some rectangle. And we need to check 
			 * if it sutisfies all input points
			 */
            for (n = 0; n < N; n++)
			{
				isRectangleValid = CheckRectangle(
					SumSet, 
					i, 
					DiffSet,
					j, 
					X[n], Y[n], C[n]);
				/* Some unpredictable error. Need to investigate 
				 * previous parts of the algorythm
				 */
				if (isRectangleValid == -1)
				  return -1;
                /* 0 means that this rectangle does not sutisfy
				 * constraints. We breaks from the cycle
				 */
				if (isRectangleValid == 0)
				   break;
			}
            /* This means that previous cycle has been performed to the end.
			 * And it has not broken that means that the rectangle is correct.
			 */
			if (n == N)
			{
				Rectangles[*RectN].sum = SumSet[i];
				Rectangles[*RectN].sumNext = SumSet[i + 1];
		
				Rectangles[*RectN].diff = DiffSet[j];
				Rectangles[*RectN].diffNext = DiffSet[j + 1];

                (*RectN)++;
			}
		}
	 }
}

int FindTheBestRectangles_Advanced(
    int SumSet[], 
	int SumSetCount, 
	int DiffSet[], 
	int DiffCount,
	int X[], int Y[], char C[], int N,
	struct Rectangle Rectangles[], int* RectN,
	int (*DiffRestriction)(int Sum[], int i, int xn, int yn, char c))
{
	int i;
    int n;
	int diffRestriction;
	int min = INF;
	int max = -INF;
	/* We go through all rows. SumSet is sorted in ascendant mode. */
    for (i = 0; i < SumSetCount - 1; i++)
	{
	   /* Then go through all critearias and weed out those which 
	    * do not sutisfy the conditions.
		* For every parity assumption and for every tuple point color 
		* and coordinates parity we have a constraints: RectSutisfy_X_More_Y
		* or RectSutisfy_Y_More_X. Each of these constraints set restrictions for 
		* the current row [sum...sumNext] xi + yi > or < sum. For each constraint 
		* we have constraints for diffNext: yi - xi > diffNext or yi - xi < diffNext
		* So we need to solve a system of inequalities: 
		* diffNext < yi - xi, i = 1..k or diffNext > yi - xi, i = 1..m
		* And we need to find a point were all inequalities are intersected.
		* For this we need to solve a system of two inequalities:
		* diffNext < min(yi - xi) and diffNext > max(yi - xi)
		* This system of inequalities can have only one solution or no solutions
		* We need to find min or max restriction.
	    */
       for (n = 0; n < N; n++)
	   {
            diffRestriction = DiffRestriction(
					SumSet, 
					i, 
					X[n], Y[n], C[n]);
			/* Some unpredictable error. Need to investigate 
			 * previous parts of the algorythm
			 */
			if (diffRestriction == 0)
				  return -1;
            /* 1 means that diffNext > yi - xi. In this case we need to find 
			 * max value.
			 */
			if (diffRestriction == 1)
			{
               if (Y[n] - X[n] > max)
			   {
                   max = Y[n] - X[n];
			   }
			}
            /* -1 means that diffNext < yi - xi. In this case we need to find 
			 * min value.
			 */
			if (diffRestriction == -1)
			{
               if (Y[n] - X[n] < min)
			   {
				   min = Y[n] - X[n];
			   }
			}
	   }
	   /* Some unpredictable error. Need to investigate 
	    * previous parts of the algorythm
		*/
       if (max == -INF || min == INF)
		   return -1;
       /* Only in this case a solution is possible */
	   if (max < min)
	   {
           Rectangles[*RectN].sum = SumSet[i];
		   Rectangles[*RectN].sumNext = SumSet[i + 1];
		
		   Rectangles[*RectN].diff = max;
		   Rectangles[*RectN].diffNext = min;

           (*RectN)++;
	   }
	}
}

struct Point
{
	int X;
	int Y;
};

int Sign(int k, int l)
{
	if (k > 0 && l > 0 || k < 0 && l < 0)
	{
		return abs(k) < abs(l) ? abs(k) : abs(l);
	}
	return 0;
}

/* M := max(g(A, B), g(C, D)) */
int GetManhatanDistance(struct Rectangle Rect)
{
	return (Sign(Rect.sum, Rect.sumNext) >  Sign(Rect.diff, Rect.diffNext))
		? Sign(Rect.sum, Rect.sumNext) 
		: Sign(Rect.diff, Rect.diffNext); 
}

int InsideRectangle(struct Rectangle Rect, int CPointX, int CPointY)
{
	return CPointY + CPointX > Rect.sum 
		&& CPointY + CPointX < Rect.sumNext
		&& CPointY - CPointX > Rect.diff
		&& CPointY - CPointX < Rect.diffNext;
}

void ChooseCenter(
        struct Rectangle Rect, 
	    struct Point* Center, 
	    int ChallengerX, 
		int ChallengerY)
{   
    int insideRectangle;
	int isChallengerBetter;
    insideRectangle = InsideRectangle(Rect, ChallengerX, ChallengerY);
    /* If a challenger point is not inside, we do not check it */
    if (insideRectangle == 1)
		return;
    /* Challenger is better when Manhatan distance is small
	 * or when Manhatan distances are equal then we choose that which x is more than previous.
	 * or when Manhatan distances are equal and x are equal than 
	 * we choose that which y is more than previous.
	 */
	isChallengerBetter = 
		abs(ChallengerX) + abs(ChallengerY) < abs(Center->X) + abs(Center->Y)
	 || (abs(ChallengerX) + abs(ChallengerY) == abs(Center->X) + abs(Center->Y)
	 && (ChallengerX > Center->X || (ChallengerX == Center->X && ChallengerY > Center->Y)));
    /* If challenger is better we change our center */
     if (isChallengerBetter == 1)
	 {
		 Center->X = ChallengerX;
		 Center->Y = ChallengerY;
	 }
}

/* If it is not possible to find a point appropriate for center, function 
 * returns 0 else 1.
 */
int FindPointInRectangle_X_Odd_Y_Odd(struct Rectangle Rect, struct Point* Center)
{
	/* If A and B are of different signs and C and D are of different signs 
	 * that means that we need to check points (-1, -1), (-1, 1), (1, -1), (1, 1)
	 * if it of course are covered by the rectangle.
	 */
    if (Sign(Rect.sum, Rect.sumNext) == 0 && Sign(Rect.diff, Rect.diffNext) == 0) 
	{
		ChooseCenter(Rect, Center, -1, -1);
        ChooseCenter(Rect, Center, -1, 1);
        ChooseCenter(Rect, Center, 1, -1);
        ChooseCenter(Rect, Center, 1, 1);
		return 1;
	}
}
/* 0 - failure
 * 1 - success
 */
int FindPointInRectangle_X_Even_Y_Even(struct Rectangle Rect, struct Point* Center)
{
    if (Sign(Rect.sum, Rect.sumNext) == 0 && Sign(Rect.diff, Rect.diffNext) == 0) 
	{
		Center->X = 0;
		Center->Y = 0;
		return 1;
	}  
}

int FindPointInRectangle_X_Odd_Y_Even(struct Rectangle Rect, struct Point* Center)
{
    if (Sign(Rect.sum, Rect.sumNext) == 0 && Sign(Rect.diff, Rect.diffNext) == 0) 
	{
		ChooseCenter(Rect, Center, 1, 0);
        ChooseCenter(Rect, Center, -1, 0);
        ChooseCenter(Rect, Center, 3, 0);
        ChooseCenter(Rect, Center, 1, 2);
        ChooseCenter(Rect, Center, 1, -2);
        ChooseCenter(Rect, Center, -1, 2);
        ChooseCenter(Rect, Center, -1, -2);
        ChooseCenter(Rect, Center, -3, 0);
		return 1;
	}
}

void RestorePicture()
{
	int isInArray;
	int i, j, n;
	int isSolution;
	int s0, d0;
	int s, d;
	int N = 3;
	int NN = 0;
	int X[100] = { 0, 0, 1 };
	int Y[100] = { 0, 1, 0 };
    int XX[100];
    int YY[100];
	/* # means max(|xi-x|,|yi-y|) = 2k + 1 odd
	 * . means max(|xi-x|,|yi-y|) = 2k even
	 */
	char C[100] = { '#', '#', '#' };
    char CC[100];
	char CRest;
	int XRest;
	int YRest;
	int SumSet[100];
	int SumSetCount = 0;
	int DiffSet[100];
	int DiffCount = 0;
	int parityAssumption;
    /* We have N points (xi, yi) 0<=i<=N-1 
	 * We are looking for a centre point (x, y)
	 * 1. Lets assume that ith point is # we have max(|xi-x|,|yi-y|) = 2k + 1
	 * A parity of xi, yi we know. Lets assume that we know a parity of x, y. So
	 * 1.1 |xi-x|,|yi-y| = 2k+1. Every solution which sutisfies the parity of x and y is fit
	 *                           We miss this case because we need more restricted case.
	 * 1.2 |xi-x|,|yi-y| = 2k. There are no solutions. We also omit this case because we will
	 *                         never find a solution.
	 * 1.3 |xi-x|=2k+1, |yi-y| = 2p. In this case we need |xi-x|>=|yi-y|.
	 * This inequality can be written: x'=xi-x, y'=yi-y
	 * |x|>=|y|                                                Y
	 * x >= 0, y >= 0: x >= y   => y <=              ..y=-x \  |  / y = x ......
	 * x < 0, y < 0  : -x >= -y => y >=x             ....... \ | /..............
	 * x < 0, y >= 0 : -x >= y  => y <= -x           _________\|/____________X
	 * x >= 0,y <= 0 : x >= - y => y >= -x           .......  /|\...............
	 *                                               ....... / | \..............
	 *                                               ......./  |  \...........
	 * |x-x'|>=|y-y'|                                     
     * x >= x', y >= y': x - x' >= y - y' => y <=  x + y' - x'
     * x < x', y < y'  : x' - x >= y'- y =>  y >=  x + y' - x'         
     * x < x', y >= y' : x' - x >= y - y' => y <= -x + y' + x'      
     * x >= x ,y <= y' : x - x' >= y'- y =>  y >= -x + y' + x' 
	 * In this case we move central point (0, 0) to the (x', y') and graph is the same.
	 * We hold y' + x' and y' - x' in temporary arrays
	 */
    
	 /* Before starting our algorythm we need to add 2 restrictive points 
	  * not to loose very important areas
	  */ 
	 /* x odd and y odd */
	 if (parityAssumption == 0)
	 {
		 // max(|x - xn|,|y - yn|) is odd
		 CRest = '#';
	     XRest = 0;
	     YRest = -1000;
	 }

	 /* Filter all tuple points 
	  * For every parity assumption go through all tuples and: 
	  * 1. If there is a situation with no solution break
	  * 2. Skip all tuples which accept all possible solutions 
	  */
	 for (i = 0; i < N; i++)
	 {
        isSolution = Many_Or_None_Solution_When_X_Odd_Y_Odd(X[i], Y[i], C[i]);
		if (isSolution == 0)
			return; //This parity assumption is incorrect.
		if (isSolution == 2)
			continue; //Too many solution. We need to skip this.
		XX[NN] = X[i];
		YY[NN] = Y[i];
		CC[NN] = C[i];
		NN++;
	 }

     for (i = 0; i < NN; i++)
	 {
        isInArray = IsInArray(XX, SumSetCount, YY[i] + XX[i]);
        if (isInArray = 0)
		{
            SumSet[i] = YY[i] + XX[i];
            SumSetCount++;
		}
        
        isInArray = IsInArray(XX, DiffCount, YY[i] - XX[i]);
        if (isInArray = 0)
		{
            DiffSet[i] = YY[i] - XX[i];
            DiffCount++;
		}
	 }
	 Pr3HeapSort(SumSet, 0, SumSetCount);
	 Pr3HeapSort(DiffSet, 0, DiffCount);
}
