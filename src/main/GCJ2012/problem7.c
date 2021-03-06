/*  GCJ R3 2 2012  



Problem

Havannah is an abstract strategy board game created by Christian Freeling. 
Havannah is a game played on a hexagonal board with S hexagons to each side. 
Each hexagon has two horizontal and four slanted edges. 

The hexagons are identified by pairs of integer values. 
The hexagon in the bottom corner of the board is (1, 1). 
The hexagon adjacent to (x, y) in the direction of a two-o'clock hand is (x, y+1). 
The hexagon adjacent to (x, y) in the direction of a ten-o'clock hand is (x + 1, y). 

Here is an example board with S = 5:
 
In the game of Havannah, each hexagon can be occupied by at most one stone. 
Stones once put on the board are never removed or moved. 

The goal of the game is to build from stones 
a connected set of stones of one of three kinds. 

The winning structures are:

 •A ring that encircles one or more empty hexagons. 
  That is, at least one of the inner hexagons must be empty. 
  More specifically, there is an empty hexagon that is separated 
  from the outermost boundary of the board by hexagons with stones. 
  Note that this rule is different from the official game Havannah.

 •A bridge that connects any two corners of the board.

 •A fork that connects any three of the board's six edges. 
  Corners do not count as part of either adjacent edge.
 
  This picture shows examples of winning structures:
 
  Your program should determine whether a sequence 
  of moves of a single player builds a winning structure. 
  
  If so, it should output the name of the structure 
  and the number of the move that completed it. 

  If a move completes multiple rings, 
  connects more than two corners, 
  or connects more than three edges, 
  the structure is still considered a ring, a bridge, or a fork, respectively. 
  But if a move completes structures of different kinds at once, 
  your program should output the names of all of them. 
  We are only interested in the first winning move: 
  ignore all moves following the winning one. 
  If there is no winning structure on the board after playing 
  all the moves from the sequence, your program should output none.

  Input

  The first line of input gives the number of test cases, T. 
  T test cases follow. The first line of each test case contains two integers S and M, 
  the number of hexagons on each side of the board 
  and the number of moves in the sequence, respectively. 
  
  The next M lines provide the sequence of moves, 
  in order, where each line contains a space-separated pair (x, y) of hexagon identifiers. 
  All the moves in the sequence lie on the board of size S. 
  
  In each test case, the board is initially empty and the moves do not repeat.


  Output

  For each test case, output one line containing "Case #n: " followed by one of:

   •none
   •bridge in move k
   •fork in move k
   •ring in move k
   •bridge-fork in move k
   •bridge-ring in move k
   •fork-ring in move k
   •bridge-fork-ring in move k
 
  The cases are numbered starting from 1. The moves are numbered starting from 1.

  Limits

  Small dataset
 
   1 ≤ T ≤ 200
   2 ≤ S ≤ 50
   0 ≤ M ≤ 100

  Large dataset
 
   1 ≤ T ≤ 20
   2 ≤ S ≤ 3000
   0 ≤ M ≤ 10000
 
  Sample
  Input

7
 2 4
 1 1
 1 2
 2 3
 3 3
 3 6
 2 1
 2 2
 2 3
 2 4
 1 2
 4 4
 3 7
 3 3
 2 2
 2 3
 3 4
 4 4
 4 3
 3 2
 3 6
 2 2
 2 3
 3 4
 4 4
 4 3
 3 2
 3 8
 1 1
 2 1
 1 3
 2 4
 1 2
 3 2
 3 3
 3 4
 3 7
 1 1
 2 2
 3 5
 3 4
 5 3
 4 3
 3 3
 3 3
 1 1
 1 3
 3 5
 

 Case #1: bridge in move 2
 Case #2: fork in move 5
 Case #3: none
 Case #4: ring in move 6
 Case #5: bridge-fork in move 5
 Case #6: bridge in move 7
 Case #7: none
 

Havannah was created by Christian Freeling and MindSports. 
MindSports and Christian Freeling do not endorse and have no involvement with Google Code Jam.
 



Solution:



This problem probably looked a little scary to many competitors at first. 
(I, for one, remember some nasty problems involving hexagons in other contests). 
However, this one wasn't as bad as it looked.


Let's first see what kind of cells there are and how to distinguish between them:

 •Inner Cells: 
    Those were the cells that are not part of the border of the board. 
    They had 6 neighboring cells.

 •Corner Cells: 
    Those were the cells where two edges of the board overlap. 
    They had 3 neighboring cells.

 •Edge Cells: 
    Those were the cells that were neither inner cells, nor corner cells. 
	They had 4 neighboring cells.
    Note that it was a bit counter-intuitive that the corner cells 
	were *not* edge cells 
	(as they, in fact, lie on the edges), 
	however that was clearly defined in the statement: "Corners do not count as edges.".

  We can enumerate the edges and the corners of the board. 
  Each edge will have a distinct integer index between 0 and 5, 
  inclusive, and each corner will also have a distinct index between 0 and 5, inclusive. 
  This is okay, but for simplicity later we will give the indices from 0 to 5 
  to the corners and add 6 to the indices of the edges, 
  to obtain 6 to 11 for the edges. 
  If a cell neither is a corner, nor belongs to any of the edges, 
  we can mark it with -1. 
  Thus, we create a function int checkType(row, col) that checks 
  if the cell at (row, col) is a corner (and which one if it is), 
  part of an edge (and which edge, if it is) or neither.

    •Corners are the cells: 
	  {
	     (1, 1), 
		 (1, S), 
		 (S, S * 2 - 1), 
		 (S * 2 - 1, S * 2 - 1), 
		 (S * 2 - 1, S), 
		 (S, 1)
	  }

    •Edges are the cells: 
	  { 
	     (1, X), 
		 (X, 1), 
		 (X, S * 2 - 1), 
		 (S * 2 - 1, X)
	  } 
	  
	  where X is any integer, together with all cells for which |row - col| == S - 1.

      Don't forget that corners are not considered edges, 
	  so exclude them when checking for edges.
 
     
	 Now let's examine in more detail the winning figures: 

      •Bridge: 

	      This was probably the simplest of the three: 
		  just a set of stones that connects any two corner cells. 
		  The minimal number of stones to achieve it was 2 on a board with S = 2 
		  (which was the minimal possible board).

      •Fork: 

	      This was a bit more complicated. 
		  We need a set of stones, 
		  that connects 3 distinct edges 
		  (i.e. cells from three distinct edges). 
		  
          The minimal number of stones to achieve it was 5 
	      on a board with S = 3 
	      (note that the board with S = 2 does not have any edge cells).

      •Ring: 

	      This was the most complicated figure. 
		  We need a set of stones that encircles an empty cell. 
		  As it might have been a bit unclear what "encircles" in this case means, 
		  the problem statement contained a detailed explanation and an example. 

		  The minimal number of stones to achieve it was 6 on a board with S = 3. 
		  Note that a ring on a board with S = 2 is impossible, 
		  as it inevitably would lead to a bridge first.
 

      As many of the competitors might have realized, 
	  the Bridge and Fork cases are relatively simple to detect. 
	  
	  One way to solve them would be to use binary search over the move 
	  in which they are created and check if a Bridge and/or a Fork exists. 
	  
	  That was okay, however it didn't work for Rings. 

	  Some competitors used this solution for Forks and Bridges, 
	  and another one for Rings. 
	  
	  However, this required some extra code, 
	  which is rarely a good idea in a speed contest.
 

      Some competitors probably realized that even an O(M2) solution would pass. 
	  However, we will describe an O(M) solution, 
	  which was very fast and not much harder to implement.
 

      First, it's a good idea to store the board in 
	  a both memory and time efficient way. 
	  
	  The thing to notice was that when the board is really big, 
	  then it is also really sparse 
	  (only 10000 stones in a board with over 9000000 cells). 
	  
	  So the way to go was to use any kind of set data structure 
	  our programming language of choice provided. 
	  In C++, a hashset is fastest, but tree sets are also fine. 
	  
	  Putting a stone is a constant operation 
	  if we are using a hashset and logarithmic one 
	  if we're using balanced tree set (for example C++ STL set).

      Now we start putting stones on the board in the order they are given. 
	  We need a fast way to check if putting a stone created some 
	  of the winning figures. 
	  
	  The fork and the bridge needed some cells to be connected 
	  (i.e. to belong to the same connected component). 
	  In fact, the ring also requires the same thing. 
	  
	  As many competitors in this round probably know, 
	  these are usually handled using the union find algorithm. 
	  
	  As a reminder union-find supports the following basic operationss:

      • Create a new node, belonging to a group of size 1. 
      • Merge two new groups into one. 
      • For a given node, find the group it is. 

      Implemented correctly, all of these operations are blazingly fast. 

      We will use union-find here with a minor addition. 
	  After putting a stone, we see if some of the neighboring cells contains a stone already. 
	  
	  If none do, we add the new stone to its own connected component. 
	  If there are stones in adjacent squares, 
	  we merge them all to a single connected component that also contains the new stone. 
	  
	  Additionally to the straight-forward union-find algorithm, 
	  we will store what type of cells each component contains. 
	  Since the number of different types of cells we are interested in 
	  is relatively small (only 12 types) 
	  we can use a mask with 12 bits 
	  (corresponding to the indices from 0 to 11 we mentioned earlier). 
	  
	  We do this in order to have an easy way to see 
	  if a component contains corner or edge cells. 
	  
	  When merging the component A to component B, 
	  we change the mask of B to contain the bits of the mask of A as well 
	  (since now they are the same component). 
	  
	  This can be done really easily with the bitwise operation OR. 
	  If, after merging, a component ends up with 2 set bits in the first 6, 
	  or 3 set bits in the second 6 positions, 
	  then we've just created a bridge or a fork, respectively.
 
      Everything's fine with that, but we still haven't answered the question 
	  how do we handle rings. 
	  Well, having the stones and their respective components makes it easy to check for them. 
	  
	  In order for a ring to be created, 
	  we must have just added a stone that connects a component to itself. 
	  
	  But if it does, it still does not necessarily create a ring. 
	  What we can do is check all neighbors of the cell 
	  where we are putting the stone, and search for two cells with stones with the same component.


      We can represent the neighbors of the given cell as the following: 

      # 1 2
      6 * 3
      5 4 #

      Going clockwise twice (or counter-clockwise, if you prefer, it doesn't matter) 
	  and looking at the neighbors there should be one of the following sequences 
	  (as a subsequence of the created sequence of 12): 
	  
	  1.{C, X1, C, Y1, Y2, Y3}
      2.{C, X1, X2, C, Y1, Y2}
      3.{C, X1, X2, X3, C, Y1}

      where the cells C belong to the same component, 
	  and each of the cells X and Y are either empty or belong to some component 
	  (not necessarily the same as C, and not necessarily the same as other Xs and Ys).

      After this move a ring is formed if and only if: 

      •At least one of the Xs is an empty cell
      •At least one of the Ys is an empty cell

      (Note that if there is a {C, X1, C, Y1, Y2, Y3} sub-sequence, 
	   then there will be a {C, X1, X2, X3, C, Y1} one, 
	   however we've included them both for clarity).

      Why is this true? Well, 
	  if none of the Xs or none of the Ys is an empty cell, 
	  then the two Cs were already connected "from this side" 
	  and adding the stone doesn't change it into a ring (obviously). 
	  
	  If both some of the X cells and some of the Y cells contain an empty cell, 
	  then we've just encircled at least one of them! 
	  
	  Imagine it this way - a circle has two sides - inside and outside. 
	  We know that we created a circle (since we are connecting a component to itself), 
	  but we don't know which side is the inner one and which side is the outer one. 
	  
	  Thus, if both contain an empty cell, then we have an empty cell in the inner side for sure.
 
      What is the time complexity of the given algorithm? 
	  Since OR is a constant operation, we have a constant number of bits to check 
	  when looking for a Fork or a Bridge, 
	  and checking for a ring involves also a constant number of operations 
	  (equal to the number of neighbors), 
	  the complexity is dominated by the speed of the union-find operations. 
	  
	  Thus, the total complexity of the algorithm is O(M * RACK(M)), 
	  where RACK() is the inverse of the Ackerman function. 
	  However, RACK() is so slowly growing, that you 
	  can assume the algorithm will run in nearly O(M) for the given constraints.
 

      Remark: 
	  
	  Another nice trick for dealing with rings is to start 
	  with all pieces played on the board and work backwards, 
	  always considering which empty squares are connected to each other. 
	  
	  Removing a stone possibly connects two of these components, 
	  and so we can again use union-find to track the state quickly. 
	  
	  This is conceptually simpler, but it is slower because 
	  we need to first find all the connected components after the pieces are played.
 

*/

#define STONES_MAX 10000
#define HEX_SIZE_MAX 3000
#define HEX_NEIGHBOURS_NUM 6
#define INT_MAX 2147483647

/* We need to calculate the max hexagon coordinates.
 * For example if the size of hexagon is S.
 * then if we start cell enumeration from the 1 we get
 * 
 * 1. Corner 0 is:      (1, 1)
 *
 * 2. Edges in direction 45' are:  (1, 2), (1, 3),..., (1, S - 2), (1, S - 1)
 * 3. Edges in direction -45' are: (2, 1), (3, 1),..., (S - 2, 1), (S - 1, 1)
 *    We can get 3 by swapping of x and y in 2
 *
 * 4. Next corner 45' is:  (1, S) 
 * 5. Next corner -45' is: (S, 1)
 *    We can get 5 by swapping of x and y in 4
 * 
 * 6. Edges in direction 90' are:  (2, S + 1), (3, S + 2),..., (i, S + i),..., (S - 1, 2S - 2)
 * 7. Edges in direction -90' are: (S + 1, 2), (S + 2, 3),..., (S + i, i),..., (2S - 2, S - 1)
 *    We can get 7 by swapping of x and y in 6
 * 
 * 8. Next corner 90' is:  (S, 2S - 1)
 * 9. Next corner -90' is: (2S - 1, S)
 *    We can get 9 by swapping of x and y in 8
 * 
 * 10. Edges in direction 135' are:  (S + 1, 2S - 1), (S + 2, 2S - 1),..., (2S - 2, 2S - 1)
 * 11. Edges in direction -135' are: (2S - 1, S + 1), (2S - 1, S + 2),..., (2S - 1, 2S - 2)
 *     We can get 11 by swapping of x and y in 10
 * 
 * 12. The highest corner is (2S - 1, 2S - 1)
 *     The hexagon coordinate size is 2S - 1
 *     But we add 2 for wrapping this hexagon by additional cells
 *     We need this to facilitate a process of detecting rings.
 */

#define HEX_COORD_MAX 2 * HEX_SIZE_MAX + 1

/* If there is a hexagon cell (x, y) 
 * 1. The next neighbour in -45' direction is: (x + 1, y)
 * 2. The neighbour in 90' direction is: (x + 1, y + 1)
 * 3. The neighbour in 45' direction is: (x, y + 1)
 * 4. The next is: (x - 1, y)
 * 5. The next is: (x - 1, y - 1)
 * 6. The next is: (x, y - 1)
 */
int DeltaX[] = {1, 1, 0, -1, -1,  0};
int DeltaY[] = {0, 1, 1,  0, -1, -1};

int count2;

/* This struct is used in union sets algorythm */
struct Pr7SetItem
{
	int IdX;
	int IdY;
    int ParentIdX;
    int ParentIdY;
    int Info;
	int Rank;
};

void CreateSetItem(
   int StoneIdX, 
   int StoneIdY, 
   struct Pr7SetItem *SetItem, 
   int HexagonSize)
{
	/* First of all every stone is in its own set.
	 * And parent points to itself, self reference.
	 */
	SetItem->ParentIdX = SetItem->IdX;
    SetItem->ParentIdY = SetItem->IdY;
    /* First off all set Info to null. This means that this cell is an invard cell */ 
	SetItem->Info = 0;
	SetItem->Rank = 0;
    /* (1, 1) is the lowest corner. In this case into looks 000001 */
    if (StoneIdX == 1 && StoneIdY == 1)
		SetItem->Info |= 1;
	/* (S, 1) corner -45'. It looks like 000010 */
    if (StoneIdX == HexagonSize && StoneIdY == 1)
        SetItem->Info |= (1 << 1);
	/* (2S - 1, S) corner -90'. It looks like 000100  */
    if (StoneIdX == 2 * HexagonSize - 1 && StoneIdY == HexagonSize)
        SetItem->Info |= (1 << 2);
    /* (2S - 1, 2S - 1) the highest corner. It looks like 0001000 */
    if (StoneIdX == 2 * HexagonSize - 1 && StoneIdY == 2 * HexagonSize - 1)
        SetItem->Info |= (1 << 3);
	/* (S, 2S - 1) corner 90'. It looks like 00010000  */
    if (StoneIdX == HexagonSize && StoneIdY == 2 * HexagonSize - 1)
		SetItem->Info |= (1 << 4);
	/* (1, S) corner 45'. It looks like 00100000 */
    if (StoneIdX == 1 && StoneIdY == HexagonSize)
        SetItem->Info |= (1 << 5);
    /* ([1..S], 1) Edges in direction -45' */
    if (StoneIdY == 1 && StoneIdX > 1 && StoneIdX < HexagonSize)
        SetItem->Info |= (1 << 6);
    /* (y + S - 1, [1..S]) Edges in direction -90' */
    if (StoneIdX - StoneIdY == HexagonSize - 1 && StoneIdY > 1 && StoneIdY < HexagonSize)
        SetItem->Info |= (1 << 7);
    /* (2S - 1, [S..2S - 1]) Edges in direction -135' */
    if (StoneIdX == 2 * HexagonSize - 1 && StoneIdY > HexagonSize && StoneIdY < 2 * HexagonSize - 1)
        SetItem->Info |= (1 << 8);
    /* ([S..2S - 1], 2S - 1) Edges in direction 135' */
    if (StoneIdY == 2 * HexagonSize - 1 && StoneIdX > HexagonSize && StoneIdX < 2 * HexagonSize - 1)
        SetItem->Info |= (1 << 9);
    /* ([1..S], y + S - 1) Edges in direction 90' */
    if (StoneIdY - StoneIdX == HexagonSize - 1 && StoneIdX > 1 && StoneIdX < HexagonSize)
        SetItem->Info |= (1 << 10);
    /* (1, [1..S]) Edges in direction 45' */
    if (StoneIdX == 1 && StoneIdY > 1 && StoneIdY < HexagonSize)
        SetItem->Info |= (1 << 11);
}

void FindSetHeader(
    int X, int Y, 
	int* HeaderX, int* HeaderY,
	struct Pr7SetItem **StonesSet)
{
	int parentX = StonesSet[X][Y].ParentIdX;
	int parentY = StonesSet[X][Y].ParentIdY;

	/* If the set item by the key (x, y) references to itself
	 * it means that this is the head of the set and we return it.
	 */
	if (parentX == X && parentY == Y)
	{
        *HeaderX = X;
		*HeaderY = Y;
		return;
	}
    /* Recursively find the header coordinates */
    FindSetHeader(parentX, parentY, HeaderX, HeaderY, StonesSet);
    /* Flatten our set. Change parent link to point directly to 
	 * the head of the set.
	 */
    StonesSet[X][Y].ParentIdX = *HeaderX;
    StonesSet[X][Y].ParentIdY = *HeaderY;
}

/* In merge function we also merge information about the mask of items.
 * Lets illustrate it on an example:
 * 1. From the start the set is empty 
 * 2. Then we put a stone into an inward cell, the set increases but the mask
 *    is 0000 until we put a stone into edge or into corner.
 * 3. The next stone is edge 000 000 100 000 | 0 the new mask.
 * 4. If the next stone is the same edge it does not matter for us, because 
 *    we distinguish only figures from different edges or corners.
 * 5. If the next stone is a different edge we will get the mask: 000 000 100 001   
 *    If we see only 2 different ones in positions from 1 to 6 that means that 
 *    this set is bridge which connects two different corners.
 *    If we have 3 different edges in position from 6 to 12 that means that we have 
 *    got a fork.
 */
void MergeSet(
    int LeftX, int LeftY, 
	int RightX, int RightY, 
    struct Pr7SetItem **StonesSet,
	void (*func)(struct Pr7SetItem**, int, int, int, int))
{
	int LeftHeaderX;
	int LeftHeaderY;

    int RightHeaderX;
	int RightHeaderY;

    FindSetHeader(LeftX, LeftY, &LeftHeaderX, &LeftHeaderY, StonesSet);
    FindSetHeader(RightX, RightY, &RightHeaderX, &RightHeaderY, StonesSet);

	/* We union a smaller set to a larger set. */
	if (StonesSet[LeftX][LeftY].Rank > StonesSet[RightX][RightY].Rank)
	{
		StonesSet[RightX][RightY].ParentIdX = LeftX;
		StonesSet[RightX][RightY].ParentIdY = LeftY;
		/* Unite information about what types of edges or corner the stone set has */
		StonesSet[LeftX][LeftY].Info |= StonesSet[RightX][RightY].Info;
		return;
	}

    StonesSet[LeftX][LeftY].ParentIdX = RightX;
    StonesSet[LeftX][LeftY].ParentIdY = RightY;
	/* Unite information about what types of edges or corner the stone set has */

	if (func)
	{
		func(StonesSet, LeftX, LeftY, RightX, RightY);
	}

	//StonesSet[RightX][RightY].Info |= StonesSet[LeftX][LeftY].Info;

	/* When ranks of left and right side are equal then we union them 
	 * and increase rank for 1 point.
	 */
	if (StonesSet[RightX][RightY].Rank == StonesSet[LeftX][LeftY].Rank)
	{
		StonesSet[RightX][RightY].Rank++;
	}
}

void SetMask(
    struct Pr7SetItem** StonesSet, 
    int LeftX, int LeftY, 
	int RightX, int RightY)
{
    StonesSet[RightX][RightY].Info |= StonesSet[LeftX][LeftY].Info;
}

void IncrementCount2(
    struct Pr7SetItem** StonesSet, 
    int LeftX, int LeftY, 
	int RightX, int RightY)
{
    count2--;
}

int IsBridgeOrFork(int X, int Y, struct Pr7SetItem **StonesSet)
{  
	int HeaderX;
	int HeaderY;
	int Answer;
    
	/* Let's describe how to check if the set mask is a bridge.
	 * For example we have a 000 000 001 011
	 * The last 6 digits represent corners. If the set has 1 on the 
	 * i - th position in the mask, where 6 <= i <= 12
	 * that means the set has this corner.
	 * 63 = 111 111 = 1 << 6 - 1 = 
	 *
	 *  000 000 111 111  = 111 111 =  001 011
	 *  000 000 001 011  = 001 011
	 *
	 *  When we have only one corner in [6..12], it 
	 *  does not sutisfy us. The list of those indexes is:
	 *
	 *  000 000 = 0
	 *  000 001 = 1
	 *  000 010 = 2
	 *  000 100 = 4
	 *  001 000 = 8
	 *  010 000 = 16
	 *  100 000 = 32
	 */
	int CornerSet[1 << 6] =
    {
        0, 0, 0, 1, 0, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
    };
    /* For edges we need at least 3 ones in the range 
	 * of 6 difits: 000 011
	 * We do not accept all sequences with only one 1: 
	 *  0, 1, 2, 4, 8, 16, 32
	 * We do not accept all sequences with only two ones:
	 *  110 000, 101 000, 100 100, 100 010, 100 001,
	 *  011 000, 010 100, 010 010, 010 001,
	 *  001 100, 001 010, 001 001,
	 *  000 110, 000 101,
	 *  000 011 
	 */
	int EdgeSet[1 << 6] =
    {
        0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 1, 0, 1, 1, 1,
        0, 0, 0, 1, 0, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 0, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
    };

	FindSetHeader(X, Y, &HeaderX, &HeaderY, StonesSet);    

	/* This number checkes whether a set contains a bridge or fork */
	Answer = 0;
    /* First 6 bits are corners. 
	 * Bitwise 'and' is needed to clear bits from 6 to 12
	 */
    if (CornerSet[StonesSet[X][Y].Info & ((1 << 6) - 1)])  
        Answer |= 1;
    /* The last 6 bits are edges 
	 * It is also possible when Anwser contains two bits set
	 * In this case the set contains a fork and a brigde.
	 */
    if (EdgeSet[StonesSet[X][Y].Info >> 6])
        Answer |= (1 << 1);

    return Answer;
}

/* Example when HexagonSize is equal to 5. 
 * Then X >= 1 and X <= 5
 * When X == 1:  1 <= Y <= 1 + 5 - 1: (1, 1), (1, 2), (1, 3), (1, 4), (1, 5)
 * When X == 2:  1 <= Y <= 6: (2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6)
 * When X == 3:  1 <= Y <= 3 + 5 - 1 = 7: (3, 1),...,(3, 7)
 * When X == 4:  1 <= Y <= 4 + 5 - 1 = 8: (4, 1),...,(4, 7), (4, 8)
 * When X == 5:  1 <= Y <= 5 + 5 - 1 = 9: (5, 1),...,(5, 7), (5, 8), (5, 9)
 * and so on 
 * This function checks if the cell (x, y) is inside the hexagon.
 */
int InHexagon(int X, int Y, int HexagonSize)
{
	int low = X >= 1 
		 && X <= HexagonSize 
		 && Y >= 1 
		 && Y <= X + HexagonSize - 1;

	int upper = X > HexagonSize 
		 && X <= X <= 2 * HexagonSize - 1
		 && Y >= X - HexagonSize + 1 
		 && Y <= 2 * HexagonSize - 1;

	return low || upper;
}

int AtEdge (int X, int Y, int HexagonSize)
{
    return X == 1 
		|| Y == 1 
		|| X == 2 * HexagonSize - 1 
		|| Y == 2 * HexagonSize - 1 
		|| X - Y == HexagonSize - 1 
		|| Y - X == HexagonSize - 1;
}

void Pr7Solve()
{   
	/* 0 - set contains a bridge
	 * 1 - set contains a fork 
	 * SetState[0] contains the smallest stone id. The first 
	 * stone that causes win structure.
	 */
	int SetState[3];
    int StoneX[STONES_MAX];
	int StoneY[STONES_MAX];
	struct Pr7SetItem StonesSet[HEX_COORD_MAX][HEX_COORD_MAX]; 
	/* This array represents all hexagon's cells */
    int Hexagon[HEX_COORD_MAX][HEX_COORD_MAX];
	/* Number of testcases */
	int testCaseNumber;
	int hexagonSize;
	int stonesNumber;
    int i, j, t, k;
	int countRing;
	/* Reading of number of testcases */
    scanf("%d", &testCaseNumber);
    /* Process all testcases */
    for (i = 1; i <= testCaseNumber; i++)
    {
		/* Read of stones number and hexagon size */
        scanf("%d%d", &hexagonSize, &stonesNumber);
		/* Read all stones point destination inside the hexagon */
        for (j = 0; j < stonesNumber; j++)
        {
		    scanf ("%d%d", &StoneX[j], &StoneY[j]);
        }
		for (j = 0; j < 3; j++)
		{
            SetState[j] = INT_MAX;
		}
        /* Fill in all hexagon's cells by zero */
		memset(Hexagon, 0, sizeof(Hexagon));
		/* Read all stones sequentually and put them in the appropriate
		 * position inside the hexagon.
		 */
        for (j = 0; j < stonesNumber; j++)
        {
			int isBridgeOrFork;
			/* Hexagon[x][y] = 1 means that there is stone in the cell
			 * Hexagon[x][y] = 0 the cell is empty and we can put a stone here
			 */
			Hexagon[StoneX[j]][StoneY[j]] = 1;
            /* Create a set item for each stone */
			CreateSetItem(
				 StoneX[j], 
				 StoneY[j], 
				 &StonesSet[StoneX[j]][StoneY[j]], 
				 hexagonSize);
			/* Check all neighbour, adjacent cells whether there is  */
            for (t = 0; t < HEX_NEIGHBOURS_NUM; t++)
			{
				/* If neighbour has a stone we need to merge the current item
				 * with its neighbour.
				 */
                if (Hexagon[StoneX[j] + DeltaX[t]][StoneY[j] + DeltaY[t]])
				{
                     MergeSet(
						 StoneX[j], 
						 StoneY[j], 
	                     StoneX[j] + DeltaX[t], 
					     StoneY[j] + DeltaY[t], 
                         StonesSet,
						 SetMask
						 );
				}
			}
            
			Hexagon[StoneX[j]][StoneY[j]] = 1;

			isBridgeOrFork = IsBridgeOrFork(StoneX[j], StoneY[j], StonesSet);

            if ((isBridgeOrFork & 1) && j < SetState[0])
			{
                SetState[0] = j;
			}

            if ((isBridgeOrFork & 2) && j < SetState[1])
			{
                SetState[1] = j;
			}
		}

		count2 = 1;
        
		StonesSet[0][0].ParentIdX = 0;
		StonesSet[0][0].ParentIdY = 0;

		/* Go through all cells inside hexagon and find ones 
		 * suspicious for being inside a circle.
		 * We take only those cells, which are inside the hexagon
		 * and there are not a stone in it.
		 */
        for (j = 0; j < 2 * hexagonSize - 1; j++)
		{
            for (t = 0; t < 2 * hexagonSize - 1; t++)
			{
                if (InHexagon(j, t, hexagonSize) && !Hexagon[j][t])
				{
					Hexagon[j][t] = 2;

                    StonesSet[j][t].ParentIdX = j;
		            StonesSet[j][t].ParentIdY = t;

					count2++;

					for (k = 0; k < 6; k++)
					{
                        if (Hexagon[j + DeltaX[k]][t + DeltaY[k]] == 2)
				        {
                            MergeSet(
						       j, 
						       t, 
	                           j + DeltaX[k], 
					           t + DeltaY[k], 
                               StonesSet,
						       IncrementCount2
						    );      
						}
					}

                    if (AtEdge(j, t, hexagonSize))
					{
                        MergeSet(
						       j, 
						       t, 
	                           0, 
					           0, 
                               StonesSet,
						       IncrementCount2
						    );      
					}
				}
			}
		}
        /* Here we need some explanation about count2. 
		 * In case when there is a ring after the set union 
		 * we always have count2 set to 2. Lets count how many merge operations 
		 * we need to unite the adjacents set of n cells in an arbitrary order
		 * For two adjacenr cells we need count2 = 1. For three we need 2
		 * For n we need n - 1 merge operations. 
		 * So we have n increments of the count2 variable and n - 1 decrements of 
		 * this count2. So after this we have count2 = 1 + 1.
		 * When we process a set which has at least on cell on the edge of the hexagon.
		 * we merge only one with the point (0, 0) and decrement the count2 variable as well
		 * This means that after this we have count2 = 0.
		 * If count2 > 1 that means there at least one ring inside the hexagon
		 */
		if (count2 > 1)
		{
			/* We have at least one winning ring.
			 * We set the last stone which makes the winning ring to the last stone
			 */
            SetState[2] = stonesNumber - 1;
		}
        
		/* We need to find the first stone which causes the winning ring
		 * We loop from the latest stone to the first and remove it from the 
		 * hexagon. When we remove a stone there are 4 possible variants:
		 *
		 * 1. Extend existing space. 
		 *    If we discover a ring count2 +1 
		 *    If we discover a ring which on the edge +0
		 *    If we extend an existed ring count2 +0
		 *    If we extend an existed ring on an edge count2 +0
		 *
		 * 2. Unite two rings. 
		 *    Remove stone +1, unite cell with the first set -1,
		 *    unite the cell with the second set -1. When we unite two
		 *    rings into one ring we decrement count2.
		 * 
		 * 3. Unite a ring with edge ring. Also decrement count2
		 * 
		 */
        for (j = stonesNumber - 1; j > 0; j--)
        {
            Hexagon[StoneX[j]][StoneY[j]] = 2;

            StonesSet[StoneX[j]][StoneY[j]].ParentIdX = StoneX[j];
		    StonesSet[StoneX[j]][StoneY[j]].ParentIdY = StoneY[j];

			count2++;
            
			for (k = 0; k < 6; k++)
			{
                if (Hexagon[StoneX[j] + DeltaX[k]][StoneY[j] + DeltaY[k]] == 2)
		        {
                    MergeSet(
				       StoneX[j], 
				       StoneY[j], 
                       StoneX[j] + DeltaX[k], 
			           StoneY[j] + DeltaY[k], 
                       StonesSet,
				       IncrementCount2
				    );      
				}
			}

            if (AtEdge(StoneX[j], StoneY[j], hexagonSize))
			{
                MergeSet(
				       StoneX[j], 
				       StoneY[j], 
                       0, 
			           0, 
                       StonesSet,
				       IncrementCount2
				    );      
			}

			if (count2 > 1)
			{
				SetState[2] = j - 1;
			}
		}
	}
}


