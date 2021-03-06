
/*



Problem


We're going to run a tournament with 2N teams, 
and give out P identical prizes to the teams with ranks 0..P-1.

The teams are numbered 0 through 2N-1. 
When team i and team j play against each other in a game, 
team i will win iff i<j.


The teams for a tournament are organized in some order, 
called the tournament's tournament list, 
which contains all 2N teams in the tournament. 

The tournament list will affect 
which teams play each other, and in what order.

Your job will be to find the largest-numbered team 
that is guaranteed to win a prize, 
independent of how the tournament list is ordered; 

and to find the largest-numbered team that could win a prize, 
depending on how the tournament list is ordered.


Tournament Resolution

The tournament is conducted in N rounds.

Each team has a record: 
the list of the results of the games it has played so far. 

For example, 
if a team has played three games, 
and won the first, 
lost the second and won the third, 
its record is [W, L, W]. 

If a team has played zero games, 
its record is [].

In each round, 
every team plays a game against a team with the same record. 
The first team in the tournament list with a particular record 
will play against the second team with that record; 

the third team with the same record 
will play against the fourth; and so on.

After N rounds, each team has a different record. 
The teams are ranked in reverse lexicographical order of their records; 
so [W, W, W] > [W, W, L] > [W, L, W] ... > [L, L, L].

Here is an example of a tournament with N=3, 
and the tournament list [2, 4, 5, 3, 6, 7, 1, 0],
where the columns represent different rounds,
and the teams are grouped by their records. 

The winner of each game in the example has been marked with a *.

[2, 4, 5, 3, 6, 7, 1, 0],

Round1 
We play pairs

[2, 4], [5, 3], [6, 7], [1, 0]

Winner is a person with a smaller index
[2*, 4], [5, 3*], [6*, 7], [1, 0*]
[2*, 3], [6, 0*], [4*, 5], [7, 1*]

Round 1    Round 2    Round 3    Final Result
                                 (best rank at top)
[]         [W]        [W,W]
2  *       2  *       2          0  [W,W,W]
4          3          0  *       2  [W,W,L]
                      [W,L]
5          6          3  *       3  [W,L,W]
3  *       0  *       6          6  [W,L,L]
           [L]        [L,W]
6  *       4  *       4          1  [L,W,W]
7          5          1  *       4  [L,W,L]
                      [L,L]
1          7          5  *       5  [L,L,W]
0  *       1  *       7          7  [L,L,L]

If we give out 4 prizes (N=3, P=4), 
the prizes will go to teams 0, 2, 3 and 6.

The largest-numbered team 
that was guaranteed to win a prize with N=3, P=4, 
independent of the order of the tournament list, 
was team 0: 
this tournament list demonstrated 
that it's possible for team 1 not to win a prize, 
and it turns out that team 0 will always win one, 
regardless of the order of the tournament list.

The largest-numbered team that could 
win a prize with N=3, P=4, 
depending on how the tournament list was ordered, was team 6: 
this tournament list demonstrated that 
it's possible for team 6 to win a prize, 
and it turns out that team 7 will never win one, 
regardless of the order of the tournament list.


Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case consists of two space-separated integers: 
N, which indicates the tournament has 2N teams, 
and P, the number of prizes.

Output

For each test case, 
output one line containing "Case #x: y z", 
where x is the case number (starting from 1), 
y is the largest-numbered team 
that is guaranteed to win a prize, 
independent of how the tournament list is ordered; 
and z is the largest-numbered team that could win a prize, 
depending on how the tournament list is ordered.



Solution

Let's begin by an observation that will make our life a bit easier: 
if we reverse all the team numbers 
(that is, team 0 becomes team 2N-1, team 1 becomes team 2N-2, etc.), 
without changing the tournament list. 

This will result in the final ranks of the teams also being reversed, 
since it's relatively easy to see that all the records will be reversed 
(wins becoming losses and vice versa). 

This means that the problem of having a team rank 
as low as possible to get into the first P 
is equivalent to the problem of a team ranked as high as possible 
to get into the bottom P 

(or, in other words, not get into the top 2N - P). 
Thus, if we are able to answer the question 
what is the lowest rank 
that can possibly get into the top P, 
we can run the same code to see what's the lowest rank 
that can possibly be in the top 2N - P, 
subtract one, and reverse, 
and this will be the lowest rank that will always get a prize. 

This means we only have to solve one problem 
— lowest ranked team guaranteed to be in the top P

For the small dataset we have at most 1024 teams, 
which means we can try to figure out the tournament tree 
that will give a given team the best position, do this for all teams, 
and see which team was the lowest-ranked one to get a prize. 

Let's skip this, however, and jump straight to the solution for the large dataset, 
where 250 teams clearly disallow any direct approaches.

The key observation we can make here is that 
if we have a team we want to be as high as possible, 
we can do it with a record that includes a string of wins 
followed by a string of losses, and nothing else. 

This sounds surprising, but is actually true. 
Imagine, to the contrary, that the team (let's call them A) has a loss 
(against some team B) followed by a win against C, 
who played D in the previous round. 

Note that up to the round where A plays B 
the records of the four teams were identical. 

Also note that all the tournament trees of the four teams so far are disjoint, 
and so we can swap them around. 
In particular, we can swap team C and all its tree with team B and all its tree. 

Since we swap whole trees, the records of teams don't change, 
so now team A will play C in the first match and win 
— and so its record is going to be strictly better than it was, no matter what happens next. 
Thus, any ordering in which team A has a loss followed by a win is suboptimal.

This allows us to solve the problem of getting 
the highest possible rank for a given team. 

We simply need to greedily win as much as we can. 
If we're the worst team, we can't win anything. 

If we're not, we can certainly win the first match. 
The second match will be played against the winner of some match, 
so in order to win it we need to be better than three teams. 

To win two matches, we need to be better than seven teams, and so on.

We can also reverse the reasoning 
to get the lowest-ranked team that can win the prize. 

First, let's ask how many matches does one need to win in order to win a prize. 
If you win no matches, you are in the top 2N (not a huge achievement!). 
If you win one, you are in the top 2N - 1. And so on. 

Once we know how many matches you need to win, 
you directly know how many teams you need to be better than. 
Simple python code follows:



Reasoning




def LowRankCanWin(N, P):
  matches_won = 0
  size_of_group = 2 ** N
  while size_of_group > P:
    matches_won += 1
    size_of_group /= 2
  return 2 ** N - 2 ** matches_won

def ManyPrizes(N, P):
  print 2 ** N - LowRankCanWin(N, 2 ** N - P) - 2, LowRankCanWin(N, P)



  [2, 4, 5, 3, 6, 7, 1, 0]

  Round 1    Round 2    Round 3    Final Result
                                 (best rank at top)
[]         [W]        [W,W]
2  *       2  *       2          0  [W,W,W]
4          3          0  *       2  [W,W,L]
                      [W,L]
5          6          3  *       3  [W,L,W]
3  *       0  *       6          6  [W,L,L]
           [L]        [L,W]
6  *       4  *       4          1  [L,W,W]
7          5          1  *       4  [L,W,L]
                      [L,L]
1          7          5  *       5  [L,L,W]
0  *       1  *       7          7  [L,L,L]

*/


/* Reasoning
 * 1. We play a match with some team. to win it we must be better than our opponent
 *    To win a tornament of 2 teams we must be better than 1 team. And our rank will be (1, 2) 
 *    where 2 is our opponent
 * 2. We have 4 teams: 
 *    To win a whole tournamnet we need to be better than 3 other teans - (1, 2, 3, 4)
 *    To be in top 1 we need to be better than 3 teams
 *    To be in top 2 we need to be better than 2 teams.
 *
 * General subtask:
 * We have 2N teams. What number of teams should we be better than to be in top P?
 * 
 * Solution:
 * 
 * 1. Form the start suppose we are better than 0 teams, 
 *    or we are the worst team so that other 2N - 1 teams will be higher than we
 *    We will be in the top P if P >= 2N
 *  
 * 2. If we are better than 1 team, 2N - 2 teams will be higher than we
 *    We will be in top P if P >= 2N - 1
 * 
 * 3. 
 * But there can be a situation when P = 4, and p1 is better than p2, p3, p4....
 * In the first rank he plays with p0 and losses and goes to the lowest part of the 
 * tournament list.
 * Change our algorithm:
 *
 * 1. From the start we have team po and he is absolute winner 
 *    because he is better than all other players.
 * 2. Try to find out if p1 player will win a prize nevertheless for start timetable
 *    If he loose to player p0, he will be at N position and will wil prise only if P > N
 * 3. p0 plays with p1 and p1 goes to the lowest part of tournament list
 *    p0 plays with p2 and p2 goes to the lowest part of tournament list
 *    Than p1 plays with p2 and p2 goes to the [2n-n/2, 2n] part and he wins the price 
 *    if P > 2n-n/2
 */


void LowRankCanWin(int N, int P)
{
  int matches_won = 0;
  int size_of_group = 2 ^ N;

  while (size_of_group > P)
  {
      matches_won += 1;
      size_of_group /= 2;
  }

  return 2 ^ N - 2 ^ matches_won;
}



void round22Solve() 
{  
    long long n, p, ans, total, i;
	n = 4;
	p = 4;
    total = 0;
    ans = 0;

	/* i = 4 - 1 = 3, total = 6
	 * 
	 */
    for (i = n - 1; i >= 0; i--) 
	{
      total += 2 ^ i;
      if (p <= total) break;
      ans += (i == 0 ? 1LL : 2 ^ (n-i));
    }

    // cout << ans << " ";

    for (i=n;i>=0;i--)
      if (p >= (1LL << i)) {
        ans = (1LL << n) - (1LL << (n-i));
        break;
      }
    //cout << ans << endl;
}
