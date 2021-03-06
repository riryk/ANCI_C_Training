/* GCJ F 13 5


Problem

You have been walking in the woods for hours, and you want to go home.


The woods contain N clearings labeled 1, 2, ..., N. 
You are now at clearing 1, and you must reach clearing N in order to leave the woods. 
Each clearing from 1 to N-1 has a left path and a right path leading out to other clearings,
as well as some number of one-way paths leading in. 

Unfortunately, the woods are haunted, and any time you enter a clearing, 
one of the two outgoing paths will be blocked by shifty trees. 

More precisely, on your kth visit to any single clearing:

You must leave along the left path if k is odd.
You must leave along the right path if k is even.
All paths are one-way, so you have no choice at each step: 
you must go forward through the one unblocked outgoing path.

So the first time you are in clearing #1, 
you will leave along the left path. 

If you ever come back to clearing #1 for a second time, 
you would leave along the right path; 
the third time, you'd leave along the left path again; and so on.

You begin at clearing #1, 
and when you get to clearing #N, 
you can leave the woods. 

How many paths do you need to follow before you get out?

Input

The first line of the input gives the number of test cases, T. 
T test cases follow, each beginning with a line containing a single integer N.

N-1 lines follow, each containing two integers Li and Ri. 
Here, Li represents the clearing you would end up at 
if you follow the left path out of clearing i, 
and Ri represents the clearing you would end up at 
if you follow the right path out of clearing i.


No paths are specified for clearing N 
because once you get there, you are finished.


Output

For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the number of paths you need to follow to get to clearing N. 
If you will never get to clearing N, output "Infinity" instead.

Limits

1 ≤ T ≤ 30.
1 ≤ Li, Ri ≤ N for all i.
Small dataset

2 ≤ N ≤ 10.

Large dataset

2 ≤ N ≤ 40.

Sample


Input 
 	
Output 
 
2
4
2 1
3 1
2 4
3
2 2
1 2
Case #1: 8
Case #2: Infinity

Sample Explanation

In the first sample case, your route through the woods will be as shown below:

Paths followed	Clearing	Path direction

0	1	Left
1	2	Left
2	3	Left
3	2	Right
4	1	Right
5	1	Left
6	2	Left
7	3	Right
8	4	-



For the small dataset, 
we can directly simulate the process of walking through the forest. 
How many steps can this take? 

There are 2^10 states the trees in the clearings can be in, 
and 10 possible states for the clearing we are standing in. 

So if we reach the final clearing, 
it can't take more than 10 × 2^10 steps, 
and if we take that many steps without reaching 
the final clearing we know the answer is Infinity.

For the large dataset, 40 × 2^40 steps 
is too many to simulate directly, but can an input case approach this many?

A test case can be designed to make the clearings simulate an N-1 bit counter. 

The first clearing has both paths leading to the second clearing. 

Each clearing after that forms a chain where one path leads to the next clearing,
and one path leads back to the first clearing. 

Whenever the trail leads back to the first clearing,
the states of the clearings give the next N-1 bit number, 
and after all of those numbers have been produced, it can reach the final clearing. 

This will take at least 2^39 steps.



So we need a solution that will not simulate every step individually. 

In the previous example, we spend 2^21-2 steps in the first 
20 clearings between each time we visit any of the last 20 clearings. 

If our program could detect that this always happens, 
it could simulate those 2^21-2 steps without having to do them all individually. 
We will only take 2^20-3 steps from any of the second twenty clearings,
so the total runtime would be reasonable.

An implementation of this approach is to split the clearings 
into two sets A and B of roughly equal size. 

We then use dynamic programming to compute, 

for each location in A and each of the 2^|A| states of the clearings in A, 
how many steps it will take to leave A, 
which clearing in B we will be leaving towards, 
and what state the clearings in A will be left in.


After this DP is done, our 
solution will take an amount of time proportional 
to the number of steps we take from clearings in B 
-- for each of those, we simulate that step, 
and if that takes us to a clearing in A, 
we look up the appropriate result of the DP from a table. 

So to make this solution efficient, 
we need to choose A and B so that only 
a small number of steps are taken from clearings in B.


Find the distance from each clearing to the final clearing, 
where distance is defined as the number of paths you need to take, 
assuming the state of the clearings is optimal. 

If the final clearing cannot be reached from some clearings, 
put those in a separate set. 
If we ever reach one of those, the answer is Infinity.


Choose B to be the closest half of the remaining clearings. 
There can be at most B × 2|B| steps taken from clearings in B, 
because we cannot repeat a state of B without reaching the final clearing.

This can be only at most 20 × 2^20, so this solution is sufficiently fast.



*/

/* The number of clearings in the wood */
#define N 4
#define INF -666
#define PR5_INF 22222222

void ZeroStates(int* States[], int rowCount, int columnCount)
{
   int i, j;
   
   for (i = 0; i < rowCount; i++)
   {
	   for (j = 0; j < columnCount; j++)
	   {
		   States[i][j] = 0;
	   }
   }
}

void SolveSmall()
{ 
   /* This array contains left paths for each point */
   int Left[N] = { 2, 3, 2, -666 };
   /* This array contains right paths for each point */
   int Right[N] = { 1, 1, 4, -666 };
   /* Snapshot description.
    * The state of the forest can be a man's current clearing, where he is 
	* standing in and the number of thye man's visits for every clearings.
	* Bacause the next path depends only on a parity of the number the man has visited 
	* a clearing it is enough to keep only the parity: 1 is odd, 0 is even.
	* We have at most 2^N possible variants: (2^(2N))/(2*...*2 N times)
	*  000...0  till  111...11
	* If we have ai = 1 on ith position the means that a number we have visited this clearing
	* so far is odd, otherwise if we have ai = 0, this number is even.
	* States[i][j] can be 
	*      1(we have already visited this state) 
	*   or 0 (we have not visited it yet)
    */ 
   int States[1<<N][N];
   int currentClear;
   int currentState;
   int currentBit;
   int steps = 0;
   /* Fill in all states with 0 */
   ZeroStates(States, 1<<N, N);
   /* start from clearing 0 */
   currentClear = 0;
   /* start state is 0 (000....00)N times. This means that 
    * number of visits for each clearings is odd (0 from start)
    */
   currentState = 0;
   /* We exit from cycle when we meet the last N-th clearing or
    * when we get into the state we have already visited.
	* In this case we would start spinning and the time is infinity
    */
   while (1 == 1)
   {
	  /* Check if we have reached the last point */
      if (currentClear == N - 1)
	  {
		  break;
	  }
	  /* If we have already been in some state we break with infinity */
      if (States[currentState][currentClear] == 1)
	  {
		  steps = INF;
          break;
	  }
      /* if we have currentState in bit representation: 1001001..011
	   * and we need to find bit value on the currentClear position from 
	   * the start, (N - start + 1) th position from the end
	   * start = currentClear + 1 so (N - currentClear)-th position from 
	   * currentState >> N - currentClear - 1 moves (N - currentClear)-th
	   * to the first position and after applying &1 we can get the bit value
	   */ 
      currentBit = (currentState >> N - currentClear - 1) & 1;
	  /* After reading this bit we need to change its value to opposite 
	   * 1 << N - currentClear - 1 is the number 000100.000 On (N - currentClear - 1)th
	   * we have 1
	   */
      currentState ^= 1 << N - currentClear - 1;
	  /* Move to the next state */
	  currentClear = (currentBit == 1) 
		  ? Left[currentClear] - 1 
		  : Right[currentClear] - 1;

	  steps++;
      States[currentState][currentClear] = 1;
   }
}

/* The number of clearings in the wood for large solution */
#define NL 10
/* This struct describes a forest state in large solution */
struct StateLarge
{
   int clearing;
   int state;
   int visited;
   int stepsToGetOut;
   int clearingAGetFrom;
   int clearingBGetTowards;
   int stepsAreComputed;
};
/* This struct is needed to keep track of visited items */
struct StateTrack
{
   int clearing;
   int state;
};
void ZeroStatesLarge(struct StateLarge* States[], int rowCount, int columnCount)
{
   int i, j;
   
   for (i = 0; i < rowCount; i++)
   {
	   for (j = 0; j < columnCount; j++)
	   {
		   States[i][j].clearing = j;
		   States[i][j].state = i;
		   States[i][j].stepsToGetOut = 0;
		   States[i][j].visited = 0;
           States[i][j].stepsAreComputed = 0;
		   States[i][j].clearingAGetFrom = 0;
           States[i][j].clearingBGetTowards = 0;
	   }
   }
}

/* Here we do not need sorting and binary search because
 * a number of elements in the array is not supposed to be large
 * function returns 1 if true and 0 otherwise
 */
int IsElementInSet(int Set[], int SetCount, int element)
{
   int i;
   for (i = 0; i < SetCount; i++)
   {
	   if (Set[i] == element)
		   return 1;
   }
   return 0;
}

/* Parameters:
 * StateA
 *   For the set A we have |A|*2^|A| different states. And we need to find 
 *   a number of steps to get out of the set A or find out if we go 
 *   spinning. In this case we have found a solution and it is INFINITY.
 */
void SolveDynamicProgramming(
   int A[], 
   int countA, 
   int B[], 
   int countB, 
   int Unreachable[],
   int countUnreachable,
   int Left[],
   int Right[],
   int totalN,
   struct StateLarge* StatesA[])
{
   int i, j;
   int currentClearA;
   int currentStateA;
   int currentBitA;
   int Ai = 1<<countA;
   int Aj = countA;
   /* We need to find a number of steps needed for getting out of the wood
    * for each possible state. Supposing that we started from state 1 and the latest 
	* state before getting out of the set of clearings A is k. The track looks like:
	* State1 -> State2 -> ... -> Statek-1 -> Statek. Then the number of steps needed for
	* getting out of wood A when we started from the state State1 is k, for State2 this is k-1,
	* for State3 this is k-2,...,for Statek this is 1. So it is enough only one cycle to 
	* set correct number of steps for whole track.
    */
   struct StateTrack TrackStatesA[(1<<20)*20];
   int TrackStatesACount = 0;
   /* A and B must be equal parts of clearings array */
   if (countA + countB != totalN)
   	   return;
   /* Fill in all states with 0 */
   ZeroStatesLarge(StatesA, 1<<countA, countA);
   /* We start moving from the clearing 0 */
   currentClearA = 0;
   /* We start moving from zero state */
   currentStateA = 0;
   /* What do we need to achieve. We need for each state in A find the number
    * of steps we need to get out of the set A, a clearing where we leave it in,
	* and a clearing from the set B where we leave towards. 
	* For this we need to loop through all possible states and launch the small 
	* algorythm. Suppose we have chosen one state and launched the small algorythm
	* and this algorythm processes other states and mark them as processed. 
	* So when we choose the next state we need to skip states we have already processed
	* before.
    */
   for (i = 0; i < 1<<countA; i++)
   {
	   for (j = 0; j < countA; j++)
	   {
           /* First of all we need to skip all already computed states */
		   if (StatesA[i][j].stepsAreComputed == 1)
			   continue;

		   currentClearA = StatesA[i][j].clearing;
		   currentStateA = StatesA[i][j].state;

           while (1 == 1)
		   {
			  /* This means that we have reached out of the set A
			   * and clearing currentClear is from the set B
		   	   */
              /* This means that we have reached out of the set A
			   * and clearing currentClear is from the set B
			   */
			  if (IsElementInSet(B, countB, currentClearA) == 1)
			  {
				  struct StateTrack last = TrackStatesA[TrackStatesACount];
				  /* Here we need to update track and set an appropriate number of steps
				   * neede for getting out of the set A.
				   */
				  for (i = 0; i < TrackStatesACount; i++)
				  {
					  struct StateLarge* track = 
						  &StatesA[TrackStatesA[i].state][TrackStatesA[i].clearing];

					  track->stepsToGetOut = TrackStatesACount - i + 1;
					  track->stepsAreComputed = 1;
                      track->clearingAGetFrom = last.clearing;
					  track->clearingBGetTowards = currentClearA;
				  }
				  break;
			  }
			  /* If we have already been in some state we break with infinity.
			   * In this case we will never get out of the set A.
			   */
			  if (StatesA[currentStateA][currentClearA].visited = 1
				  || IsElementInSet(Unreachable, countUnreachable, currentClearA) == 1)
			  {
				  /* Here we need to update track and set an infinity for steps
				   * needed for getting out of the set A.
				   */
				  for (i = 0; i < TrackStatesACount; i++)
				  {
					  struct StateLarge* track = 
						  &StatesA[TrackStatesA[i].state][TrackStatesA[i].clearing];

					  track->stepsToGetOut = INF;
					  track->stepsAreComputed = 1;
				  }
				  break;
			  }
			  /* Adding item into track */
			  TrackStatesA[TrackStatesACount].clearing = currentClearA;
			  TrackStatesA[TrackStatesACount].state = currentStateA;
			  TrackStatesACount++;
		      
			  /* Mark the current state as visited */ 
			  StatesA[currentStateA][currentClearA].visited = 1;

			  currentBitA = (currentStateA >> countA - currentClearA - 1) & 1;
			  /* Move to next state */
			  currentStateA ^= 1 << countA - currentClearA - 1;
			  /* Move to the next clear */
			  currentClearA = (currentBitA == 1) 
				  ? Left[currentClearA] - 1 
				  : Right[currentClearA] - 1; 
		   }
	   }
   }
}
/* After we have found the shortest paths to each vertexes lets choose
 * one vi. vi->previous points to another vertex. We need to prove 
 * that if we have the shortest path v0->v1->....->vi-1->vi to some vertex i
 * than v0->v1->...->vi-2->vi-1 is the shortest one for the vertex i - 1
 * Suppose opposite There exist another shortest path from v0 to vi-1
 * v0->v1'->...->vi-2'->vi-1 is the shortest path from vertex v0 to vertex vi-1
 * That means that distance(v0->v1'->...->vi-2'->vi-1) < distance(v0->v1->...->vi-2->vi-1)
 * but that means that dist(v0->v1'->...->vi-2'->vi-1->vi) <
 * distance(v0->v1->...->vi-2->vi-1->vi). We have got a contradiction.
 */
struct GraphVertex
{
	int id;
	int adjacentNumber;
	int numberOfStepsToAchieve;
    int adjacentVertexes[40];
	int previousVertex;
};

void AddAdjacentVertex(struct GraphVertex* vertex, int newAdjacent)
{
	vertex->adjacentVertexes[vertex->adjacentNumber++] = newAdjacent;
}

void ClearAllGraphVertexes(struct GraphVertex ReverseGraph[40])
{
    int i,j;
    for (i = 0; i < 40; i++)
	{
		ReverseGraph[i].id = -1;
		ReverseGraph[i].adjacentNumber = 0;
		/* When we have a number of steps equal to -1, it means that
		 * this point is not achievable at all and a number of steps to 
		 * move to this point is infinity. 
		 */
		ReverseGraph[i].numberOfStepsToAchieve = PR5_INF;
		/* Prevoius vertex -1 means that this vertex is unreachable */
		ReverseGraph[i].previousVertex = -1;
	}
}

void Pr5SwapMap(struct GraphVertex HeapQueue[40], int i, int j, int Map[])
{
    struct GraphVertex temp = HeapQueue[i];

	Map[HeapQueue[i].id] = j;
	Map[HeapQueue[j].id] = i;

    HeapQueue[i] = HeapQueue[j];
    HeapQueue[j] = temp;
}

void Pr5Swap(struct GraphVertex HeapQueue[40], int i, int j)
{
    struct GraphVertex temp = HeapQueue[i];
    HeapQueue[i] = HeapQueue[j];
    HeapQueue[j] = temp;
}

void AddToQueue(
    struct GraphVertex HeapQueue[40], 
	int* QueueSize, 
	struct GraphVertex newItem,
	int Map[])
{
	int i;
	Map[newItem.id] = &QueueSize;
	HeapQueue[*QueueSize] = newItem;
    (*QueueSize)++;
	/* Then we need to balance heap */
	for (i = &QueueSize; i > 0; i /= 2)
	{
	   if (HeapQueue[i].numberOfStepsToAchieve < HeapQueue[i/2].numberOfStepsToAchieve)
	   {
           Pr5Swap(HeapQueue, i, i/2, Map);
	   }
	   else
	   {
		   break;
	   }
	}
}

struct GraphVertex Pr5ExtractMin(struct GraphVertex HeapQueue[40], int* QueueSize, int Map[])
{
    struct GraphVertex min;
    struct GraphVertex minItem = HeapQueue[0];
	/* Then we need to balance our heap */
	int i;
	int imin;
	/* A[i] must be less than A[2i] and A[2i+1] 
	 *A[i] <= min(A[2i])
	 */
	for (i = 0; i < &QueueSize; i++)
	{
       min = HeapQueue[2*i];
	   imin = 2*i;
	   if (HeapQueue[2*i + 1].numberOfStepsToAchieve < min.numberOfStepsToAchieve)
	   {
		   min = HeapQueue[2*i + 1];
		   imin = 2*i + 1;
	   }
	   if (HeapQueue[i].numberOfStepsToAchieve < min.numberOfStepsToAchieve)
	   {
           Pr5Swap(HeapQueue, i, imin, Map);
	   }
	   else
	   {
           break;
	   }
	}
}

void Pr5DecreaseKey(
  struct GraphVertex HeapQueue[40], 
  int* QueueSize, 
  int Map[],
  int Id,
  int newStepsNumber)
{
   int i;
   int queueIndex = Map[Id];
   if (newStepsNumber > HeapQueue[queueIndex].numberOfStepsToAchieve)
	   return;

   HeapQueue[queueIndex].numberOfStepsToAchieve = newStepsNumber;
   /* We need to balance our heap */
   for (i = queueIndex; i > 0; i++)
   {
	   if (HeapQueue[i].numberOfStepsToAchieve < HeapQueue[i/2].numberOfStepsToAchieve)
	   {
           Pr5Swap(HeapQueue, i, i/2, Map);
	   }
	   else
	   {
		   break;
	   }
   }
}

void Pr5HeapSort(struct GraphVertex ReverseGraph[], int start, int end)
{
	int medium, i, j;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
	Pr5Swap(ReverseGraph, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
	   if (ReverseGraph[i].numberOfStepsToAchieve < ReverseGraph[medium].numberOfStepsToAchieve)
	   {
		   Pr5Swap(ReverseGraph, i, j++);
	   }    
	}
  
    Pr5Swap(ReverseGraph, start, j - 1);
    Pr5HeapSort(ReverseGraph, start, medium - 1);
    Pr5HeapSort(ReverseGraph, medium + 1, end);
}

/* After applying dynamic programming method to the set A we have for every state
 * from A we have a number of steps needed for getting out of the set A and a 
 * clering from B where it will get out towards. After that we run small algorythm
 * for the set B and if any point from B points to A we can easily get an answer using 
 * accumulated data from dynamic programming method.
 * So to make this solution more efficient we need to choose B as close as possible to 
 * the last clearing N-1. 
 * Assume that we did not have any shifting trees. In this case if we have some clearing i.
 * and we can find the shortest path from the vertex i to vertex N-1 using Dijkstra algorythm
 * What happens when we have shifting paths. iR is optimal iL is not optimal
 * with possibility 1/2 optimal way will be closed. We need to choose the closest points 
 * to the destination point. 
 * If there are some clearings which are not rechable from the last point -
 * they will never be rechable in shifting trees.
 */
void ChooseOptimalAB(
   int Left[],
   int Right[],
   int count,
   int A[],
   int* CountA,
   int B[],
   int* CountB,
   int UnReachable[],
   int* CountReach)
{
   int i,j;
   int queueSize = 0;
   /* First of all we need to reverse our graph. Change all edges
    * direction to opposite
    */ 
   struct GraphVertex ReverseGraph[40];
   /* Heap queue for storing all vertexes. */
   struct GraphVertex HeapQueue[40];
   struct GraphVertex MinVertex;
   int Map[40];
   /* Clear all graph vertexes */
   ClearAllGraphVertexes(ReverseGraph);

   for (i = 0; i < count - 2; i++)
   {
	   AddAdjacentVertex(&ReverseGraph[Left[i]], i);
       AddAdjacentVertex(&ReverseGraph[Right[i]], i);
   }
   /* The number of steps needed for moving from the
    * vertex (count - 1) to the vertex (count - 1) is 0
    */
   ReverseGraph[count-1].numberOfStepsToAchieve = 0;
   /* Adding all vertexes into the heap queue */
   for (i = count - 1; i > 0; i--)
   {
       AddToQueue(HeapQueue, &queueSize, ReverseGraph[i], Map);
   }
   /* Go through all vertexes */   
   while (&queueSize > 0)
   {
	   int adj;
	   int newStepsNumber;
       MinVertex = Pr5ExtractMin(HeapQueue, &queueSize, Map);
	   if (MinVertex.numberOfStepsToAchieve == PR5_INF)
	   {
           continue;
	   }
       /* Go through all adjacent vertexes and relax their number to achieve */
	   for (i = 0; i < MinVertex.adjacentNumber; i++)
	   {
		   adj = MinVertex.adjacentVertexes[i];
		   newStepsNumber = ReverseGraph[adj].numberOfStepsToAchieve + 1;
		   /* Relax the vertex */
           if (newStepsNumber < ReverseGraph[adj].numberOfStepsToAchieve)
		   {
               ReverseGraph[adj].numberOfStepsToAchieve = newStepsNumber;
			   ReverseGraph[adj].previousVertex = MinVertex.id;
		   }
	   }
   }
   Pr5HeapSort(ReverseGraph, 0, 0);
   /* Find all unreachable vertexes */
   for (i = count - 1; i > 0; i--)
   {
	   if (ReverseGraph[i].numberOfStepsToAchieve == PR5_INF)
	   {
		   UnReachable[(*CountReach)++] = ReverseGraph[i].id;
	   }
	   else
	   {
		   break;
	   }
   }
   *CountA = (count - *CountReach) >> 1;
   *CountB = count - *CountReach - *CountA;
   /* Find the closest vertexes to the finish point. */
   for (i = 0; i < count - *CountReach; i++)
   {
	   if (i < *CountA)
	   {
           B[i] = ReverseGraph[i].id;
	   }
	   else
	   {
		   A[i] = ReverseGraph[i].id;
	   }
   }
}

/* Using dynamic programming model we divide the ser of clearing into 2 sets:
 * A and B. For example if we have 0, 1, 2, ... , NL-2, NL-1, NL clearings we 
 * have: 
 * A = { 0, 1, 2, ... , [NL/2] }
 * B = { [NL/2] + 1, [NL/2] + 2, ... , NL }
 * medium = [NL/2]
 */
void SolveLarge()
{
   int Count = 10;
   /* This array contains left paths for each point */
   int Left[NL] = { 2, 9, 9, 9, 2, 9, 9, 9, 2, -666 };
   /* This array contains right paths for each point */
   int Right[NL] = { 10, 9, 6, 5, 1, 7, 4, 3, 8, -666 };
   /* This is a boundary between A and B sets */
   int A[40];
   int CountA = 0;
   int B[40];
   int CountB = 0;
   int UnReachable[40];
   int CountReach = 0;
   struct StateLarge StatesA[1<<10][10];

   int StatesB[1<<20][20];
   int currentClear;
   int currentState;
   int currentBit;
   int steps = 0;

   ChooseOptimalAB(
      Left,
      Right,
      Count,
      A,
      &CountA,
      B,
      &CountB,
      UnReachable,
      &CountReach);

   SolveDynamicProgramming(
      A, 
      CountA, 
      B, 
      CountB, 
      UnReachable,
      CountReach,
      Left,
      Right,
      Count, 
	  StatesA);
   
   /* Now we start our small algorythm, slight modified using results
    * of dynamic programming 
    */
   ZeroStates(StatesB, 1<<20, 20);
   currentClear = 0;
   currentState = 0;

   while (1 == 1)
   {
      if (currentClear == Count - 1)
	  {
		  break;
	  }
      if (StatesB[currentState][currentClear] == 1)
	  {
		  steps = INF;
          break;
	  }
      currentBit = (currentState >> Count - currentClear - 1) & 1;
      currentState ^= 1 << Count - currentClear - 1;

	  currentClear = (currentBit == 1) 
		      ? Left[currentClear] - 1 
		      : Right[currentClear] - 1;

	  if (IsElementInSet(A, CountA, currentClear) == 1)
	  {
		  struct StateLarge stateA = StatesA[currentState][currentClear];
		  currentClear = stateA.clearingBGetTowards;
		  steps += stateA.stepsToGetOut + 1;
	  }
	  else
	  {
          steps++;
	  }

      StatesB[currentState][currentClear] = 1;
   }
}