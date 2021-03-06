/*
  Problem

  You are on the face of Mount Everest. 
  You need to find shelter before you freeze, and it's dark! 
  What do you do? 

  The good news is you have already memorized the layout of the mountain. 
  It is a grid with certain squares impassable 
  and other squares containing caves 
  where you can rest for the night. 
  
  The bad news is you don't know where you are, 
  and it's too steep to climb up. 
  All you can do is move left, right, or down. 

  Here is an example layout, with '.' representing a passable square, 
  '#' representing an impassable square, 
  and numbers representing caves. 

  ######
  ##...#
  #..#.#
  #...##
  #0#..#
  ####1#
  ######

  Since it is so dark, you will move around by following a plan, 
  which is a series of instructions, 
  each telling you to move one square left, right, or down. 
  
  If an instruction would take you to a passable square or to a cave, 
  you will follow it. 
  
  If it would take you to an impassable square, 
  you will have to ignore it. 
  
  Either way, you will continue on to the next step, 
  and so on, until you have gone through the whole plan. 

  To help with your descent, you want to find out two things for each cave C: 

  What squares is it possible to reach C from? 
  We will label the set of these squares by SC, 
  and the number of them by nC. 

  Is there a single plan that, 
  if followed from any square in SC, will finish with you at cave C? 
  If so, we say the cave is lucky. 

  Note that you might pass by several caves while following a plan. 
  All that matters is what square you finish on after executing all the steps, 
  not what caves you visit along the way.

  For example, in the layout above, cave 0 is lucky. 
  There are 9 squares that it can be reached from 
  (including itself), 
  and the plan "left-left-down-down-left-down" 
  will finish with you at the cave from any of those squares.

  Input

  The first line of the input gives the number of test cases, T. 
  T test cases follow, beginning with a line containing integers R and C,
  representing the number of rows and columns in the mountain layout.

  This is followed by R lines,
  each containing C characters, 
  describing a mountain layout. 
  
  As in the example above, a '#' character represents an impassable square, 
  a '.' character represents a passable square, 
  and the digits '0'-'9' represent caves (which are also passable squares).

  Output

  For each test case, first output one line containing "Case #x:", 
  where x is the case number (starting from 1). 
  For each cave C, starting with 0 and counting up from there, 
  write a line "C: nC LC". 
  Here, C is the cave number, nC is the number of squares 
  you can reach the cave from, and LC is either the string 
  "Lucky" or the string "Unlucky", as defined above

  Limits

  There will be between 1 and 10 caves inclusive.
  If there are d caves, they will be labeled with the digits {0, 1, ..., d - 1}, 
  and no two caves will have the same label.
  All squares on the boundary of the mountain layout will be impassable.

  1 ≤ T ≤ 20.
  Small dataset

  3 ≤ R, C ≤ 10. 
  Large dataset

  3 ≤ R, C ≤ 60. 
  Sample

  Input 
  	
  Output 
  
  2
  7 5

  #####
  ##0##
  ##1.#
  ##2##
  #3..#
  #.#.#
  #####

  7 6

  ######
  ##...#
  #..#.#
  #...##
  #0#..#
  ####1#
  ######	 
  
  Case #1:

   0: 1 Lucky
   1: 3 Lucky
   2: 4 Unlucky
   3: 7 Lucky

  Case #2:

   0: 9 Lucky
   1: 11 Unlucky

   In the first case, here are some valid plans 
   you could use for the lucky caves: 

     For cave 0, you can use the empty plan. 
	 If you can reach the cave at all, you are already in the right place! 

     For cave 1, you can use the plan right-down-left. 
     For cave 3, you can use the plan right-right-left-down-down-down-left.




  The main challenge in this problem is determining whether a cave is lucky. 
  The set of all possible states is huge, 
  so a complete search over all plans is simply not going to work. 
  Fortunately, there is one observation we can use to greatly simplify the task.

  Eliminating Backtracking 

  Fix a cave C, and recall 
  that SC denotes the set of squares from which C can be reached. 
  We will build up our plan one instruction at a time. 

  Let X be the set of squares that you can be in 
  if you start in SC and follow the plan so far. 
  If X contains a square not in SC, 
  we know the plan cannot work for every starting position. 
  
  Otherwise, we can be 100% sure that the plan so far is fine! 
  
  This is because the set of possible squares we are in 
  has either stayed the same or gotten strictly smaller. 
  
  In particular, if there was a plan that worked from the starting position, 
  we can append it to what we have done so far, and it will still work. 

  So what does this mean? 
  
  As long as we don't do a move that adds a square 
  not in SC to the set of possible squares X, 
  we can go ahead and do that move, 
  and it will still be possible to finish. 
  
  In particular, we can always move left and right whenever we want, 
  since moving left or right can never move you out of SC.


  All You Need Is Down! 

  Even with the previous observation, 
  we still have work to do. 
  
  We now know all the moves that can be done safely, 
  but the state space is still huge. 
  
  We can't find just any move; we need one that makes progress. 

  Here is where it is important that you cannot go up the mountain. 
  Suppose you can add a Down move to the plan, 
  satisfying the following two properties: 

    There is at least one position in X from which you can actually move down. 
	(Without this, the Down move will never do anything, and so is useless!) 

    There is no position in X from which a down move will take you outside of XC. 
    If you add this Down move to the plan, 
	then the sum of the heights over all squares in X will have gone down, 
	and it can never go up again, because you can never climb the mountain! 


  Since the sum of the heights is a positive integer, 
  you will eventually have to stop making Down moves. 
  
  At that point, you are stuck in one or more horizontal intervals. 
  If there is just one interval and it contains the cave, 
  the plan can be finished successfully. 
  Otherwise, you're screwed! 
  
  And remember, since this set of squares is a subset of what you started with, 
  you were in fact screwed from the start.
  

  Can You Go Down? 

  Only one question remains: 
  given a set of positions X reachable from the start, 
  can you come up with a valid plan that includes at least one Down move? 

  X must be contained in a set of horizontal intervals,
  bounded by impassable squares to the left and right. 
  
  Since it is always safe to move left and right, 
  we can keep moving left until X is actually just 
  the leftmost square in each of these intervals. 
  
  If we cannot make progress from that situation, 
  we have already shown we are lost. 

  As we perform left and right moves from there, 
  our position within each interval might change. 
  
  However, note that if two intervals have the same length, 
  our relative horizontal positions within them will always be the same. 
  
  Therefore, let's define xj to be our relative horizontal position within 
  all intervals of length j. 
  
  (In particular, xj = 0 if we are in the leftmost position, 
   and xj = j - 1 if we are in the rightmost position.) 

  Lemma: 
     It is possible to reach position 
	 (x1, x2, ...) using left and right moves 
	 if and only if xi ≤ xj ≤ xi + j - i for all i < j. 

  Proof: 
     First we show xi ≤ xj. 
	 This is true initially. 
	 If it ever failed after some sequence of moves, 
	 it would be because xi and xj were equal, and then either: 
     We moved left, and only xj was able to move. 
     We moved right, and only xi was able to move. 
     However, both of these scenarios are impossible. 
	 Therefore, 
	 xi is indeed at most xj, 
	 or in other words, 
	 the distance from xi to the left wall is no larger 
	 than the distance from xj to the left wall. 
	 The same argument can be applied for the right wall, 
	 which gives us the other half of the inequality: xj ≤ xi + j - i. 


  Conversely, any set of positions with 
  xi ≤ xj ≤ xi + j - i 
  really can be reached via the following algorithm: 

  Start with each xi = 0. 
  Loop from i = largest interval length down to 2. 
  Move i - 2 + xi - xi-1 times to the right, 
  and then i - 2 times to the left. 

  Try it yourself and you will see why it works!

  We are now essentially done. 
  We need to determine if there is set of positions {xi} 
  that can be reached for which it is safe to move down. 
  Once you have gotten this far, you can finish it off with dynamic programming. 
  Here is some pseudo-code that determines whether there is a set of positions 
  from which it is possible to move down and make progress: 

  old_safety = [SAFE] * (n+1)

  for length in {n, n-1, ..., 1}:

     for i in [0, length-1]:
        pos_safety[i] = best(old_safety[i], old_safety[i+1])
        if moving down leaves SC:
           pos_safety[i] = UNSAFE
        elif moving down is legal and pos_safety[i] == SAFE:
           pos_safety[i] = SAFE_WITH_PROGRESS
        old_safety = pos_safety

  return (pos_safety[0] == SAFE_WITH_PROGRESS)

  This is a good starting point, 
  but you would still have to tweak it to actually record 
  what the right xi is for all i. 


  Putting it all together, we repeatedly use the above algorithm to see 
  if it is possible to make a down move. 
  If so, we do it and repeat. Otherwise, we stop. 
  If the only remaining interval is the one containing the cave, 
  then that cave is lucky. 
  Otherwise, it is not! 

  Since there are no correct submissions for the large input during the contest, 
  we provide here a full java solution (by Petr Mitrichev) 
  so everyone can use it to generate correct outputs for various inputs.




 */


struct Segment 
{
   int len;
   long goodExitMask;
   long badExitMask;
};

void Pr14Solve()
{
   int rows = 7;
   int cols = 6;
   char* field[7];
   int r, c, i;
   char caveId;
   struct Segment segments[100];
   int segmentCount = 0;

   field[0] = "######";
   field[1] = "##...#";
   field[2] = "#..#.#";
   field[3] = "#...##";
   field[4] = "#0#..#";
   field[5] = "####1#";
   field[6] = "######";
  
   for (caveId = '0'; caveId <= '9'; ++caveId) 
   {
     int cr = -1;
     int cc = -1;
     int reach[7][6];
     int nc = 0;

     for (r = 0; r < rows; ++r)
      for (c = 0; c < cols; ++c)
       if (field[r][c] == caveId) 
	   {
          cr = r;
          cc = c;
       }

     if (cr < 0) continue; 

     reach[cr][cc] = 1;
     nc = 1;

     while (1) 
	 {
        int updated = 0;

        for (r = 0; r < rows; ++r)
          for (c = 0; c < cols; ++c)
            if (reach[r][c]) 
			{
              if (r > 0 && field[r - 1][c] != '#' && !reach[r - 1][c]) 
			  {
                 reach[r - 1][c] = 1;
                 ++nc;
                 updated = 1;
              }

              if (c > 0 && field[r][c - 1] != '#' && !reach[r][c - 1]) 
			  {
                 reach[r][c - 1] = 1;
                 ++nc;
                 updated = 1;
              }

              if (c + 1 < cols && field[r][c + 1] != '#' && !reach[r][c + 1]) 
			  {
                 reach[r][c + 1] = 1;
                 ++nc;
                 updated = 1;
              }
            }

        if (!updated) break;
     } 

     for (r = 0; r <= cr; ++r)
      for (c = 0; c < cols; ++c)
       if (reach[r][c] && (c == 0 || !reach[r][c - 1])) 
	   {
         int c1 = c;
		 int pos;
		 struct Segment s = segments[segmentCount];
         while (reach[r][c1 + 1]) ++c1;
		 s.len = c1 - c + 1;
         for (pos = c; pos <= c1; ++pos) 
		 {
           if (r + 1 < rows && field[r + 1][pos] != '#') 
		   {
             if (reach[r + 1][pos])
               s.goodExitMask |= 1L << (pos - c);
             else
               s.badExitMask |= 1L << (pos - c);
           }
         }
         segmentCount++;
       }

      
      while (1) 
	  {
        int maxLen = 0;
		int len = 0;
        long badByLen[100];
        long badByLenCount = 0;
		long possible[100];
		long possibleCount = 0;
		struct Segment remaining[100]; 
		long remainingSize = 0;
        for (i = 0; i < segmentCount; i++)
			maxLen = (maxLen > segments[i].len) ? maxLen : segments[i].len;

		badByLenCount = maxLen + 1;
		possibleCount = maxLen + 1;
        for (i = 0; i < segmentCount; i++)
           badByLen[segments[i].len] |= segments[i].badExitMask;

        possible[1] = 1;
        for (len = 1; len <= maxLen; ++len) 
		{
           possible[len] &= ~badByLen[len];
           if (len < maxLen) 
              possible[len + 1] = possible[len] | (possible[len] << 1);
        }

        for (len = maxLen; len > 1; --len) 
		{
           possible[len - 1] &= possible[len] | (possible[len] >> 1);
        }

        for (i = 0; i < segmentCount; i++)
          if ((segments[i].goodExitMask & possible[segments[i].len]) == 0) 
		  {
             remaining[remainingSize++] = segments[i];
             remainingSize++;
		  }
      
        if (remainingSize == segmentCount) break;

        //segments = remaining;
        segmentCount = remainingSize;
    }
    
	  //System.out.println(caveId + ": " + nc + " " + (segments.size() == 1 ? "Lucky" : "Unlucky"));
   } 
}