/*

 You can put a stick of √2 length in diagonal to make a maze.
 The maze contains enclosed areas and open areas.
 Given the maze you are required to count the number of enclosed areas
 and find the area of the largest one.
 For the maze attached (mazedata.txt) we have 2 closed areas and the
 largest one has an area of 8.

 mazedata.txt contents (0s were converted to _ for legibility)
 1_______1___1___1_______1
 _1_____1___1_1___1_____1_
 __1___1___1___1___1___1__
 ___1_1___1_____1___1_1___
 1___1___1___1___1___1____
 _1_____1___1___1_1___1___
 __1___1___1___1___1___1__
 ___1_1___1___1_____1___1_
 ____1___1___1_______1___1
 ___1___1_1___1_____1_1___
 __1___1___1___1___1___1__
 _1___1_____1___1_1_____1_
 1___1___1___1___1___1___1
 _1_____1_1_____1___1___1_
 __1___1___1___1___1___1__
 ___1_1_____1_1___1___1___
 ____1_______1___1___1___

 */

#include <malloc.h>

#define NORTH 1
#define EAST 1 << 1
#define SOUTH 1 << 2
#define WEST 1 << 3


struct Node
{
   int X;
   int Y;
   int Direction;
   int UseCount;
};

struct Node* CreateNode(int X, int Y, int Direction)
{
   struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
   newNode->X = X;
   newNode->Y = Y;
   newNode->Direction = Direction;
   newNode->UseCount = 0;
   return newNode;
}

int GetNumberOfDirections(struct Node* node)
{
    int count = 0;
	if (node->Direction & NORTH) count++;
    if (node->Direction & EAST) count++;
	if (node->Direction & SOUTH) count++;
	if (node->Direction & WEST) count++;
	return count;
}

int RawAt(int row, int col, char* Raw[])
{
    if (col < 0 || col >= 25 || row < 0 || row >= 17)
		return 0;
	return Raw[row][col] == '1';
}

/*  Direction in clockwise directions are 
 *  { NORTH, EAST, SOUTH, WEST } 
 *  Return the next 4 directions in clockwise order
 *  after the current direction. Clockwise order is the best
 *  when we go through a maze. If our current direction is, 
 *  for example, NORTH. then the most prioritized direction is 
 *  EAST. Then SOUTH and WEST. And then NORTH
 */
int* RotateDirections(int Current)
{
	int Ret[4];

    if (Current == NORTH)
	{
		/* { EAST, SOUTH, WEST, NORTH } */
		Ret[0] = EAST; Ret[1] = SOUTH; Ret[2] = WEST; Ret[4] = NORTH; 
	}

	if (Current == EAST)
	{
        /* { SOUTH, WEST, NORTH, EAST } */
        Ret[0] = SOUTH; Ret[1] = WEST; Ret[2] = NORTH; Ret[4] = EAST; 
	}

	if (Current == SOUTH)
	{
		/* { WEST, NORTH, EAST, SOUTH } */
        Ret[0] = SOUTH; Ret[1] = WEST; Ret[2] = NORTH; Ret[4] = EAST; 
	}

	if (Current == WEST)
	{
        /* { WEST, NORTH, EAST, SOUTH } */ 
        Ret[0] = NORTH; Ret[1] = EAST; Ret[2] = SOUTH; Ret[4] = WEST; 
	}

    return Ret;
}

struct Node* GetNextNeighbourNode(
    struct Node* currentNode, 
	int direction,
	struct Node** Map)
{
	struct Node* nullNode;

	int row = currentNode->X;
	int col = currentNode->Y;

	if (direction == NORTH) return &Map[row - 1][col - 1];
    if (direction == EAST) return &Map[row + 1][col - 1];
	if (direction == SOUTH) return &Map[row + 1][col + 1];
	if (direction == WEST) return &Map[row - 1][col + 1];
    
	return nullNode;
}

/* When we have a node [ESW] and we are coming up 
	* to this node from a direction: EAST.
	* And we need to choose the next direction from the 
	* set: EAST, SOUTH, WEST in the clockwise direction.
	* but we must miss the opposite direction.
	* Opposite direction means to come back, which is 
	* unaccetable because we always need to move forward
	* In this case an opposite direction to EAST is WEST
	* so we do not move here.
	*/
int GetOppositeDirection(int direction)
{
    if (direction == NORTH) return SOUTH;
    if (direction == EAST) return WEST;
	if (direction == SOUTH) return NORTH;
	if (direction == WEST) return EAST;
    
	return -1;
}

int GetIndex(int Direction, int* Rotation)
{
    if (Direction == Rotation[0]) return 0;
    if (Direction == Rotation[1]) return 1;
    if (Direction == Rotation[2]) return 2;
    if (Direction == Rotation[3]) return 3;
	return -1;
}

/* We need to explain how std library Rotate function works:
 * The initial array of points is: 1 2 3 4 5 6 7 8 9  
 * The begin is 1, the 3 rd element in array is 4.  
 * After rotation we will get: 4 5 6 7 8 9 1 2 3 
 * Lets describe all possible directions
 * ds = [N E S W]
 * Describe all possible cases. 
 * 1. If prev = N 
 *    then indexOfPrev = point to N, indexOfPrev + 1 points to E.
 *    After rotate we receive [E S W N]
 *    This function chooses the best direction from [Cur, Test]
 *    according to Prev direction.
 */
int Closest(int Prev, int Cur, int Test) 
{	
    int currentIndex;
	int testIndex;
     
    int* theBestNexts = RotateDirections(Prev);

	if (Cur == -1)
		return Test;

    currentIndex = GetIndex(Cur, theBestNexts);
    testIndex = GetIndex(Test, theBestNexts);

	if (currentIndex == -1 || testIndex == -1)
		return -1;

    if (testIndex < currentIndex)
		return testIndex;
    
    return currentIndex;
}

 /* This function gets the next node for the current node
	 * in clockwise walk around.
	 * From the start we choose a random the best direction NORTH 
	 * by which we come up to the first node.
	 * In this case we must check all possible outgoing direction
	 * All thease direction comes up because we can not move back.
	 * We solved this issue by using isInitial boolean variable,
	 * which indicates the moment when we choose the first node.
	 * Firstly we set theBestNext direction to -1.
	 * and loop through all possible outgoing directions and choose 
	 * the closest one. 
	 * If the best direction is -1 that means that something bad 
	 * has happened in the algorythm and we need to investigate.
	 * The first one we process all possible directions
	 * Then we skip the opposite
	 */
struct Node* GetNextNode(
    struct Node** Map, 
	struct Node* curNode,
	int prevDirection,
	int isInitial)
{
	int i;
	int direction;
	int theBestDirection;

    theBestDirection = -1;

	/* 0 is NORTH 
	 * 1 is EAST
	 * 2 is SOUTH
	 * 3 is WEST
	 */
    for (i = 0; i < 3; i++)
	{
       direction = (1 << i);
       
	   if (!(curNode->Direction & direction))
		   continue;

       if (isInitial || theBestDirection != GetOppositeDirection(direction))
	   {
           theBestDirection = Closest(prevDirection, theBestDirection, direction);
	   }
	}
    /* Something wrong has happened */
	if (theBestDirection == -1)
		return (0x000000);	
}  

double TraceShape(
   struct Node* node,
   struct Node** Map)
{
    struct Node* start = node;
	int i, j;
	int lastDirection = NORTH;
	int hasLastDir = 0;	
	int x, y;
	double area = 0;	
	int isInitial = 1;

	struct Node* current;
	struct Node* previous = start;

	/* Loop through all possible nodes in 
	 * clockwise direction. If we come back 
	 * to the start we break from the cycle
	 */
	for(;;) 
	{
		current = GetNextNode(
			        Map, 
			        node,
	                lastDirection,
	                isInitial);
        /* We have already processed the first 
		 * node and we set the hasLastDir flag to false.
		 */
		hasLastDir = 0;
		/* If we have reached the start node we break
		 * the cycle
		 */
		if (current == start)
			break;
		/* Increase the usage count of this node */
		current->UseCount++;

		area += (current->Y + previous->Y) * (current->X - previous->X) / 2;
	}

	/* Compute the square of this closed area */
    return area;
}


void Solve()
{
	int rowCount = 17;
	int colCount = 25;

	int mazeRowCount;
	int mazeColCount;

	int row;
	int col;

	char* Raw[17];
	struct Node* newNode;
	struct Node* currentNode;
	struct Node Map[100][100];
	int again;
	int size;

	Raw[0] =  "1_______1___1___1_______1";
    Raw[1] =  "_1_____1___1_1___1_____1_";
    Raw[2] =  "__1___1___1___1___1___1__";
    Raw[3] =  "___1_1___1_____1___1_1___";
    Raw[4] =  "1___1___1___1___1___1____";
    Raw[5] =  "_1_____1___1___1_1___1___";
    Raw[6] =  "__1___1___1___1___1___1__";
    Raw[7] =  "___1_1___1___1_____1___1_";
    Raw[8] =  "____1___1___1_______1___1";
	Raw[9] =  "___1___1_1___1_____1_1___";
    Raw[10] = "__1___1___1___1___1___1__";
    Raw[11] = "_1___1_____1___1_1_____1_";
    Raw[12] = "1___1___1___1___1___1___1";
    Raw[13] = "_1_____1_1_____1___1___1_";
    Raw[14] = "__1___1___1___1___1___1__";
    Raw[15] = "___1_1_____1_1___1___1___";
    Raw[16] = "____1_______1___1___1____";

	mazeRowCount = 0;
	mazeColCount = 0;

	/*  We need to transform this file into our Node
	 *  array. We remove excessive 1 and keep only
	 *  Node items with directions.
	 *  For example our file will looks like:
	 *
	 *  [S]   [*]   [E]  [ES]  [S]  [*]   [E] 
	 *  [S]  [NW]  [WE]  [E]  [NES] [SNW] [*]
	 *  [*]  [NWE] [WES] [WS] [*]   [NES] [N]
	 *  [SW] [W]   [ES]  [N]  [NWE] [E]   [EN]
	 *  [*]  [NW]  [*]   [NW] [W]   [W]   [*]
	 *
	 */

	for (row = 0; row < rowCount; row += 4) 
	{
       mazeColCount = 0;

       for (col = 0; col < colCount; col += 4) 
	   {
           newNode = CreateNode(mazeRowCount, mazeColCount, 0);

		   if (RawAt(row, col, Raw))
		   {
              if (RawAt(row - 1, col - 1, Raw))
				  newNode->Direction |= NORTH;
			  if (RawAt(row - 1, col + 1, Raw))
				  newNode->Direction |= EAST;
              if (RawAt(row + 1, col + 1, Raw))
				  newNode->Direction |= SOUTH;
              if (RawAt(row + 1, col - 1, Raw))
				  newNode->Direction |= WEST;
		   }

		   Map[mazeRowCount][mazeColCount] = *newNode;
		   mazeColCount++;
	   }

	   mazeRowCount++;
	}
    
	/* We need to remove all dead Ends. We consider a vertex to
	 * be a dead end if the number of possible directions is only one.
	 * For example in the first cell in the Maze: Maze[0][0] = (S)
	 * In this case (0,0) maze points to Maze[1][1] which has North
	 * direction. If we remove Maze[0][0] we need to remove North
	 * direction component from the Maze[1][1]. 
	 * Lets illustrate this process for the first 2 rows.
	 *  
	 *  Process all nodes with only one direction 
	 *  from the first line 
     *
	 *  [S]   [*]   [E]  [ES]  [S]  [*]   [E] 
	 *  [S]  [NW]  [WE]  [E]  [NES] [SNW] [*]
	 *
	 *  [*]   [*]   [E]  [ES]  [S]  [*]   [E] 
	 *  [S]   [W]  [WE]  [E]  [NES] [SNW] [*]
	 *  
	 *  [*]   [*]   [*]  [ES]  [S]  [*]   [E] 
	 *  [S]   [*]  [WE]  [E]  [NES] [SNW] [*]
	 *
	 *  [*]   [*]   [*]  [ES]  [*]  [*]   [E] 
	 *  [S]   [*]  [WE]  [E]  [NES] [SW]  [*]
	 *
	 *  [*]   [*]   [*]  [ES]  [*]  [*]   [*] 
	 *  [S]   [*]  [WE]  [E]  [NES] [S]   [*]  
	 * 
	 *  Process all nodes with only one direction
	 *  from the second line.
	 *  
	 *  The second line before the first process step
	 *  [S]  [NW]  [WE]  [E]  [NES] [SNW] [*]
	 *  Here we have only 2 singular items: [S], [E]
     *  But after the first step we have a new singular
	 *  item [S]. We skip it and will process it after 
	 *
	 *  [S]   [*]  [WE]  [E]  [NES] [S]   [*]  
	 *  [*]  [NWE] [WES] [WS] [*]   [NES] [N]
	 *  
	 *  [*]   [*]  [WE]  [E]  [NES] [S]   [*]  
	 *  [*]   [WE] [WES] [WS] [*]   [NES] [N]
	 *
	 *  [*]   [*]  [WE]  [*]  [NES] [S]   [*]  
	 *  [*]   [WE] [WS]  [WS] [*]   [NES] [N] 
	 *
	 *  The third process step
	 *  
	 *  [*]   [*]   [*]  [ES]  [*]  [*]   [*] 
	 *  [*]   [*]  [WE]  [*]  [NES] [*]   [*]
	 *  [*]   [WE] [*]   [WS] [*]   [NE]  [*]
	 *  [SW]  [*]  [ES]  [*]  [NWE] [*]   [*]
	 *  [*]   [NW] [*]   [NW] [*]   [*]   [*]
	 *  
	 *  
	 *
	 */

	again = 1;

	while (again) 
	{
		again = 0;

		for (row = 0; row < mazeRowCount; row++) 
		{
			for (col = 0; col < mazeColCount; col++)
			{
				currentNode = &Map[row][col];

				if (GetNumberOfDirections(currentNode) == 1) 
				{
					again = 1;

					if (currentNode->Direction & NORTH)
						Map[row - 1][col - 1].Direction &= ~SOUTH;
					else if(currentNode->Direction & EAST)
                        Map[row + 1][col - 1].Direction &= ~WEST;
					else if(currentNode->Direction & SOUTH)
                        Map[row + 1][col + 1].Direction &= ~NORTH;
					else if(currentNode->Direction & WEST)
                        Map[row - 1][col + 1].Direction &= ~EAST;

					currentNode->Direction = 0;
				}
			}
		}
	}

	/* Process all left nodes to find shapes */
	for (row = 0; row < mazeRowCount; row++) 
	{
		for (col = 0; col < mazeColCount; col++)
		{
			currentNode = &Map[row][col];

			/* Here can be only 3 possible cases for UseCount
			 * 1. a node has only 2 directions. If we have already visited 
			 *    this node, we have used one of them to get into and one 
			 *    to get out of the node.
			 *    In this case we have UseCount = 1
			 *    GetNumberOfDirections(currentNode) = 2 - 1 = 1
			 *    1 < 1 is false
			 * 2. a node has only 3 direction.
			 *    First time we use two of them and the last one is free 
			 *    Secondly we use this direction.
			 */
			if (currentNode->UseCount < GetNumberOfDirections(currentNode) - 1)
			{
                currentNode->UseCount++;
                /* Then we go through this closed area
				 * We increase UseCount for each node we have processed
				 */
                size = TraceShape(currentNode, Map);
			}
		}
	}
}
