/*  GCJ R3 2012 
 

Problem
 
You're playing a video game, in which you will get an achievement 
if you complete all of the levels consecutively without dying. 

You can play the levels in any order, and each time you play a level 
you'll either complete it or die. 

Each level has some probability that you'll complete it, and takes some amount of time. 
In what order should you play the levels so that the expected time 
it takes you to get the achievement is minimized? 

Assume that it takes equally long to beat a level or to die in it, 
and that you will start again from the first level in your ordering as soon as you die.
 
Note: 
If you fail to complete a level, you do not personally die
—only your character in the game dies. 
If that were not the case, only a few people would try to earn this achievement.
 
Input
 
The first line of the input gives the number of test cases, T. 
T test cases follow, each of which consists of three lines. 
The first line of each test case contains a single integer N, the number of levels.

The second line contains N space-separated integers Li. 
Li is the number of seconds level i lasts, 
which is independent of whether you complete the level or die. 
The third line contains N space-separated integers Pi. 
Pi is the percent chance that you will die in any given attempt to complete level i.
 
Output
 
For each test case, output one line containing "Case #x: ", 
where x is the case number (starting from 1), 
followed by N space-separated integers. 

The jth integer in the list should be the index of the jth level 
you should attempt to beat in order to minimize the amount of time 
you expect to spend earning the achievement.
 
Indices go from 0 to N-1. If there are multiple orderings 
that would give the same expected time, 
output the lexicographically least ordering. 

Out of two orderings, the lexicographically smaller one is 
the one with the smaller index at the first location where they differ;

out of many orderings, the lexicographically least one is 
the one that is lexicographically smaller than every other ordering.
 
Limits
 
1 ≤ T ≤ 100.
0 ≤ Pi < 100.
 

Small dataset
 
1 ≤ N ≤ 20.
Li = 1.
 
Large dataset
 
1 ≤ N ≤ 1000.
1 ≤ Li ≤ 100.
 
Sample
Input 
Output 
3
 4
 1 1 1 1
 50 0 20 20
 3
 100 10 1
 0 50 0
 3
 100 80 50
 40 20 80
 

Case #1: 0 2 3 1
 Case #2: 1 0 2
 Case #3: 2 0 1
 




 Solution



 Preamble
 
 It will come as no great surprise that this problem was inspired by 
 a video game that the author played: Super Meat Boy. 
 
 Turning the real achievement challenge into a Code Jam problem 
 involved a little simplifying: 
 it doesn't take the same amount of time to lose a level as to win it, 
 
 for example.
 
 Total Expected Time and Number of Attempts

 Your total expected time to complete the achievement will be the expected number of attempts, 
 times the expected time per attempt.
  
 First, let's figure out how many attempts it's going to take us 
 to complete the achievement. 

 The probability of success in any given attempt is easy to compute, 
 since it's simply the probability of succeeding on every level 
 once without failing any of them: (1-P1)*(1-P2)*...*(1-PN). 
 
 The expected number of attempts won't depend on anything other 
 than this probability, and this probability doesn't depend on 
 the order of the levels; 
 
 so the expected number of attempts doesn't depend on the order of the levels, 
 and we can ignore it when figuring out what order to attempt them in.
 
 In case you're curious about what the expected number of attempts is anyway, read on. 
 
 Let's say that we're going to try to do something that might fail, 
 like complete this achievement, and that we're going to keep 
 trying until we succeed. 
 
 If the probability of success is P, 
 then what we're doing is called a Bernoulli Trial, 
 repeated until we achieve success. 
 
 The number of attempts that will take falls into 
 a random distribution called the Geometric Distribution. 
 
 The expected value of this distribution
 –the number of attempts it will take–is 1/P.
 

 Knowing about the Geometric Distribution lets us compute 
 the expected number of attempts easily. 
 
 The chance that we'll complete every level successfully, 
 one after another, is the product of the probabilities of success, 
 which we'll call P; 
 
 the number of attempts it will take us to complete that is 1/P. 
 As promised, this number doesn't depend on the order 
 in which we attempt the levels, and since what we're trying to do 
 is compute the best order, we're going to ignore it.
 

 Time Per Attempt

 
 Let's choose an arbitrary order for the levels. 
 Suppose the ith of those levels takes ti time, 
 and you die in that level with probability pi. 
 
 In any attempt, you'll definitely reach level 1; 
 you'll reach level 2 with probability 1-p1; 
 you'll reach level 3 with probability (1-p1)*(1-p2); 
 and so on.
 
 Based on those calculations, the amount of time 
 it takes you to make one attempt will be expected 
 time = t1 + (1-p1)t2 + (1-p1)(1-p2)t3 + .... 
 
 This is because you will try level i only 
 if you pass the first i-1 levels.
 
 Now, let's consider what would happen to that expected time 
 if we swapped levels i and i+1. 

 Only two of the terms of the expected time equation 
 would be affected—the others would simply reverse the order 
 of (1-pi) and (1-pi+1), which doesn't matter. 
 
 Those two terms themselves have several multiplicative terms in common:
 
 Pre-swap:
    (1-p1)(1-p2)...(1-pi-1)ti +
    (1-p1)(1-p2)...(1-pi-1)(1-pi)ti+1.

 Post-swap:
    (1-p1)(1-p2)...(1-pi-1)ti+1 +
    (1-p1)(1-p2)...(1-pi-1)(1-pi+1)ti. 

 So we're better off post-swap iff:
    ti + (1-pi)ti+1 > ti+1 + (1-pi+1)ti
    tipi+1 > ti+1pi

 Now we can compare two adjacent levels to see whether 
 they should be swapped. 
 Doing this greedily results in a stable sort of the levels. 
 
 With a little more work, you can prove that non-adjacent 
 levels should be swapped under the same conditions, 
 and that the stable sort is thus optimal.
 
 Implementation Details
 
 Our last inequality above said we should swap iff tipi+1 > ti+1pi. 
 It's tempting to go one step further, to ti/pi > ti+1/pi+1, 
 but we can't: pi or pi+1 could be zero, 
 and dividing by zero isn't a mathematically valid thing to do.
 
 Another reason not to make that final step
 –though the final step actually kind of works 
 if you don't mind special-casing pi=0 
 to give an infinite score–
 
 is because division almost inevitably makes us deal 
 with floating-point numbers. 
 In a problem like this where we're trying to make 
 a decision based on numbers, 
 we want to make sure we're doing exact comparisons. 
 
 We wouldn't want to reverse two equivalent levels 
 because ti/pi = 10.0000000001 and ti+1/pi+1 = 10.0.


 */



/* Solution
 
   If we have n levels: 1, 2, 3,..., n-1, n to go through
   p1, p2, p3,...,pn-1, pn are possibilities of successful outcomes
   for all levels. The possibility of successful way through all levels
   is (1-p1)(1-p2)...(1-pn-1)(1-pn) and this does not depend on the order
   of level to go through.
   p = (1-p1)(1-p2)...(1-pn-1)(1-pn).

   Then we need to find the number of steps needed for the first success.
   We have f(x) a discrete random variable and we know the value of this 
   function in the points: 1, 2, 3,..., k - 1, k, ....

   f(k) - is the situation when we get success after k failures
   f(k) = (1-p)(1-p)*...*(1-p)p = p(1-p)^(k-1)
                   k-1 times

   Then we need to find an average value:

   <f> = sum(pi*i)/sum(pi) = sum(kp(1-p)^(k-1)) / 1 = 
   = 
   1 + p + p^1 + p^2 + ... + p^k + ... = 1/(1-p) =
   d/dp(1 + p + p^1 + p^2 + ... + p^k) = 1 + p + 2p^1 + 3p^2 + ... + kp^(k-1) + ... = 
   sum(pk(1-p)^(k-1)) = p * sum(k(1-p)^(k-1)) = p * d/dp(1/(1-(1-p))) = 
   = p * d/dp(1/p)) = p(-1)p^-2 = 1/p
  
  1/p - is the number of attempts needed for the first success of going through
        all levels.      
  
  Then we need to find a minimal time to achieve that:
  Suppose that we go through an arbitrary order of all levels:
  1, 2, 3,..., i - 1, i, i + 1, ... 
  
  It takes ti time for the ith level and with a probability pi a player die in
  this level.

  A player definitely reach the level 1 with probability 1 and time t1
  level 2 with probability (1-p1) - we must not die on the first level.
  level 3 with probability (1-p1)(1-p2).

  So we have a total amount of time:

  t1 + (1-p1)t2 + (1-p1)(1-p2)t3 + ... + (1-p1)(1-p2)...(1-pk-1)tk + ...

  Lets see how the change of order affects the total sum.
  
  Before swap:

  ... + (1-p1)(1-p2)*...*(1-pi-1)ti +
      + (1-p1)(1-p2)*...*(1-pi-1)(1-pi)ti+1 + ...

  After swap:

  ... + (1-p1)(1-p2)*...*(1-pi-1)ti+1 +
      + (1-p1)(1-p2)*...*(1-pi-1)(1-pi+1)ti + ...

  Now subtraction these two values we get:
  (1-p1)(1-p2)...(1-pi-1)[ ti+1 + (1-pi+1)ti - ti - (1-pi)ti+1 ] = 
  (1-p1)(1-p2)...(1-pi-1)[ ti+1 + ti - pi+1ti - ti - ti+1 + piti+1 ] = 
  (1-p1)(1-p2)...(1-pi-1)[ piti+1 - pi+1ti ]

  So for making this solution efficient we need achieve:
  piti+1 - pi+1ti < 0;
  piti+1 < pi+1ti
 
 */

struct Level
{
	int Time;
	int Prob;
	int Id;
};

void Pr6Swap(struct Level Levels[], int i, int j)
{
    struct Level temp;
	temp = Levels[i];
    Levels[i] = Levels[j];
    Levels[j] = temp;
}

int Compare(struct Level leftLevel, struct Level rightLevel)
{   
	/* piti+1 < pi+1ti */
	int diff = leftLevel.Prob * rightLevel.Time - rightLevel.Prob * leftLevel.Time;
	/* If diff is not 0 then we have 2 cases:
	 * 1. diff > 0 - means that rightLevel > leftLevel
	 * 2. diff < 0 - means that rightLevel < leftLevel
	 */
	if (diff != 0)
		return diff;
    /* If diff == 0 we use lexigraphical order: compare identifiers. */
	return leftLevel.Id < rightLevel.Id;
}

void Pr6HeapSort(struct Level Levels[], int start, int end)
{
	int medium, i, j;
    int compResult;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
	Pr6Swap(Levels, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
       if ((compResult = Compare(Levels[i], Levels[medium])) < 0)
	   {
		   Pr6Swap(Levels, i, j++);
	   }    
	}
  
    Pr6Swap(Levels, start, j - 1);
    Pr6HeapSort(Levels, start, medium - 1);
    Pr6HeapSort(Levels, medium + 1, end);
}

void Pr6SolveLarge(struct Level Levels[], int N)
{
    Pr6HeapSort(Levels, 0, N - 1);
}

