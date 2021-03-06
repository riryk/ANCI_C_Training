/*
 
 Problem

 Ryan is playing Kingdom Rush, 
 a single-player tower defense game developed by Ironhide Game Studio. 
 
 In Kingdom Rush, players earn stars by completing levels, 
 in a way described below. 
 
 Having more stars makes the player more powerful; 
 so while Ryan might not be able to complete level 2 right away, 
 he might be able to complete it after earning stars from level 1.

 The real game Kingdom Rush doesn't work in quite the same way as this problem. 
 It isn't important to have played the game in order to solve the problem.

 In this problem's version of Kingdom Rush, 
 when a player completes a level, 
 he or she is given a 1-star rating or a 2-star rating. 
 
 That rating might allow the player to earn stars as follows:

 If the player has never completed the level before 
 and completes it with a 1-star rating, that player earns 1 star.

 If the player has never completed the level before 
 and completes it with a 2-star rating, that player earns 2 stars.

 If the player has only completed the level before with a 1-star rating 
 and completes it this time with a 2-star rating, the player earns 1 more star.

 Otherwise there is no way for a player to earn stars.

 Ryan might not be able to complete every level right away. 

 For each level, before he can complete it with a 1-star rating, 
 he needs to have earned a certain number of stars; 
 
 and he will need a larger or equal number of stars 
 to complete that level with a 2-star rating.


 For example, suppose there are two levels:


 Level 1 

 requires 0 stars to complete with a 1-star rating, 
      and 1 star to complete with a 2-star rating.

 Level 2 
 
 requires 0 stars to complete with a 1-star rating, 
      and 2 stars to complete with a 2-star rating.


 Here's a possible series of events for Ryan:

 Ryan starts with 0 stars. 

 He can choose to complete either level 1 or level 2 with a 1-star rating. 
 He chooses to complete level 1 with a 1-star rating. Now he has 1 star.

 Now Ryan can either complete level 2 with a 1-star rating, or level 1 with a 2-star rating. 
 He chooses to complete level 1 with a 2-star rating. Now he has 2 stars.

 Now Ryan can complete level 2 with a 2-star rating. 
 He does that, and now he has 4 stars.

 Now he is done, having completed all levels with 2-star ratings and earned 4 stars (2 per level). 
 
 He has completed levels 3 times: level 1 twice, and level 2 once.

 Ryan is great at tower defense games, 
 but he needs some help to beat Kingdom Rush as quickly as possible. 

 Your job is to figure out how many times he needs to complete levels 
 in order to earn a 2-star rating on every level.


 Input


 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case starts with a line containing a single integer N, 
 indicating how many levels are in the game. 
 N lines follow. 
 The ith line contains two integers ai and bi: 
 the number of stars it takes to earn a one-star rating or a two-star rating, respectively, on level i.


 Output

 For each test case, output one line containing 
 "Case #x: y", where x is the case number (starting from 1) 
 and y is the minimum number of times Ryan must complete levels 
 in order to have earned a 2-star rating on every level. 
 
 If it is impossible for Ryan to earn a 2-star rating on every level, 
 y should instead be the string "Too Bad" 
 (without the " characters, but with that exact capitalization). 
 This indicates that Ryan is too bad at Kingdom Rush to finish the whole game.


 Limits

 1 ≤ T ≤ 100.
 0 ≤ ai ≤ bi ≤ 2001.

 Small dataset

 1 ≤ N ≤ 10.

 Large dataset

 1 ≤ N ≤ 1000.

 Sample


Input 
 	
Output 
 
4
2
0 1
0 2
3
2 2
0 0
4 4
1
1 1
5
0 5
0 1
1 1
4 7
5 6	 Case #1: 3
Case #2: 3
Case #3: Too Bad
Case #4: 6

Kingdom Rush was created by Ironhide Game Studio. 
Ironhide Game Studio does not endorse and has no involvement with Google Code Jam.



This problem was an interesting one to create. 
In the actual game Kingdom Rush, 
there are three stars per level, 
"challenge" levels, and you can't try level 2 
until you've beaten level 1 with at least one star. 

Coming up with a problem that was solvable, 
while maintaining the same feeling as the game that inspired it, 
was a balancing act.

We solved this problem with a greedy algorithm. 

At every step of the algorithm, 
Ryan will make a decision about which level to play, 
and his decision will be based simply on the properties of the levels available, 
and what he's done so far.

First, let's observe that Ryan should only complete a level 
if he's never completed it before, 
or if he can go from a one-star rating to a two-star rating. 

There's simply no point in beating a level otherwise. 
When we're talking about levels below, we'll ignore levels that he shouldn't complete for this reason.

Second, if Ryan ever reaches a state 
where he can't complete any of the remaining levels, 
then he is "TOO BAD" to beat the game. 

This will happen independent of the order in which he completes the levels.

Third, if Ryan can complete a level with a two-star rating, he should do it immediately. 

There's no reason for him to wait: 
he can earn those two stars (or one star) with one level completion either now or later. 

If there are multiple levels with two-star ratings that Ryan could complete, 
he should choose one arbitrarily; 
he can do the other one next.

Now we've covered all situations except for one: 
when the only levels Ryan can complete are levels 
that he can complete with a one-star rating. 

Consider two levels like that, level 0 and level 1:

a0 b0
a1 b1

The values of a0 and a1 don't matter: 

by assumption, Ryan has at least that many stars already. 
Let's assume without loss of generality that b0 < b1. 
Which level should Ryan complete first?

Let's remember that Ryan's objective is to complete levels the minimum number of times. 
In the worst case, it will take Ryan 4 completions to finish those two levels: 

two to get him a one-star rating in both levels, 
and two more to get him a two-star rating in both levels. 

But earning stars from these levels (or other levels) 
might allow him to complete one of them with a two-star rating 
without having to complete it with a one-star rating first.

Here's a possible series of events. 
Assume Ryan starts with S stars. 
We'll decide later whether k is 0 or 1:

Ryan completes level k with a 1-star rating and earns 1 star.

Ryan completes other levels and earns s stars.

Ryan completes level 1-k with a 2-star rating.

Which choice of k makes this scenario possible? 

If k=0, then this is possible iff S + 1 + s ≥ b1. 
If k=1, then this is possible iff S + 1 + s ≥ b0. 

Since b0 < b1, then this is possible with k=0 only if it's possible with k=1. 
So we might as well simply choose k=1, and have Ryan choose the level with the highest value of b.
So to summarize, Ryan's strategy should be:

While there is any level remaining that Ryan hasn't completed yet, 
or any level for which he can earn a higher rating than he previously had:

If he can earn a two-star rating on any of those levels, 
he should complete one of those levels (chosen arbitrarily).

Otherwise, if there is some set of levels on which 
he can earn a one-star rating, 
he should complete the one of those levels with the highest value of b.

If Ryan has beaten all levels with a two-star rating, he's done. 
Otherwise he's TOO BAD.

By simulating this strategy, 
we can see whether Ryan can beat Kingdom Rush, and the smallest number of level completions he can do it in.


 */

int ai[1001];
int bi[1001];
int done[1001];

void Pr22Solve()
{
	int T = 10;
	int N;
	int prob = 1;
	int t, i, stars, ret;

    for (t = 0; t < T; t++)
	{
        N = 12;
        for (i = 0; i < N; i++)
			; // cin >> ai[i] >> bi[i]; read ai, bi
		memset(done, 0, sizeof(done));
		stars = 0;
		ret = 0;

		while (stars < 2*N) 
		{
           int best1 = -1;
		   /* From the start we have not beaten any level yet
		    * We scan all availiable levels and choose those, which we can beat 
			* without necessity to beat this level with one-star rating.
			* First of all we beat all 2 star levels without beating it with one
			* star rating.
			*
		    */
           for (i = 0; i < N; i++) 
		   {
			   /* stars >= bi[i] means that we have enough stars to beat 2 stars level
			    * done[i] < 2    means that we haven't beaten this level 
				*                or have beaten it only with 1 star.
			    */
			   if (done[i] < 2 && bi[i] <= stars)  // stars >= bi[i] 
			   {
                   ret++;
                   stars += 2 - done[i];
                   done[i] = 2;
                   goto played;
               }
		   }

		   /* First time when we come here, there are no levels, which we 
		    * can beat without beating them with rating 1.
			* We need to beat a level with the largest bi
			*
			* Royan has S stars. We have 2 levels 0 and 1. b1 > b0
			* We choose k = 0 level to beat
			* We beat it with rating 1 and earn 1 star: S + 1
			* having stars S + 1 we can beat other levels and earn another s stars
			* We have S + s + 1 stars. We can beat the level 1 only given 
			* S + s + 1 > b1. 
			*/
           for (i = 0; i < N; i++) 
		   {
			   if (done[i] < 1 && ai[i] <= stars) 
			   {
                   if (best1 == -1 || bi[i] > bi[best1])
                       best1 = i;
               }
		   }

		   if (best1 == -1) 
		   {
               //cout << "Case #" << prob++ << ": Too Bad" << endl;
               goto done;
           }
           ret++;
           stars += 1-done[best1];
           done[best1] = 1;
played:;
		}
		 //cout << "Case #" << prob++ << ": " << ret << endl;
done:;
	} 
}


