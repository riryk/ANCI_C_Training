
/*
GCJ F 13 1
Problem1

Description
 
You are playing Zombie Smash: 
a game where the objective is to smash zombies with your trusty Zombie Smasher 
as they pop out of graves at the graveyard. 

The graveyard is represented by a flat 2D grid. 
Each zombie will pop out of a grave at some (X, Y) cell on the grid, 
stand in place for 1000 milliseconds (ms), 
and then disappear back into the grave. 
At most one zombie can stand around a grave at a time.
 
You can move to any one of the 8 cells adjacent to your location in 100ms; 
i.e., you can move North, East, South, West, NW, NE, SW, and SE of your current location.
You may move through or stand on a cell even if it is currently occupied by a zombie. 

You can smash a zombie instantly once you reach the cell that the zombie is standing on,
but once you smash a zombie it takes 750ms for your 
Zombie Smasher to recharge before you can smash another zombie. 

You may move around while Zombie Smasher is recharging. 

For example, immediately after smashing a zombie at (0, 0):
  •  It will take 750ms to reach and smash a zombie at (1, 1) or 
  •  2000ms to reach and smash a zombie at (20, 20). 


You start at cell (0, 0) at the beginning of the game (time=0).
After you play a level you would like to know how many zombies you could have smashed,
if you had played optimally.



Input
 
The first line will contain a single integer T, the number of test cases. 
It is followed by T test cases, 
each starting with a line containing a single integer Z, the number of zombies in the level.
 
The next Z lines contain 3 space-separated integers each, 
representing the location and time at which a given zombie will appear and disappear. 

The ith line will contain the integers Xi, Yi and Mi, where:

 • Xi is the X coordinate of the cell at which zombie i appears, 
 • Yi is the Y coordinate of the cell at which zombie i appears, 
 • Mi is the time at which zombie i appears, in milliseconds after the beginning of the game. 
 
The time interval during which the zombie can smashed is inclusive: 
if you reach the cell at any time in the range [Mi, Mi + 1000] with a charged Zombie Smasher, 
you can smash the zombie in that cell. 


Output
 
For each test case, output one line containing "Case #c: d", 
where c is the case number (starting from 1), 
and d is the maximum number of zombies you could have smashed in this level.
 
Limits
 
 1 ≤ T ≤ 100.
 -1000 ≤ Xi, Yi ≤ 1000.
 0 ≤ Mi ≤ 100000000 = 108.

 Two zombies will never be in the same location at the same time. 
 In other words, if one zombie appears at (x, y) at time t, 
 then any other zombie that appears at (x, y) 
 must appear at or before (t - 1001), or at or after (t + 1001).
 
Small dataset
 
1 ≤ Z ≤ 8.
 
Large dataset
 
1 ≤ Z ≤ 100.
 
Sample
 
 3

 4

 1 0 0
 -1 0 0
 10 10 1000
 10 -10 1000

 3

 1 1 0
 2 2 0
 3 3 0

 5

 10 10 1000
 -10 10 1000
 10 -10 1000
 -10 -10 1000
 20 20 2000
 

*/





/*
  Possible mathematical solution

  Firstly, it is worth noting that the small dataset with only 8 zombies 
  can be solved simply by evaluating each possible permutation for the order in which 
  to smash zombies and keeping the best one. 
  
  For each permutation, simply attempt to smash the zombies in the order given, 
  skipping any zombies that cannot be reached on time. 
  This simple approach has exponential time complexity and clearly will not scale for the large data set.


  For the more efficient approach, 
  let us start by considering the game state represented in an inefficient way 
  and see how it can be made more efficient. 
  
  We can represent the game state at any point in time with the following tuple:

  • (Time, Location, Set of zombies already smashed) 

  This certainly works - given this state, we essentially have the snapshot of the game 
  at any point in time, but this is very verbose. 
  
  Since we want to smash as many zombies as possible, 
  we want to smash zombies as soon as possible, 
  i.e. we want to arrive at the grave they pop out of as soon as possible. 
  
  With that in mind, we can make the following assumption: 

  we will arrive at the grave of the next zombie as soon as possible, 
  and potentially stand around waiting until that zombie can be smashed. 
  
  Once smashed, we will move to the next grave as quickly as we can and repeat the process. 
  That way, it becomes unnecessary to keep track of any states 
  where we are transiting between two graves 
  because those states can be derived from the states at the origin and destination graves. 
  
  We can change the state to be: 

  • (Time last zombie was smashed, Last zombie smashed, Set of zombies already smashed)

  This is an improvement, 
  but now the problem is that we keep track of all possible sets of zombies already smashed. 
  
  Let’s see what we can do about that.
 
  Consider the state (T1, Z1, {Z1 …}) 
  where we have just smashed zombie Z1 at time T1. 
  
  The set of zombies already smashed contains Z1, 
  and possibly a bunch of other zombies. 
  
  Suppose that Z0 is a zombie that we have smashed earlier. 
  
  There are two cases: 
  either Z0 appears at an interval overlapping with Z1, 
  or it has already appeared before Z1.

  1. If Z0 has already appeared before Z1 
     then by T1 it can no longer be at its grave 
	 (even if we haven’t smashed it) 
	 and explicitly tracking that it has already been smashed is unnecessary.

  2. Otherwise, if Z0 appears in an interval overlapping with Z1, 
     is it possible that we will attempt to smash Z0 again, if we don’t keep track of it? 
	 
	 Suppose that Z0 was smashed at T0, 
	 since the Zombie Smasher needs to recharge twice, 
	 Z0 will be already gone because it stands around for 1000ms 
	 and it takes 1500ms for the Zombie Smasher to recharge twice. 

	 Again, it is not necessary to explicitly track the set of zombies smashed 
	 to avoid smashing the same zombie twice.

  In light of the above observations we can simplify the state to be: 

  •(Time last zombie was smashed, Last zombie smashed, Number of zombies already smashed)

  It is easy to see that we prefer earlier times over later times for smashing a zombie 
  - the sooner we smash a zombie, the sooner we can move on to the next one, 
  so we are only interested in states with minimal time possible. 
  
  Let us model the state transitions as a graph and minimize on time.
 
  The game starts at time 0 and location (0, 0). 
  Based on this information, 
  we can generate the initial frontier of zombies that can be reached and smashed on time. 
  
  Given this frontier, the times and the locations at which zombies will appear, 
  we can apply a modified Dijkstra’s Algorithm to find the set of game states that are reachable. 
  
  Once we know those, we can simply return the maximum number of zombies smashed in a reachable state. 
  Here is the pseudo-code:
 
 generateStates():
  states = {}
  states.Add(0, nil, 0) // Include the initial state.
  for each zombie z:
    for zombies_killed from 1 to Z:
      // For other reachable states this will be revised later.
      earliest_smash_time = infinity
      if zombiles_killed = 1:
        earliest_arrival_time = dist((0, 0), z)
        if earliest_arrival_time ≤ z.appearance + 1000:
          earliest_smash_time = max(z.appearance,
                                    earliest_arrival_time)
      states.Add(earliest_smash_time, z, zombies_killed)
  return states


*/

/* This is the time while a corpse is standing before he lie down into a grave
 * It can be illustrated:
 * ........ is in the grave .....|... out of the grave ...| .. is in a grave 
 *                              t0                        t1
 * t1 - t0 = const = CORPSE_STAND_DURATION
 * There is a restriction that only one zombie can stand in time. The process can be illusrtated
 * .... means that corpse is in grave
 * #### means he is standing around a grave
 * Z1 .......|########|..........................
 * Z2 ..............|########|...................
 * Z3 ...................|########|..............
 *   t0      t1     t2  t3   t4   t5
 */
#define CORPSE_STAND_DURATION 1000

/* Speed, with which an alive person can move through a graveyard's cell
 * Example:  North, East, South, West, NW, NE, SW, and SE 
 *
 *  1 --- 2 --- 3   From position 5 we can move tp every adjacent position for the 
 *  |   N |     |   time SPEED
 *  |_W___|___E_| 
 *  4     5     6
 *  |   S |     |
 *  7_____8_____9
 */
#define SPEED 100
/* The time to recharge a rifle before smashing another zombi 
 * Example:   to reach two different edges of rectangle [6 X 2] we 
 * |_| | | | |  need: min {6, 2} * speed + (max {6, 2} - min {6, 2}) * speed
 * |_|_|_|_|_|  = (min {6, 2} + max {6, 2} - min {6, 2}) * speed
 * 1 2 3 4 5 6  = max {6, 2} * speed
 * Let's analyze and example:
 * 10 10 1000
 * -10 10 1000
 * 10 -10 1000
 * -10 -10 1000
 * 20 20 2000
 *           |
 *           |   .20
 *      -10. | .10   
 * ----------|-------------------
 *      -10. | .-10
 *           |
 */
#define RECHARGE_TIME 750
/* If we are at point [0, 0] we can build all possible points we could reach
 * For example: (x0,y0,t0) Set = (xi,yi,ti) i = 1,..,n - the set of all available 
 * corpses. We can compute time we could reach Ti
 * Ti = t0 + max {xi - x0, yi - y0} * Speed <= ti + StandDuration
 * and choose a restriction: Ti <= ti + StandDuration.
 * 
 */

/* A game snapshot:
 * { Time, Location, Set of zombies already smashed }
 * We are not interested in transitting graves. Only source and destination.
 * where we can smash a zombie
 * { Time, Last zombie smashed, Set of zombies already smashed }
 * in Ti we have a game state:
 * { Ti, Zi, {Zi, Zi-1,... Z0} } 
 * Lets take some zombiw from the set {Zi, Zi-1,... Z0}
 * Z0: have been smashed before Zi: T0 + 1000 < Ti and it has already disappeared
 * It does not influence on our strategy because we will never smash it
 * if T0 + 1000 >= Ti and in the moment when we are killing i zombie, 0 zombie is standing 
 * around. So we can smash it again. But we need to 750 
 * In the moment when we have smashed zombie Zo in To we need to recharge a rifle 750,
 * smash zombie, recharge again 750, but by this time the Zo zombie will have been already 
 * moved back to grave. So we will never smash already smashed zombie.
 * The state is 
 * { Time last zombie was smashed, last zombie smashed, A number of already smashed zombies }
 * we are interested in those states which have the minimal time.
 * Model the state transition as graph minimize on time.
 */

#define ARRAY_LENGTH 1000;
#define NIL -1;
#define INF 858993460

#define ABS(item) ((item < 0) ? -item : item)
#define MAX(left, right) (left >= right) ? left : right
#define MAXABS(left, right) (ABS(left) >= ABS(right)) ? ABS(left) : ABS(right)

struct State
{
	int TimeSmash;
	int ZombieSmashed;
	int NumberAlreadySmashed;
};

struct ZombieInfo
{
	int Id;
	int X;
	int Y;
	int AppearanceTime;
};

/* Possible graph representation 
 * Lets take an example of 
 *  0 |   0 0 0
 *  1 |   1 0 0
 *  2 |   -1 0 0
 *  3 |   10 10 1000
 * States: (NIL, 0, 0)
 * Then (1, 1, t11) (1, 2, t12) (1, 3, t13)
 *      (2, 1, t21) (2, 2, t22) (2, 3, t23)
 *      (3, 1, t31) (3, 2, t32) (3, 3, t33)
 * Then we need to determine what minimal time those states can be reached
 * and if they are not reachable at all, the time will tij will be INF.
 * Firstly we have t00 = 0, tij = INF for all other states.
 * Then according to the Dijkstra algorythm we choose a tuple with the smallest time
 * and try to relax all vertexes. It is obvious that from (NIL, 0, 0) we will never
 * move to (2, 2, INF) because it is impossible to kill 2 zombies.
 * (NIL, 0, 0) -> (1, 1, t11) t11 = 1 * speed + 750 = 1 * 100 + 750 = 0 < 850 < 1000 good
 * (NIL, 0, 0) -> (2, 1, t21) t21 = 850
 * (NIL, 0, 0) -> (3, 1, t31) t31 = 1000 <= 10 * 100 = 1000 <= 2000 
 * So after the first iteration we have those states:
 * Then (NIL, 0, 0)   (1, 1, 850)  (1, 2, INF) (1, 3, INF)
 *                    (2, 1, 850)  (2, 2, INF) (2, 3, INF)
 *                    (3, 1, 1000) (3, 2, INF) (3, 3, INF)
 * Then choose the next minimal vertex: (1, 1, 850) and try to relax all vertexes 
 * (1, 1, 850) -> (1, 1, 850)   pointless
 * (1, 1, 850) -> (2, 1, 850)   pointless
 * (1, 1, 850) -> (3, 1, 2000)  pointless
 * have improved the time from 2000 to 1750
 * (1, 1, 850) -> (1, 2, INF): pointless
 * (1, 1, 850) -> (2, 2, INF): 850 + min { 2* 100, 750 } = 850 + 750 = 1600 > 1000 - we can not go here
 * (1, 1, 850) -> (3, 2, INF): 850 + 9 * 100 = 1750 > 1000 and < 2000 - 1750
 * The new states are
 * Then *(NIL, 0, 0) *(1, 1, 850)  (1, 2, INF)  (1, 3, INF)
 *                    (2, 1, 850)  (2, 2, INF)  (2, 3, INF)
 *                    (3, 1, 1000) (3, 2, 1750) (3, 3, INF)
 * Lets take (2, 1, 850) -> (3, 2, 1750): 850 + 11 * 900 > old 1750
 * Lets take (3, 1, 1000) -> (1, 2, INF) 1000 + 9 * 100 = 1900 > 1000 - INF
 * Lets take (3, 1, 1000) -> (2, 2, INF) 1000 + 11 * 100 = 2100 > 1000 - INF
 * Lets take (3, 2, 1750) -> (1, 3, INF) impossible
 * (3, 2, 1750) -> (1, 3, INF) = 1750 + 9*100 - impossible
 * (3, 2, 1750) -> (3, 3, INF) = 1750 + 11 * 100 = 2850 
 */

/* Swap functio need to keep track of the item in a heap
 * For example we have  4 ..... 5  after swapping 5 .... 4
 *                      i       j                  
 * map[4] = i  after swapping map[4] = j
 * map[5] = j                 map[5] = i
 */

struct QueueItem
{
	int id;
	int value;
};

void HeapSwap(struct QueueItem* Queue[], int Map[], int i, int j) 
{
    struct QueueItem* temp;
	/* Changng map file */
	Map[Queue[j]->id] = i;
	Map[Queue[i]->id] = j;
    /* Swapping */
	temp = Queue[i];
    Queue[i] = Queue[j];
    Queue[j] = temp; 
	
} 
/* Heap is very similar to binary tree.
 * item i parent items 2*i, 2*i + 1 are his left and right childs respectively
 * Tree property: leftChild <= parent <= rightChild. Q[i] <= Q[2*i+1] and Q[i] <= Q[2*i]
 * The parent has less or equal value than his childs. Provided that the minimal
 * value is in the root of the heap. When one of the childs is less than its 
 * parent we need to balance the heap. For example Q[2*i] < Q[i] Q[2*i+1] >= Q[i]
 * We need to swap 2*i and i and continue this process until the item takes a right place
 *
 */
void Insert(struct QueueItem* NewItem, struct QueueItem* Queue[], int Map[], int* size) 
{
    int i;

    (*size)++;
    Queue[*size] = NewItem;
	Map[NewItem->id] = *size;

	for (i = *size; i > 1 && Queue[i/2]->value > Queue[i]->value; i=i/2)
    {
        HeapSwap(Queue, Map, i/2, i);
    }       
}
/* Example 1 3 2 5 4 7 6 
 * size = 7. min = 1 swap 6 to 1
 * 6 3 2 5 4 7 6 
 * Then we go to the smallest chuld of 6. 2
 * and swap 6 with smallest: 
 * 2 3 6 5 4 7 6  2*2 
 */ 
int ExtractMin(struct QueueItem* Queue[], int Map[], int* size) 
{
    int i;
	int current;
    struct QueueItem* min;
    struct QueueItem* last;

    min = Queue[1];
	Map[min->id] = -1;

    last = Queue[*size];
	Map[last->id] = 1;

    Queue[1] = last;
	(*size)--;
	
    for (i = 1; (current = 2*i) <= *size; i = current) 
	{
		if (current + 1 <= *size && Queue[current + 1]->value < Queue[current]->value)
		{
            current++;
		}

		if (Queue[i]->value <= Queue[current]->value)
		{
            break;
		}
        HeapSwap(Queue, Map, current, i); 
    } 
    return min;
}

/* We have i-th parent and 2 children: 2*i - th and (2*i + 1) - th
 * And the main piramid rule is Q[i] <= Q[2*i] and Q[i] <= Q[2*i + 1]
 * Suppose that we decrease (2*i)th key. It may break the condition of:
 * Q[i] > Q[2*i]. We need to swap and continue it until the item 
 * takes an appropriate position.
 */
void DecreaseKey(struct QueueItem* Queue[], int Map[], int* size, int id, int newValue)
{
    int i;
	int position;

	position = Map[id];
	i = position;
	/* When a new value is more than current, we need to break  */
	if (newValue >= Queue[i]->value)
	{
		return;
	}

	Queue[i]->value = newValue;

	for ( ; i > 1 && Queue[i]->value < Queue[i/2]->value; i = i/2)
	{
        HeapSwap(Queue, Map, i, i/2);
	}
}

struct QueueItem* Convert(int id, struct State* state)
{
	struct QueueItem* newItem = (struct QueueItem*)malloc(sizeof(struct QueueItem));
	newItem->id = id;
	newItem->value = state->TimeSmash;
	return newItem;
}

void WorkWithQueueTest()
{
	/* All states 0 1 2 3 4 5 6 7 */
	struct State States[100] = 
	{ 
		{ 5, -1, 0 }, 
	    { 6, -1, 0 }, 
		{ -1, -1, 0 },
		{ 4, -1, 0 },
	    { 90, -1, 0 },
	    { -10, -1, 0 },
	    { 0, -1, 0 },
	    { 12, -1, 0 }
	};

	struct QueueItem* Queue[100];
	struct QueueItem* Min;
	int Map[100];
	int Size = 0;

    Insert(Convert(0, &States[0]), Queue, Map, &Size);
    Insert(Convert(1, &States[1]), Queue, Map, &Size);
    Insert(Convert(2, &States[2]), Queue, Map, &Size);
    Insert(Convert(3, &States[3]), Queue, Map, &Size);
    Insert(Convert(4, &States[4]), Queue, Map, &Size);
    Insert(Convert(5, &States[5]), Queue, Map, &Size);
    Insert(Convert(6, &States[6]), Queue, Map, &Size);
    Insert(Convert(7, &States[7]), Queue, Map, &Size);

    Min = ExtractMin(Queue, Map, &Size);
	Min = ExtractMin(Queue, Map, &Size);
	Min = ExtractMin(Queue, Map, &Size);

    DecreaseKey(Queue, Map, &Size, 4, -90);
    DecreaseKey(Queue, Map, &Size, 1, -6);
}

struct State* CreateState(
	int TimeSmash, 
	int ZombieSmashed, 
	int NumberAlreadySmashed)
{
    struct State* state = (struct State*)malloc(sizeof(struct State));
	state->NumberAlreadySmashed = NumberAlreadySmashed;
	state->TimeSmash = TimeSmash;
	state->ZombieSmashed = ZombieSmashed;
	return state;
}

struct ZombieInfo* CreateNullZombie()
{
    struct ZombieInfo* initialZombie = (struct ZombieInfo*)malloc(sizeof(struct ZombieInfo));
	initialZombie->X = 0;
	initialZombie->Y = 0;
    initialZombie->Id = 0;
    initialZombie->AppearanceTime = 0;
}

int CalculateDestinationTime(
    struct State* CurrentState, 
    struct ZombieInfo CurrentZombie, 
	struct ZombieInfo DestinationZombie)
{
	int timeToMove;
	int destPointOfTime; 
	int time = INF;
	int speed = SPEED;
	int XDiff = DestinationZombie.X - CurrentZombie.X;
	int YDiff = DestinationZombie.Y - CurrentZombie.Y;
	int maxCoordinate = MAXABS(YDiff, XDiff);
    /* Time we need to move from current zombie to destination zombie */ 
	timeToMove = maxCoordinate * SPEED;
    /* We need to take into account RECHARGE_TIME.
	 * if timeToMove is less than RECHARGE_TIME, we need to move to the destination
	 * grave for timeToMove time and then wait for (RECHARGE_TIME - timeToMove)
	 * until our rifle will be recharged. 
	 */
    timeToMove = MAX(RECHARGE_TIME, timeToMove);
	/* An absolute time when we can reach the destination zombie */
	destPointOfTime = CurrentState->TimeSmash + timeToMove;
    /* We need to determine if this time will overlapp with destination zombie 
	 * destPointOfTime:     ......... |------------------------------
	 * dest zombie pop up:  .............|----------|
	 *                           destTime        destTime + CORPSE_STAND_DURATION
	 * In this case we will never smash dest zombie.
	 */
    if (DestinationZombie.AppearanceTime + CORPSE_STAND_DURATION < destPointOfTime)
	{
		return INF;
	}
	/* It can be a situation when we need to wait until the destination zombie will peep out */
	if (destPointOfTime < DestinationZombie.AppearanceTime)
	{
        return DestinationZombie.AppearanceTime;
	}
	/* In this case when we come up to the destination zombie, it is standing over the grave.
	 * So we can smash it. */
	return destPointOfTime;
}

void GenerateStates(
      struct State* States[], int* StatesCount,
	  struct QueueItem* Queue[], int Map[], int* QueueCount,
	  struct ZombieInfo Zombies[], int ZombiesCount)
{
	int time;
	/* Zombies counter */
    int zombId;
	/* Possible killed zombies counter */
	int numSmashed;
    struct State* state;
    struct State* state0;
	struct ZombieInfo* zombie; 
	state0 = CreateState(0, 0, 0);
	zombie = CreateNullZombie();
	/* Add the first state at time 0 when we have not killed any of zombies */
    //States[*StatesCount] = state;
    //Insert(Convert(*StatesCount, state), Queue, Map, &QueueCount);
    //(*StatesCount)++;

	for (zombId = 0; zombId < ZombiesCount; zombId++)
	{
       for (numSmashed = 1; numSmashed <= ZombiesCount; numSmashed++)
	   {
		   time = INF;
		   /* Firstly from the state 0 we can go directly only to those states 
		    * which have number of already smashed zombies equal to 1
		    */
		   if (numSmashed == 1)
		   {
              time = CalculateDestinationTime(state0, *zombie, Zombies[zombId]);
		   }
		   state = CreateState(time, zombId, numSmashed);
		   States[*StatesCount] = state;
           Insert(Convert(*StatesCount, state), Queue, Map, QueueCount);
           (*StatesCount)++;
	   }
	}
}

void DijkstraAlgorythm(struct State* States[], int* StatesCount,
	  struct QueueItem* Queue[], int Map[], int* QueueCount,
	  struct ZombieInfo Zombies[], int ZombiesCount)
{
	int i;
	int time;
	int stateId;
	struct QueueItem* Q;
	struct State* QState;
	struct State* DestState;
    /* Generate initial set of states */
    GenerateStates(States, StatesCount, Queue, Map, QueueCount, Zombies, ZombiesCount);
	while (QueueCount > 0)
	{
		/* Take the minimal state */
		Q = ExtractMin(Queue, Map, QueueCount);
		QState = States[Q->id];
		/* Try to move over to other states from this state and minimize their time *
		 * If we have found an unreachable vertex, go out from this method. 
		 * because further processing is pointless.
		 */
		if (Q->value == INF)
		{
			break;
		}
		/* Lets suppose that current state has Time - Ti, last smashed zombie - Zi, 
		 * Number of smashed zombies - Ni. We can only move to states which have number of smashed zombies 
		 * Ni - 1.  (Z1, Ni - 1), (Z2, Ni - 1), (Z3, Ni - 1),....,(Zn, Ni - 1)
		 */
        for (i = 0; i < ZombiesCount; i++)
	    {
			/* Calculating of time for moving from zombie x to zombie x is pointless.
			 * And it can cause incorrent time. We skip it.
			 */
			if (QState->ZombieSmashed == i)
			{
                continue;
			}

			time = CalculateDestinationTime(QState, Zombies[QState->ZombieSmashed], Zombies[i]);
			stateId = ZombiesCount * i + QState->NumberAlreadySmashed;
			/* If we have found a less key, we need to rotate or decrease every reach time */
			DecreaseKey(Queue, Map, QueueCount, stateId, time);
			/* We do it only if new time is less than the old time */
			if (time < States[stateId]->TimeSmash)
			{
                States[stateId]->TimeSmash = time;
			}
	    }
	}
}

void MacroTest()
{
	int va11 = 0;
	int val2 = -20;
	int val = -10;
	int abs = ABS(val);
    int max = MAX(va11, val2);
    int maxAbs = MAXABS(va11, val2);
}

void DikstraAlgorythmTest()
{
    struct ZombieInfo Zombies[100] = 
	{
		{ 0, 10, 10, 1000 },
        { 1, -10, 10, 1000 },
        { 2, 10, -10, 1000 },
        { 3, -10, -10, 1000 },
        { 4, 20, 20, 2000 }
	};
	int ZombiesCount = 5;
	struct QueueItem* Queue[100];
	int Map[100];
	int QueueSize = 0;
	struct State* States[100]; 
    int StatesCount = 0;

	MacroTest();

	DijkstraAlgorythm(States, &StatesCount,
	  Queue, Map, &QueueSize,
	  Zombies, ZombiesCount);
}