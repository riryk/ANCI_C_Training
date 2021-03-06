
/*


Problem

An observation wheel consists of N passenger gondolas arranged in a circle, 
which is slowly rotating. 

Gondolas pass the entrance one by one, 
and when a gondola passes the entrance, 
a person may enter that gondola.


In this problem, the gondolas are so small 
that they can take just one person each, 
so if the gondola passing by the entrance is already occupied, 
the person waiting at the entrance 
will have to wait for the next one to arrive. 

If that gondola is also occupied, 
the person will have to wait for the next one after that, 
and so on, until a free gondola arrives. 

For simplicity, we will not consider people exiting 
the gondolas in this problem 
— let's assume that all people do 
is enter the gondolas, 
and then rotate with the wheel for an arbitrarily long time.

We want to make sure people are not disappointed 
because of long waiting times, 
and so we have introduced a flexible pricing scheme: 

when a person approaches the wheel, 
and the first gondola passing by the entrance is free, 
she pays N dollars for the ride. 

If the first gondola is occupied and she has to wait for the second one, 
she pays N-1 dollars for the ride. 

If the first two gondolas are occupied 
and she has to wait for the third one, 
she pays N-2 dollars for the ride. 

Generally, if she has to wait for K occupied gondolas to pass by, 
she pays N-K dollars. 

In the worst case, when she has to wait for all but one gondola to pass, 
she will pay just 1 dollar.

Let's assume that people approach our wheel at random moments in time, 
so for each person approaching the wheel, 
the first gondola to pass the entrance is picked uniformly and independently. 

Let's also assume that nobody will come to the wheel 
while there's already at least one person waiting to enter, 
so that we don't have to deal with queueing. 

A person will always take the first free gondola that passes the entrance.

You are given the number of gondolas and which gondolas are already occupied. 
How much money are we going to make, on average, until all gondolas become occupied?


Input


The first line of the input gives the number of test cases, T. 
T lines follow. 
Each line describes one test case and contains only '.' (dot) 
or 'X' (capital letter X) characters. 
The number of characters in this line gives you N. 
The i-th character is 'X' 
when the i-th gondola is already occupied, 
and '.' when it's still free. 
The gondolas are numbered in the order 
they pass the entrance, 
so the 1st gondola is followed by the 2nd gondola, 
and so on, starting over 
from the beginning after the last gondola passes.


Output

For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the average amount of money we will get, in dollars. 
Answers with absolute or relative error no larger than 10-9 will be accepted. 
See the FAQ for an explanation of what that means, 
and what formats of floating-point numbers we accept.

Limits

1 ≤ T ≤ 50.

Small dataset

1 ≤ N ≤ 20.

Large dataset

1 ≤ N ≤ 200.

Sample


Input 
 	
Output 
 
5
.X.
X.X.
.XX.
X..XX.
.XX..X
Case #1: 4.66666666666667
Case #2: 6.00000000000000
Case #3: 5.75000000000000
Case #4: 13.4722222222222
Case #5: 13.5277777777778

Notes

Here's how the first example works. There are nine possibilities, each with probability 1/9:

The first person comes. If the next gondola to pass the entrance is:

The 1st gondola, which is free, the first person enters it and pays 3 dollars. 
   Then, some time later, the second person comes. 
   If the next gondola to pass the entrance is:
     The 1st gondola, which is occupied, 
	 and so is the 2nd gondola, 
	 the second person has to wait until the 3rd gondola, 
	 and thus she pays just 1 dollar before entering it. 
	 In total, we've earned 4 dollars.

     The 2nd gondola, which is occupied, 
	 the second person has to skip it 
	 and enter the 3rd gondola and thus pays 2 dollars. 
	 In total, we've earned 5 dollars.

     The 3rd gondola, which is free, 
	 so the second person pays 3 dollars. 
	 In total, we've earned 6 dollars.

The 2nd gondola, which is occupied, 
the first person has to skip it 
and enter the 3rd gondola, paying 2 dollars. 

Then, some time later, the second person comes. 
If the next gondola to pass the entrance is:

   The 1st gondola, which is free, 
   the second person pays 3 dollars. 
   In total, we've earned 5 dollars.

   The 2nd gondola, which is occupied (as is the 3rd gondola), 
   the second person has to wait until the 1st gondola, 
   and thus she pays just 1 dollar before entering it. 
   In total, we've earned 3 dollars.

   The 3rd gondola, which is occupied, 
   the second person has to skip it 
   and enter the 1st gondola and thus pays 2 dollars. 
   In total, we've earned 4 dollars.

The 3rd gondola, which is free, 
the first person enters it and pays 3 dollars. 
Then, some time later, the second person comes. 
If the next gondola to pass the entrance is:

    The 1st gondola, which is free, 
	the second person pays 3 dollars. 
	In total, we've earned 6 dollars.

    The 2nd gondola, which is occupied (as is the 3rd gondola), 
	the second person has to wait until the 1st gondola, 
	and thus she pays just 1 dollar before entering it. 
	In total, we've earned 4 dollars.

    The 3rd gondola, which is occupied, 
	the second person has to skip it 
	and enter the 1st gondola and thus pays 2 dollars. 
	In total, we've earned 5 dollars.

We have nine possibilities, 
earning 3 dollars in one of them, 
4 dollars in three of them, 
5 dollars in three of them, 
and 6 dollars in two of them. 
On average, we earn (1*3+3*4+3*5+2*6)/9=42/9=4.6666666666... dollars.








Small dataset


In this problem we are interested in 
calculating the average amount of money 
we will make from filling up every gondola 
on our observation wheel. 

Because of linearity of expectation,
this is equivalent to summing up 
the expected amount of money paid by each person. 

Given the fact that the amount a person pays 
doesn’t depend on the order in which gondolas got occupied, 
we can represent the current free/occupied state of gondolas 
as a bitmask and use dynamic programming 
to solve the small case.

Let E(mask) be the expected amount of money 
we make starting from the configuration represented by mask, 
where 0 represents an empty gondola, 
and 1 represents an occupied gondola. 
A person has a probability of 1/N of starting at any given position. 
Once that starting position is fixed, 
we simply find the first 0 following it (in cyclic order), 
and that’s where the person will eventually end up.

We’ll define f(i) as the index of 
the gondola occupied by a person who starts at position i. 
Similarly, let c(i) be the amount of money this person pays, 
as per the problem statement. 
Then we have the following recurrence:

E(mask) = (1/N) * ∑i=0..N-1(E(mask | (1 << f(i)) ) + c(i))

The bitwise operation here simply sets 
the bit corresponding to the gondola the user occupied. 

The base case is when there are no empty positions, 
in which case the expected amount of money is 0.

There are 2^N states, 
each of which can be computed in linear time, 
so our time complexity is O(N*2^N). 

This is pretty easy for the small data set, 
but unfortunately it’s far too slow for the large case.




Large dataset



Let's analyze the problem from the end: 
one of our gondolas will be the last to become occupied, 
so we have several cases, 
one per each free gondola at the beginning. 

The expected amount of money we will get 
is equal to the sum of 
the expected amount of money 
we make in each of those cases multiplied by the probability of that case.

At first, it doesn't seem we've reduced 
the complexity of the problem: 

instead of having to find just the expected amount for the whole process, 
we now have to find both the expected amount 
and the probability for several cases! 

However, we can repeat the above trick. 
Take one of those cases, 
let's say that gondola i 
is the last to become occupied. 

Let's look at which gondola will be occupied directly before it. 
Let's say it's gondola j. 
And here's the breakthrough: 

as soon as we've fixed that gondolas i and j 
are the last two to become occupied, 

the observation wheel has been separated 
into two independent parts 
that don't affect each other: 

those between i and j, 
and those between j and i. 

They don't affect each other since i and j stay empty, 
and thus no person approaching one part 
of the wheel will end up in the other part.

Our approach in general will be to compute E(i, j), 
the expected amount of money we get from 
all gondolas from i-th until (j-1)-th, 
excluding j-th gondola itself which will stay empty. 

It is possible to have i > j 
since we’re dealing with a cyclic problem, 
so keep this in mind when implementing things. 

We basically start at i and proceed around the circle, 
stopping just short of j.


To compute expectations, 
we’re going to need probabilities, 
so first let’s look at P(i, j), 
the probability that j-th gondola will stay empty while 
we fill up all gondolas from the interval [i, j) 
assuming each coming person approaches 
some gondola in inteval [i, j] 
(note that j is included here). 
We can develop a recurrence to compute this.


Suppose we know that 
the last person enters 
the gondola at position (i + k). 

This splits the interval into two parts, 
with 
a empty squares on the left, 
b empty squares on the right, 
and 1 more empty square at (i+k).

The probability that gondola j stays empty 
while we fill interval [i, j) 
and that gondola at position (i+k) 
is filled last is P(i, j, k) 
and can be computed as:

P(i, j, k) = 
    C(a+b, a) *
	(k+1/j-i+1)^a+1 *
	(j-i-k/j-i+1)^b *
	P(i, i+k) *
	P(i+k+1, j)

a + 1 people must go to the left side
b people must go to the right side 

            j-i+1
| | | | | .... | | | * | | ... | |      
    k+1                  (j-i+1)-(k+1)=j-i-k 
  

Here C(n, k) is the binomial coefficient 
representing the number of ways 
to choose k objects from a set of n. 

The equation above amounts 
to choosing a people from (a+b) 
to go on the left side of the final empty space, 

and then making sure that (a+1) people go 
to the left side 
(including the person to fill up gondola i+k) 
and b people go to the right side. 

The probability that gondola i+k 
will stay empty is P(i, i+k), 

and the probability that gondola j will stay empty is P(i+k+1, j).

This assumes that (i+k) is empty initially, 
otherwise we define P(i, j, k) = 0.


Of course, we can’t actually fix the final person, 
but since every way to fill up 
the interval has some final person, 
we can just compute the probability 
that gondola j will stay empty as the sum of 
P(i, j, k) over all possible final positions k, giving:


P(i, j)=∑k=0..j-i-1P(i, j, k)


For our base case, we have P(i, j, k) = 1 
if the interval [i, j) contains no free gondolas. 


The probability that gondola j stays empty 
while we fill interval [i, j) 
and that gondola at position (i+k) 
is filled last is P(i, j, k) 
and can be computed as


The probability that gondola j stays empty 
while we fill interval [i, j) 
and that gondola at position (i+k) 
is filled last is P(i, j, k) 
and can be computed as:


This also includes the case where the interval is of size 0. 
Don’t forget, we’re still in a cyclic situation!

On to computing expectations! 

We’ll use the same trick of splitting around the last person. 

The expected money we get 
while filling out the interval [i, j) 
so that the last filled gondola is at position (i+k) is:

E(i, j, k) = E(i, i+k) + E(i+k+1, j) + N - k/2

Summing over all possible k to get the expectation, we get:

E(i, j)=(∑k=0..j-i-1P(i, j, k)*E(i, j, k))/(∑k=0..j-i-1P(i, j, k))
∑k=0..j-i-1P(i, j, k) = P(i,j)

E(i, j)=(∑k=0..j-i-1P(i, j, k)*E(i, j, k))/P(i, j)

The way the first equation works 
is to combine the expectations 
from the left interval and the right interval, 
and then we need the expected number of skips to place the final person. 

There are (k+1) starting positions, 
corresponding to 0 skips, 1 skip, …, k skips. 

Each of these is equally likely, so the expectation is N - 1/(k+1)*(0 + 1 + ... + k)=N - k/2.

As before E(i, j, k) = 0 if gondola at position (i+k) is occupied.

To compute the final answer, 
we’ll repeat the same trick in the final step. 

We try all possible empty positions 
as the last gondola to be filled 
and compute the expected number of skips. 

If the last empty position is i, then the expected money we get is:

P(i+1, i)*(E(i+1, i) + (N+1)/2),

and the total expected amount of money 
is just the sum of this quantity over all empty positions.

This algorithm is O(N^3), which is easily within the time limit for the large case.



*/



#include <stdio.h>
#include <string.h>

double f[1111111];

void GCJ2013_3_4_small() 
{
  int tt,qq;
  scanf("%d", &tt);
  for (qq=1;qq<=tt;qq++) 
  {
	char s[42];//gongolas occupation
	int n,t,i;

    printf("Case #%d: ", qq);
    scanf("%s", s);
    n = strlen(s);
	/* Let's for example s = "|.|.|x|.|x|.|.|" 
	 * n = 7
	 * t = (1 << 7) - 1 = 1000 0000 - 1 = 1111 1111
	 * We use dynamic programming method to calculate the expected cost 
	 * From the start we have t = 1111 1111, which means that all positions are busy
	 * and our expected cost is 0.
	 * next - 1111 1110
	 * next - 1111 1101
	 * next - 1111 1100
	 * next - 1111 1011
	 * next - 1111 1010
	 * ......
	 */
    f[(1 << n)-1] = 0.0;
    for (t=(1 << n)-2;t>=0;t--) 
	{
      int cnt = 0; 
      f[t] = 0;
	  /* Let's suppose t = 1111 1110
	   * (1/N) * ∑i=0..N-1(E(mask | (1 << f(i)) ) + c(i))
	   * i = 0;
	   * j = 0;
	   * cur = n
	   * t & (1 << j) = 1111 1110
	   *             &  0000 0001 = 0
	   * 
	   * Let's take one value t = 1010 0010
	   * i = 0, j = 0, cur = 8
	   * 1 << 0 = 1 => 1010 0010 & 0000 0001 = 0
	   * t + (1 << 0) = 1010 0011
	   * 
	   */ 
      for (i=0;i<n;i++) 
	  {
        int j = i;
        int cur = n;
        while (t & (1 << j)) 
		{
          j++;
          if (j == n) j = 0;
          cur--;
        }
        f[t] += f[t + (1 << j)] + cur;
        cnt++;
      }
      f[t] /= cnt;
    }
    t = 0;
    for (i=0;i<n;i++)
      if (s[i] == 'X') t |= (1 << i);
    //printf("%.17lf\n", f[t]);
    //fflush(stdout);
  }
  return 0;
}


double p[210][210][210]; // len, cnt1, cnt2
double dp[210][210][210];
int N,K;
char* s;

double GCJ2013_3_4_func1(void);

void GCJ2013_3_4_pre(void)
{
	int i,j,k;	
	for (i = 1; i <= 205; i++)
	{
		for (j = 1; j <= 205; j++) 
			for (k = 1; k <= 205; k++) 
                p[i][j][k] = 0.0;

		p[i][0][0] = 1.0;

		/* p[1][0][0] = 1.0
		 *
		 * Iteration 1:
		 *    p111 = 1/i
		 *    p110 = (i-1)/i
		 * 
		 * Iteration 2:
		 *    p112 = (1/i)^2
		 *    p111 = (1/i)(i-1/i)
		 *  
		 * Iteration 3:
		 *    p113 = (1/i)^3
		 *    
		 *    
		 *    
		 */   
	    for (j = 1; j <= 205; j++) 
		    for (k = 1; k <= 205; k++) 
		    {
			    p[i][j+1][k+1] += p[i][j][k] * (1.0 / i);
			    p[i][j+1][k] += p[i][j][k] * ((i - 1.0) / i);
		    }
	}
}

void GCJ2013_3_4_main2(void)
{
	int i,j;
    double ans = 0.0;
    char* t;

	K = 0;
    for (i = 1; i <= N; i++) 
		if (s[i] == '.') K++;

	for (i = 1; i <= N; i++)
	{
		//t = t.substr(1) + t[0];
		s = t;
		ans += GCJ2013_3_4_func1();
	}

	//printf("%.12f\n", (double)N * (double)K - ans);
}

double GCJ2013_3_4_func1(void)
{
	int i,j,k;
	double ans;
	
	for (i = 1; i <= N + 1; i++)
       for (j = 1; j <= K + 1; j++)
          for (k = 1; k <= K - j + 1; k++)
		     dp[i][j][k] = 0.0;
	
	dp[0][0][K] = 1.0;

	for (i = 1; i <= N + 1; i++)
       for (j = 1; j <= K + 1; j++)
          for (k = 1; k <= K - j + 1; k++)
		    if (dp[i][j][k] > 1.0E-15)
		    {
		        int x;
		        int center = k / (N-i);

		        for (x=center;x>=0;x--)
				{
					int i2, j2, k2;
			        double prob = dp[i][j][k] * p[N-i][k][x];
			        if(prob < 1.0E-15) break;
			        i2 = i+1;
					j2 = max(j+x - ((s[i]=='.')?1:0), 0);
					k2 = k-x;
			        dp[i2][j2][k2] += prob;
		        }
		
		        for (x=center+1;x<=k;x++)
				{
					int i2, j2, k2;
			        double prob = dp[i][j][k] * p[N-i][k][x];
			        if(prob < 1.0E-15) break;
			        i2 = i+1;
					j2 = max(j+x - ((s[i]=='.')?1:0), 0);
					k2 = k-x;
			        dp[i2][j2][k2] += prob;
				}
		    }

	ans = 0.0;

    for (j = 1; j <= N + 1; j++)
       for (k = 1; k <= K - j + 1; k++)
          ans += j * dp[N][j][k];

	return ans;
}

int GCJ2013_3_4_large(void)
{
	int TC,tc;
	GCJ2013_3_4_pre();
	//cin >> TC;

	for (tc = 0; tc < TC; tc++)
	{
        //printf("Case #%d: ", tc+1);
		GCJ2013_3_4_main2();
	}

	return 0;
}










