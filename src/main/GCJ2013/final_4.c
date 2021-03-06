
/*
 
 Problem

 This problem was inspired by a board game called Can't Stop, 
 designed by Sid Sackson. 
 This problem has a similar idea, 
 but does not assume you have played Can't Stop.

 You're playing a (very large) board game. 
 In this game, you're given a sequence of N roll sets. 
 Each roll set consists of D die rolls. 
 Each die roll is an integer.

 To win the game, 
 you have to find the largest totally awesome interval of the sequence. 
 An interval is any consecutive sequence of roll sets. 

 An interval is called totally awesome 
 if there exist k numbers such that 
 every roll set in the interval 
 contains at least one of those k numbers.

 For example, suppose D=2 and k=3, 
 and the roll sets are as follows:

 Set 0: 10 20
 Set 1: 50 60
 Set 2: 70 30
 Set 3: 40 40
 Set 4: 30 30
 Set 5: 20 40

 The interval from Set 0 to Set 2 
 is totally awesome because roll sets 0-2 
 all contain 10, 50 or 70. 
 
 The interval from Set 1 to Set 5 is totally awesome 
 because roll sets 1-5 all contain 50, 30 or 40. 
 
 That interval contains 5 roll sets, 
 and it is the largest totally awesome interval.

 Your job is to output the indices of the first 
 and last roll set in the longest totally awesome interval. 
 
 If there are multiple totally awesome intervals of that length, 
 output the indices for the one with the lowest first index. 
 Note that the first roll set has index 0.

 Input

 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case starts with three space-separated integers: 
 N, D and k, as described above. 
 On the next line, there will be N*D integers. 
 The first D integers will be the rolls from the first roll set; 
 the second D integers will be the rolls from the second roll set; 
 and so on.

 Output

 For each test case, output one line containing "Case #x: y z", 
 where x is the case number (starting from 1), 
 and y and z are the first and last indices 
 of the longest totally awesome interval 
 (with ties broken using the lowest index), 
 as described above.

 Limits

 1 ≤ T ≤ 100.
 1 ≤ D ≤ 4.
 1 ≤ every die roll ≤ 105.
 For 6 test cases, 1 ≤ N ≤ 105.
 For all the other test cases, 1 ≤ N ≤ 103.
 Small dataset

 k = 2.
 Large dataset

 2 ≤ k ≤ 3.
 Sample


 Input 
 	
 Output 
 
 4
 8 1 2
 1 2 3 2 4 5 4 6
 4 3 2
 1 2 3 4 5 6 7 8 9 10 11 12
 6 2 3
 10 20 50 60 70 30 40 40 30 30 20 40
 10 1 3
 2 4 3 1 4 5 3 1 1 2

 Case #1: 1 3
 Case #2: 0 1
 Case #3: 1 5
 Case #4: 1 4

 The board game Can't Stop was designed by Sid Sackson, 
 and has been published by many publishers. 
 Neither Mr. Sackson nor any of the publishers endorses, 
 or has any involvement with, Google Code Jam.





 Divide and Conquer


 There is a nice divide-and-conquer solution. 
 Split the input down the middle; 
 the best interval is either entirely in the left half, 
 entirely in the right half, 
 or crosses the middle. 
 We handle the right and left cases recursively; 

 if we can handle the "crosses" case in linear time, 
 we will have an O(N lg N) algorithm, 
 which is good enough.

 If the best interval crosses the middle, 
 we know it must use the middle element, 
 so we must pick one of the D numbers in that roll to use. 
 
 Now go out as far as we can to the left and right with that number. 
 Once we stop, 
 we can extend either to the left or the right, 
 so there are 2D different numbers to try. 
 
 Expand again, 
 and get 2D more numbers to try for a final expansion. 
 
 In total, we only tried D*2D*2D different choices, 
 so the whole thing runs in 256N time, and we get our O(N lg N) time algorithm.

 Note: 
 surprisingly for a divide and conquer algorithm, 
 we don't use any information from the left and right halves 
 to solve the middle case; 
 the fact that it crosses is enough.
 

 Slow and Simple


 There's another set of approaches to this problem.
 Try every starting position, 
 expanding out to the right 
 and only making choices when necessary 
 (just as in the divide-and-conquer). 

 This is O(D^K * N) for each starting position, 
 or O(D^K * N^2) overall, which is too slow. 
 However, there are several different improvements 
 (some small, some large) 
 one could make in order to make this linear in N.



 Linear Time

 One improvement works in this way: 
 Every time you pick a number, 
 check if the roll before your starting position contains that number. 
 
 If so, we can safely ignore that choice 
 (because we would have done better starting one roll earlier 
  and choosing the same set of numbers).

 It turns out that this runs in linear time, 
 but the reason why isn't obvious. 
 
 We want to prove that a particular index i 
 is only visited from O(1) starting positions. 
 
 Imagine we have reached i from start. 
 Now imagine starting at i and going leftward 
 (as usual, we expand left as far as possible before picking each new number). 
 
 If we only choose numbers from the set that got us from start to i, 
 we must get back to start 
 (and no further, since the optimization 
  above guaranteed that start - 1 doesn't contain any of our numbers). 
  
  But there are only 1 + D + D2 + D3 different choices of numbers starting from i, so there are only that many possible positions for start, so i will only be visited O(D3) times.




 */





/***********************************  Proble 1   ************************************/


int N, D, k;

struct RollSet 
{
  int val[4];
  int next_without[4];

  /*int where(int x)
  {
    for (int i=0; i<D; ++i) 
	{
      if (val[i] == x) 
	  {
        return i;
      }
    }
    return -1;
  }*/
};

// vector<RollSet> rolls;
// vector<int> xs;
// vector<pair<int, int>> num_pos;

// pos = a from interval [0..N]
void rek(int pos, int* farthest) 
{
  int j;
  if (/*(int)xs.size() == k || */pos == N) 
  {
     if (pos-1 > farthest)
       farthest = pos-1;
	 return;
  }
  
  /* Form teh start:
   * rek(0, 0) farthest = 0
   * pos = 0. next = 1, xs = rol[0][0] = 10.
   * w != -1, w = 0, next = 2.
   * 
   * rek(2, 0) farthest = 1.
   * pos = 2, next = 3. xs = (10, 30)
   */
  for (j=0; j<D; ++j) 
  {
    // xs.push_back(rolls[pos].val[j]);
    int next = pos+1;

    if (next < N) 
	{
      int progress = 1;
      while (progress) 
	  {
         progress = 0;
         //for (int x : xs) 
		 {
             int w = 0; //rolls[next].where(x);
             if (w != -1) 
			 {
                next = 0; //rolls[next].next_without[w];
                if (next == N) 
			    {
                   progress = 0;
                   break;
                }
                progress = 1;
            }
         }
      }
    }
    rek(next, farthest);
    //xs.pop_back();
  }
}

/*

D = 2, k = 3

Set 0: 10 20
Set 1: 10 30
Set 2: 30 50
Set 3: 50 60
Set 4: 70 30
Set 5: 40 40
Set 6: 30 30
Set 7: 20 40

*/

void final_4_solve1() 
{
  // cin >> N >> D >> k;
  // rolls.resize(N);
  // num_pos.clear();
  int i, j, a;

  /*       val  0  1  2  3    
     rolls next 0  1  2  3
	   0        10 20 -  -
                2  1  -  - 
	   1        10 30 -  -
	            2  3  -  -
	   2        30 50 -  -
	            3  4  -  -
	   3        50 60 -  -
                4  4  -  - 
	   4        70 30 -  - 
                5  5  -  - 
	   5        40 40 -  - 
                6  6  -  - 
	   6        30 30 -  - 
                7  7  -  - 
	   7        20 40 -  - 
                -  -  -  - 
   */
  for (i=0; i<N; ++i) 
  {
    for (j=0; j<D; ++j) 
	{
      int x;
      //cin >> x;
      //rolls[i].val[j] = x;
      //num_pos.push_back(make_pair(x, i));
    }
  }

  for (j=0; j<D; ++j) 
  {
    //rolls[N-1].next_without[j] = N;
  }

  /* i = N -2 = 4. 
   * j = 0, x = rolls[4][0] = 30. rv = -1. 
   * i = N -3 = 3
   */
  for (i=N-2; i>=0; --i) 
  {
    for (j=0; j<D; ++j) 
	{
      int x = 0;  //rolls[i].val[j];
      int rv = 0; //rolls[i+1].where(x);
      if (rv != -1) 
	  {
         //rolls[i].next_without[j] = 0; //rolls[i+1].next_without[rv];
      } 
	  else 
	  {
         //rolls[i].next_without[j] = i+1;
      }
    }
  }

  //pair<int, int> ans(0, 0);
  for (a=0; a<N; ++a) 
  {
    int b = a;
    rek(a, b);
    /*if (b - a > ans.second - ans.first) 
	{
      ans = make_pair(a, b);
    }*/
  }
  // cout << ans.first << ' ' << ans.second << '\n';
}

int final_4_main() 
{
  int T; //cin >> T;
  int tt;

  for (tt=1; tt<=T; ++tt) 
  {
    //cout << "Case #" << tt << ": ";
    final_4_solve1();
  }

  return 0;
}









