/*




Problem

You are kayaking through a system of underground caves 
and suddenly realize that the tide is coming in and you are trapped! 
Luckily, you have a map of the cave system. 
You are stuck until the tide starts going out, 
so you will be here for a while. 
In the meantime, you want to determine 
the fastest way to the exit once the tide starts going out.

The cave system is an N by M grid. 
Your map consists of two N by M grids of numbers: 
one that specifies the height of the ceiling in each grid square, 
and one that specifies the height of the floor in each grid square. 
The floor of the cave system is porous, 
which means that as the water level falls, 
no water will remain above the water level.

You are trapped at the north-west corner of the map. 
The current water level is H centimeters, 
and once it starts going down, 
it will drop at a constant rate of 10 centimeters per second, down to zero. 
The exit is at the south-east corner of the map. 

It is now covered by water, but it will 
become passable as soon as the water starts going down.

At any time, you can move north, south, east or west 
to an adjacent square with the following constraints:

The water level, the floor height of your current square, 
and the floor height of the adjacent square 
must all be at least 50 centimeters lower than the ceiling height of the adjacent square. 

Note: 
this means that you will never be able to enter a square 
with less than 50 centimeters between the floor and the ceiling.

The floor height of the adjacent square must be 
at least 50 centimeters below the ceiling height of your current square as well.

You can never move off the edge of the map.

Note that you can go up or down as much as you want with your kayak. 
(You're very athletic from all this kayaking!) 
For example, you can go from a square with floor at height 10 centimeters 
to an adjacent square with floor at height 9000 centimeters 
(assuming the constraints given above are met).

These constraints are illustrated below: 

In the first image, 
you can't move to the right because the water level is less than 50 centimeters 
below the ceiling height of the adjacent square.

In the second image, 
you can't move to the right because the floor height of your current square 
is less than 50 centimeters below the ceiling height of the adjacent square.

In the third image, 
you can't move to the right because the floor height of the adjacent square 
is less than 50 centimeters below the ceiling height of the adjacent square. 
You'll never be able to enter that square from any direction.

In the fourth image, you can't move to the right because the floor height 
of the adjacent square is less than 50 centimeters below 
the ceiling height of the current square.

When moving from one square to another, 
if there are at least 20 centimeters of water 
remaining on the current square when you start moving from it, 

it takes 1 second to complete the move (you can use your kayak). 
Otherwise, it takes 10 seconds (you have to drag your kayak). 

Note that the time depends only on the water level in the square you are leaving, 
not in the square you are entering.

It will be a while before the tide starts going out, 
and so you can spend as much time moving as you want before the water starts going down. \
What matters is how much time you will need from the moment the water starts going down 
until the moment you reach the exit. 
Can you calculate this time?


Input

The first line will contain a single integer, T:
the number of test cases

It is followed by T test cases, 
each starting with a line containing integers H, N and M, 
representing the initial water level height, in centimeters, and the map dimensions. 

The following 2N lines contain the ceiling and floor heights as follows:

The next N lines contain M space-separated integers each. 
The jth integer in the ith row represents Cij, 
the height of the ceiling in centimeters at grid location (j, i), 
where increasing i coordinates go South, and increasing j coordinates go East.

The next N lines contain M space-separated integers representing the heights of the floor, 
in the same format.

At the starting location, there will always be at least 50 cm of air 
between the ceiling and the starting water level, and at least 50 cm between the ceiling and the floor.

The exit location will always have at least 50 cm of air between the ceiling and the floor.
There will always be a way out (you got in, after all!).

Output

For each test case, output one line containing 
"Case #x: t", where x is the case number (starting from 1), 
and t is the time, in seconds, starting from when the tide begins going out, 
that it takes you to make your way out of the cave system. 

Answers within an absolute or relative error of 10-6 of the correct answer will be accepted.

Notes

It is possible that you can go through the whole cave system before the tide starts dropping. 
In this case you will be able to wait at the exit for the tide to start dropping, 
so the answer in this case should be zero 
(this is the case in the fourth of the sample test cases).

Limits

Small dataset

1 ≤ T ≤ 50. 
1 ≤ N, M ≤ 10. 
1 ≤ H ≤ 1000. 
1 ≤ Fxy ≤ Cxy ≤ 1000.

Large dataset

1 ≤ T ≤ 50. 
1 ≤ N, M ≤ 100. 
1 ≤ H ≤ 10000. 
1 ≤ Fxy ≤ Cxy ≤ 10000.

Sample


Input 
 	
Output 
 
4
200 1 2
250 233
180 100
100 3 3
500 500 500
500 500 600
500 140 1000
10 10 10
10 10 490
10 10 10
100 3 3
500 100 500
100 100 500
500 500 500
10 10 10
10 10 10
10 10 10
100 2 2
1000 1000
1000 1000
100 900
900 100
Case #1: 11.7
Case #2: 3.0
Case #3: 18.0
Case #4: 0.0

In the first sample test case, 
there are initially only 33 centimeters 
between the water level and the ceiling of the eastern square, 
so after the tide starts going down, 
you have to wait for at 1.7 seconds to enter it. 

Once it is accessible, you can start going in 
- but the water level in the western square is now so low 
(only 3 centimeters above the floor) 
that you have to drag your kayak for the next 10 seconds to get to the exit point.

The initial situation in the second case is better 
- you have a lot of headroom in adjacent squares, 
so you can move, 
for example, to (1, 1) before the tide starts dropping. 

Once there, you have to wait for the tide to start going down, 
and the water level to go down to 90cm (that takes one second). 
Then you can kayak south and then east and get out (in a total of three seconds). 

Note that you cannot go through the cave at (2, 1), 
even though the ceiling there is high enough, 
because there is too little space between the floor of this cave 
and the ceiling of any caves you could try to enter from ((1, 1) and (2, 0)) 
- only 10 centimeters in each case.

The third case is somewhat similar to the first 
- you have to wait at the starting position until the tide goes down to 50cm. 
After that you can kayak for the exit 
- but after three moves (taking three seconds) the water is at 20cm, 
which is only 10cm above the floor, 
which means the fourth move will be dragging instead of kayaking.


In the fourth case you are really lucky! 
You can immediately go the exit, 
even before the tide starts leaving, and wait there.








There is a lot going on in this problem, 
and it is difficult to keep track of everything. 
Only seven hundred contestants managed to get it right, 
so you know it can't have been easy! Let's go over it.

The first thing you should notice is that this is a shortest-path problem. 
We want to get from point A (the start) to point B (the exit) as fast as possible. 

There are a number of classical shortest-path algorithms available; 
we will try to adapt Dijkstra's algorithm here.

Dijkstra's algorithm works on a weighted graph, 
so we need to identify a set of vertices and edges between them to use it. 

This is pretty easy if you have done graph theory before 
- we take the squares to be the vertices, 
and we put an edge between every two adjacent cells. 

We encounter problems when trying to assign weights to the edges: 
the cost (in seconds) of travelling from one square to another is not fixed 

- it can be one or ten seconds 
(and we haven't touched on the issue of moving around before the tide starts going down yet).

A fact that might be somewhat surprising is that this is not a problem for Dijkstra's algorithm. 
Let us briefly recall how it works 

- it maintains a tentative cost for each vertex, 
  and at each step it chooses the vertex with the smallest cost, 
  fixes that to be the real cost of visiting that vertex, 
  and updates the neighbors accordingly. 
  Of course in our case the cost of reaching a vertex is simply the time it takes.

  This means that we need to consider the time of going from some square A 
  to an adjacent square B only once, when we are recalculating the time 
  of reaching B due to having fixed the time for A. 
  
  But since we fixed the time of reaching A, 
  we can easily calculate the water level at this moment 
  - and so we will know the cost of travelling this edge. 
  
  (Note that the earlier we can start moving from A, 
   the faster the move will be. 
   Therefore, we really should be moving as soon as possible instead of waiting for a better moment.)

  There are also other constraints that we have. 
  Let's name two of them 
  - the water level needs to be at least 50 centimeters below the ceiling of the square 
  we want to enter, 
  and the "gap condition" needs to be satisfied. 
  
  For the latter, we can just check 
  - this does not depend on the time we want to travel 
  - and remove the edge from the graph if the condition is not satisfied 
  (we can either do this up-front, 
   or lazily at the moment we try to use this edge). 

  For the former, we need to look at what Dijkstra's algorithm needs again. 
  The cost of travelling the edge AB is used to determine the shortest path to B 
  that goes through A 
  and then directly through the edge AB to B. 
  
  Well, if we really want to take this path, 
  and the water level is too high to enter B when we arrive at A, 
  we have only one choice 
  - we have to wait until the water level drops to CB-50 before moving. 
  
  Remember that this may cause us to have to drag the kayak!

  All this means that we can calculate the cost of each edge 
  at the moment in which we need it, 
  and we would have a complete solution for the problem 
  if not for the extra possibility of moving around 
  before the tide starts going down. 
  
  One might be tempted to solve this issue by a preprocessing phase, 
  where we use a breadth-first search to find all the squares 
  that are reachable from the start in time zero.

  A simpler solution to code, however, 
  is to insert this phase into the Dijkstra's algorithm 
  that works so well for us so far. 
  
  All this extra movement means is that if we want to traverse the edge AB, 
  we are at A in time zero, 
  and B is accessible from A in time zero, 
  then the cost of making this move is zero 
  - as we can make it before the tide starts going down. 
  
  This means that we insert one extra condition in our function 
  that calculates the cost, and we are done!

  Notice that the outcome here is just a standard 
  implementation of Dijkstra's algorithm and nothing more, 
  with all the problem-specific logic inserted into the function 
  that calculates the weight of a given edge at the time it is needed. 
  
  To convince oneself that this works, however, 
  an understanding of the inner workings of the algorithm is needed.


*/



#define INT_MAX       2147483647 
#define INT_MIN     (-2147483647 - 1)

int H;           // water level height
int F[100][100]; // cells floors positions
int C[100][100]; // cells ceilling positions
int rows;        // map dimensions number of rows 
int cols;        // map dimention number of columns

int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, -1, 0, 1};

struct Pr19QItem
{
    int r;
    int c;
    int priority;
};

struct Pr19QItem* Create(int r0, int c0, int t0)
{
    struct Pr19QItem* newItem = (struct Pr19QItem*)malloc(sizeof(struct Pr19QItem));
	newItem->c = c0;
	newItem->r = r0;
	newItem->priority = t0;
	return newItem;
}

void Pr19QueuePush(struct Pr19QItem QItem) {  }

int Pr19IsQueueEmpty() { return -1; }

struct Pr19QItem* Pr19QueueTop() 
{ 
    struct Pr19QItem* newItem = (struct Pr19QItem*)malloc(sizeof(struct Pr19QItem));
	newItem->c = 0;
	newItem->r = 0;
	newItem->priority = 0;
	return newItem;
}

void Pr19QueuePop() {}

static int enter_time(int r1, int c1, int r2, int c2)
{
	int th;
	int minC = (C[r1][c1] < C[r2][c2]) ? C[r1][c1] : C[r2][c2];
	int maxF = (F[r1][c1] > F[r2][c2]) ? F[r1][c1] : F[r2][c2];

    if (minC - maxF < 50)
        return INT_MAX;
    
    th = (C[r2][c2] - 50);
    if (H <= th)
        return INT_MIN;
    else
        return H - th;
}

int Pr19_Dijkstra(int r0, int c0, int t0)
{
	int priorities[100][100];
	int i, j, d;

    for (i = 0; i < rows; i++)
	  for (j = 0; j < cols; j++)
         priorities[i][j] = INT_MAX;

	Pr19QueuePush(*Create(r0, c0, t0));
    while (!Pr19IsQueueEmpty())
    {
        struct Pr19QItem* current = Pr19QueueTop();
        Pr19QueuePop();

		if (current->priority != priorities[current->r][current->c])
            continue;
        /* d = 0: dr = -1, dc = 0
		 * 
		 */
		for (d = 0; d < 4; d++)
        {
			int r2, c2;

            if (d == 0)
			{
				r2 = current->r - 1;
				c2 = current->c;
			}

			if (d == 1)
			{
                r2 = current->r;
				c2 = current->c - 1;
			}

			if (d == 2)
			{
                r2 = current->r + 1;
				c2 = current->c;
			}

			if (d == 3)
			{
                r2 = current->r;
				c2 = current->c + 1;
			}

            if (r2 >= 0 && r2 < rows && c2 >= 0 && c2 < cols)
            {
				int t = enter_time(current->r, current->c, r2, c2);
                if (t != INT_MAX)
                {
					if (t < current->priority)
                        t = current->priority;

                    if (H - t >= F[current->r][current->c] + 20)
                        t += 10;
                    else
                        t += 100;
                    if (t < priorities[r2][c2])
                    {
                        priorities[r2][c2] = t;
						Pr19QueuePush(*Create(r2, c2, t));
                    }
                }
			}
		}
	}
}

void Pr19Solve()
{
	int cases = 10;
    int cas, i, j;

    for (cas = 0; cas < cases; cas++)
    {
         //   cin >> H >> rows >> cols; 
		 for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
				;
                // cin >> C[i][j];   Read cells ceillings positions
  
         for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
				;
                // cin >> F[i][j];   Read cells froors positions
	}
}









