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

}

