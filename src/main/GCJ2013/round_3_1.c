
/*

Problem


You've been playing roulette for a while in a local casino. 
Roulette is a simple casino game in which multiple players place 
bets on one or more numbers between 0 and 36 (inclusive). 

Next, a wheel is spun in one direction with a ball spinning in the other direction. 
The roulette wheel contains the same numbers 0 to 36. 
Some real roulette wheels also have a space labeled 00, 
but ours does not. 

Eventually, the ball falls on one of the numbers. 
If a player placed a bet on that particular number, 
he receives 36 times his bet 
(so the profit of that bet is 35 times the bet). 

All bets placed on other numbers lose.

Unfortunately, luck hasn't been on your side, 
and you have been losing all night long. 
At one point, you started to wonder 
whether the roulette game was fair or not, 
and after observing the game some more, 
you noticed a pattern that must be profitable for the casino: 

the ball always lands on one of the numbers 
that has the least total money bet on it! 

If multiple numbers tie for the least total money bet, 
the ball lands on one of those uniformly at random.

Of course, you'll be notifying the authorities about this foul play, 
but first you want to win your money back 
by exploiting your new-found knowledge. 

To do so, you wait until all other players 
have placed their bets 
and then place bets of your own. 

Unfortunately, you have a limited budget left, 
so you cannot bet more than that. 

You are allowed to bet on zero or more different numbers, 
and each of those bets can be any positive integer amount 
(perhaps with different amounts for different numbers), 
so as long as the sum of your bets does not exceed your budget. 

What is the maximum expected profit you can make?



Input



The first line of input gives the number of cases, T. 
T test cases follow.
Each test case consists of two lines. 
The first line contains two integers: 
the budget you still have, B, 
and the number of numbers other players have placed bets on, N. 
The second line contains N integers Xi, 
the total amounts of money bet by other players 
on each of those different numbers.



Output


For each test case, output one line containing "Case #x: " 
followed by the maximum expected profit 
that you make if you place your bets optimally. 
A profit will be considered correct 
if it is within an absolute or relative error of 10-6 
of the correct answer. 
See the FAQ for an explanation of what that means, 
and what formats of floating-point numbers we accept.


Limits

1 <= T <= 100.
1 <= N <= 37.

Small dataset

1 <= B, Xi <= 1,000.

Large dataset

1 <= B, Xi <= 1012.

Sample


Input 
 	
Output 
 
3
100 1
10
34 3
5 6 7
34 4
1 1 10 10
Case #1: 0
Case #2: 2
Case #3: 0.9428571429

In example 2, bet 1 on each of the 34 empty numbers 
for a guaranteed payback of 36, 
and a profit of 36 - 34 = 2. 
In example 3, bet 1 on each of the 33 empty numbers, 
so that you win 36 with probability 33/35. 
The gives an expected profit of 33/35 * 36 - 33.





This problem challenges us with an 'intelligent' 
roulette game where the ball always lands on one of the numbers 
that has the least total money bet on it. 

Armed with this knowledge, 
we go to calculate the maximum expected profit. 
We know the current bets (need to be integers) and our budget. 

We now place bets (also integers) to maximize our expected profit.


Examples and insights


Before we delve into the solution, 
let’s go through a few examples to get an intuition 
for the solution strategy. 

First off, we describe the convention used in the figures.

a square in the figure represents a single unit of money
a column in the figure represents the current bet (pile of money) 
on some number colors of squares:

a red square represents the existing bets by other betters
a white square means there are no bets
a green square represents the bets we have already placed
a yellow square represents a bet we are considering
a blue square represents our bets that we are pointing out

In our figures, we show only 8 columns (different numbers) 
even though in our roulette game we have 37 different numbers, 
we assume all other numbers have much higher bets on them already

We sort the piles of money (red columns) 
by increasing height going from left to right

Let’s start out with a toy example (see figure a). 
Here, we have piles of heights 0, 0, 0, 2, 2, 3, 4, 4. 
Let’s say we have 3 units to bet: 
which piles should we bet on 
and how much should we bet per pile? 

In this case, we can bet on the three piles 
with height 0 (see figure b) 
which gives us an expected profit of 33.


Observation #1: 

As only the piles with minimum-height have a chance to win,
we want to try and place our bets on piles resulting in a minimum-height.

What if we had 6 units to bet? 
We can again place bets on piles with height 0 (see figure c). 
The result is 5 piles with height 2 
(3 green piles and 2 red piles). 

Our expected profit is 37.2. 
By the way, if the red piles of height 2 
were instead of height 3, 
our expected profit would have been 66!


What if we had 7 units to bet? 
We can place 6 units as described above. 
But where should we place the 7th one? 
We could try placing it in the blue locations in figure d, 
but that does not help at all. 

Or we could try placing it in any of the blue locations in figure e, 
which will change our expected profit as 
it will decrease the total number of piles with minimum-height 2. 

If we placed the 7th unit in the yellow position as in figure f, 
we will reduce our expected profit! 
But if we placed it as in figure g, 
our expected profit will increase to 47! 

It increases as we have decreased the number of piles 
with minimum-height that weren’t contributing to our expected profit.

 
Observation #2: 

We can possibly increase the expected profit 
by decreasing the number of piles with minimum-height.

What if we had 8 units to bet? 
You guessed it. 
We can place it as in figure h and get an expected profit of 64. 
In general, our optimal set of bets will have a step 
like figure of minimum-height h, 
and height h+1 or higher (as shown by the blue squares in figure i).



Simple strategy


Our simple strategy (for the small input) 
is to lay the bets one by one as shown in figure j. 
Each time we put a bet (e.g. in 1), 
we calculate the expected profit 
then place the next bet (i.e. 1 and 2) 
then calculate the expected profit, 
then repeat for the next one (1, 2 and 3). 
We retain the maximum expected profit.


This strategy works fine when the bet we have to place is small, 
but amount of money we can bet can get as large as 10^12! 
Therefore we need to use a strategy that will run faster.


Advanced strategy

Observation #3: 

For the optimal solution, 
the step from minimum-height h to h+1 (or higher) 
will occur at some vertical location (see figure k). 
At that vertical location, 
we want the highest minimum-height h possible with the amount of money we have.


Armed with this observation, 
we will essentially fix a vertical location 
and try to build as tall a step as possible. 
For example if we had 7 units of money 
and we fixed the vertical step location 
after the first column as shown in figure k, 
then the highest step we could build is shown in figure l, 
which uses 5 units of money (2 units remain unused). 

For the vertical step location after the fourth block 
as shown in figure m, we use all 7 units of money.

 
Therefore the strategy is to try all possible vertical locations, 
calculate the highest ‘step’ we can generate for each vertical location 
and calculate the expected profit. 
As usual, we retain the maximum one.


So how do we calculate the highest possible step 
given a vertical location? 

Doing it by adding squares one-by-one is going to be too slow, 
therefore we turn to our trusty friend binary search. 
We want to determine the highest minimum-height step for that vertical location. 
Notice that since the amount of money (squares) required to build 
each ‘step’ is monotonically increasing, we can perform binary search.



*/


#include <stdio.h>


int Pr_2013_3_1_main() 
{
  int tt, qq;
  scanf("%d", &tt);
  for (qq=1;qq<=tt;qq++) 
  {
	long long a[42], b;
	long double ans = 0.0;
    int n,i,p,k;
    printf("Case #%d: ", qq);
    //cin >> b >> n;
	//b-the budget you still have, B
	//n-the number of numbers other players have placed bets on, N
	//a-The second line contains N integers Xi, 
    //  the total amounts of money bet by other players 
    //  on each of those different numbers.
    //for (i=1;i<=n;i++) cin >> a[i];
	/* Fill in other people bets by 0 */
    for (i=n+1;i<=37;i++) a[i] = 0;
    //sort(a+1, a+37+1);
    a[38] = (long long)1e16;
	/* Move vertical line */
    for (p=1;p<=37;p++) 
    for (k=1;k<=p;k++) 
	{
	  long long loss;
	  long long r2, rg2;
	  long double c;
      long long lg = a[p];
      long long rg = a[p+1]-1;
      if (lg > rg) continue;

      /* Here we need an example
	   * | | | | | | | | |
	   * | | | | | | | | |
	   * | | | | | | |r|r|
	   * | | | | | |r|r|r|
	   * | | | |r|r|r|r|r|
	   * | | | |r|r|r|r|r|
	   *
	   * Let's fix p = 1, k = 1
	   * a[p] = 0, a[p+1] - 1 = 0 - 1 = -1
	   * we skip it
	   * 
	   * Let's take p = 3, k = 1
	   * a[3] = 0. rg = a[4]-1 = 2 - 1 = 1
	   * loss = 3 - 1 = 2
	   * rg2 = (7-2)/3 = 5/3 
	   * c = -2
	   * 
	   * if (p < 36) c += (36.0-p)*rg;
       *  (36.0-3)*1 = 33
	   * 
	   * p = 3, k = 2
	   * loss = 3-2=1, rg2=(7-1)/3 = 2
	   * c = -1, -1 + 
	   * 
	   * 
	   */
	  loss = p-k;
      for (i=1;i<=p;i++) loss -= a[i];
      rg2 = (b-loss)/p;
      if (rg2 < rg) rg = rg2;
      if (lg > rg) continue;

	  c = -(p-k);
      for (i=1;i<=k;i++) c -= 36.0/k*a[i];
      for (i=1;i<=p;i++) c += a[i];

      if (p < 36) c += (36.0-p)*rg;
      else c += (36.0-p)*lg;

      if (c > ans) ans = c;
	}
  }
}

int n;
int m;
int x[37];

double Pr_2013_3_1_max(double a, double b)
{
	return (a > b) ? a : b;
}

int  Pr_2013_3_1_main_() 
{
	int tt, it, i;
	//cin >> tt;
	for (it = 1; it <= tt; it++) 
	{
		double ans = 0;
		int cur = 0;
		//cin >> m >> n;
		memset (x, 0, sizeof (x));
		//for (int i = 0; i < n; i++) cin >> x[i];
		//sort (x, x + 37);
		for (i = 0; i < 37; i++) 
		{
			double l, r;
			long tmp = 0;
            int j = 0;

			cur += x[i];

			l = 0, r = 1e15;
			/* We need a test example
			 * | | | | | | | | |
	         * | | | | |.|.|.|.|
	         * |.|.|.|.|.|.|r|r|
	         * |.|.|.|.|.|r|r|r|
	         * |.|.|.|r|r|r|r|r|
	         * |.|.|.|r|r|r|r|r|
			 *  
			 * (4 * 4 - 2) / 
			 * l = 0, r = 20, s = 10
			 * s = 4,
			 * i = 4,
			 * j = 1, tmp = 4 - 0 = 4
			 * j = 2, tmp = 4 - 0 = 4
			 * j = 3, tmp = 4 - 0 = 4
			 * j = 4, tmp = 4 - 2 = 4
			 * j = 5, tmp = 5 - 2 = 3
			 * j = 6, tmp = 5 - 3 = 2
			 * j = 7, tmp = 5 - 4 = 1
			 * j = 8, tmp = 5 - 4 = 1
			 * 
			 * i     = 4
			 * i + 1 = 5
			 * l     = 4
			 * (i + 1) * l = 5 * 4 = 20 - 
			 * 
			 * ans = max(ans, ((i + 1) * l - cur) * 36 / (i + 1.0) - tmp);
			 * = 36 * (l - cur/(i+1)) - tmp
			 */
			while (r - l > 1) 
			{
				double s = (l + r) / 2;
				double tmp = 0;
				int j;
				for (j = 0; j < 37; j++)
					if (j <= i) 
						tmp += Pr_2013_3_1_max(0, s - x[j]);
					else
						tmp += Pr_2013_3_1_max(0, (s + 1) - x[j]);
				if (tmp <= m) 
					l = s; 
				else 
					r = s;
			}
			
			for (j = 0; j < 37; j++)
				if (j <= i) 
					tmp += Pr_2013_3_1_max(0, l - x[j]);
				else
					tmp += Pr_2013_3_1_max(0, (l + 1) - x[j]);

			if (l >= x[i]) 
				ans = Pr_2013_3_1_max(ans, ((i + 1) * l - cur) * 36 / (i + 1.0) - tmp);
		}		
	}
	return 0;
}