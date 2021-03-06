/*

Problem

 You own a factory with two assembly lines. 
 The first assembly line makes boxes, 
 and the second assembly line makes toys to put in those boxes. 
 Each type of box goes with one type of toy and vice-versa. 

 At the beginning, you pick up a box from the first assembly line 
 and a toy from the second assembly line. You then have a few options. 

 You can always throw out the box and pick up the next one. 
 You can always throw out the toy and pick up the next one. 

 If the box and toy are the same type, 
 you can put the toy in the box, 
 and send it out to customers. 

 You always pick boxes up in the order in which they are made, 
 and similarly for toys. 
 
 You know the order in which boxes and toys are made, 
 and you want to plan out a strategy that will allow you 
 to send as many boxed toys as possible to customers. 


 Warning: 
 
 The two assembly lines make a lot of boxes and toys. 
 However, they tend to make one kind of thing for a long period of time before switching. 


 Input

 The first line of the input gives the number of test cases, T. 
 T test cases follow.

 Each test case begins with a line contains two integers N and M. 
 It is followed by a line containing 2 * N integers 
 a1, A1, a2, A2, ..., aN, AN, 
 and another line containing 2 * M integers 
 b1, B1, b2, B2, ..., bM, BM. 

 This means that the first assembly line will make 
      a1 boxes of type A1, 
 then a2 boxes of type A2, etc., 
 until it finishes with aN boxes of type AN. 
 
 Similarly, the second assembly will make 
 b1 toys of type B1, 
 followed by b2 toys of type B2, etc., 
 until it finishes with bM toys of type BM. 

 A toy can be matched with a box if and only if they have the same type number. 

 Output

 For each test case, output one line containing "Case #x: y", 
 where x is the case number (starting from 1), 
 and y is the largest number of boxed toys that you can send out to customers. 

 Limits

 1 ≤ T ≤ 100.
 1 ≤ ai, bi ≤ 1016.
 1 ≤ Ai, Bi ≤ 100.

 Small dataset

 1 ≤ N ≤ 3.
 1 ≤ M ≤ 100.

 Large dataset

 1 ≤ N, M ≤ 100.

 Sample
 Input 
  	
 Output 
  
 4
 3 3
 10 1 20 2 25 3
 10 2 30 3 20 1
 3 5
 10 1 6 2 10 1
 5 1 3 2 10 1 3 2 5 1
 3 5
 10 1 6 2 10 1
 5 1 6 2 10 1 6 2 5 1
 1 1
 5000000 10
 5000000 100




 This problem is a variation of the Longest Common Subsequence problem, 
 which is to find the longest string of characters 
 which occurs as a subsequence of each of two strings 
 (a string S is a subsequence of a string T if S occurs in T, 
  possibly with more characters inserted between the elements of S.) 
 In this case, the first "string" is the sequence of types 
 of each box produced by the factory, 
 and the second "string" is the sequence of types of each toy produced by the factory. 

 A dynamic programming algorithm for this problem is to find 
 the maximum number of toys 
 that could be placed in boxes using the first x boxes and the first y toys. 
 Let this value be f[x][y]. 
 Then f[x][y] is equal to the maximum of: 

   f[x-1][y] 
   f[x][y-1] 
   f[x-1][y-1] + 1 

 with the last case only applying if the 
 x-th letter of the first string 
 is equal to the y-th letter of the second string. 
 
 These three cases correspond to the last action taken 
 being to drop a box, 
 to drop a toy, 
 and to put a toy in a matching box, respectively. 

 Unfortunately, even though the number of runs of boxes and toys is small, 
 the total number of boxes and toys can be very large, 
 so this algorithm is not practical. 
 But we can modify it so that 
 f[x][y] will be the maximum number of toys 
 that could be placed in boxes using the first x runs of boxes 
 and the first y runs of toys. 
 
 Now f[x][y] is the maximum of: 

 f[x-1][y] 
 f[x][y-1] 
 f[a][b]+g(a,b,x,y), for all a<x, b<y 

 Similarly to before, the last case only applies if the type of box run x 
 matches the type of toy run y. 
 It corresponds to only using boxes of that type between 
 runs a+1 and x, 
 and toys of that type between runs b+1 and y. 
 
 g(a,b,x,y) is the minimum of the number of those toys and the number of those boxes, 
 which is the number of toys that can be placed in boxes in that range. 

 This algorithm is O(n4). 
 Another improvement can reduce this again to O(n3), 
 which we leave as an exercise to the reader!


*/

const int N = 105;

long dp[105][105];

struct Element 
{
    int type;
    long count;
};

struct Element a[105];
struct Element b[105];

int n, m;

void Pr17Solve() 
{
	int i, j;
	memset(dp,0,sizeof(dp));

    scanf("%d %d", &n, &m);
    for (i = 1; i <= n; ++i) 
	{
        scanf("%lld %d", &a[i].count, &a[i].type);
    }

    for (i = 1; i <= m; ++i) 
	{
        scanf("%lld %d", &b[i].count, &b[i].type);
    }

	for (i = 1; i <= n; ++i) 
	{
        for (j = 1; j <= m; ++j) 
		{
            if (a[i].type == b[j].type) 
			{
                long totalA = a[i].count;
                long totalB = b[j].count;
				long minTotal = (totalA < totalB) ? totalA : totalB;
				 
                int prevA = i - 1;
                int prevB = j - 1;

                while (1) 
				{
					int finished = 0;
                    int state = 0;

					dp[i][j] = dp[i][j] > dp[prevA][prevB] + minTotal 
						      ? dp[i][j] 
					          : dp[prevA][prevB] + minTotal;

				    state = (totalA < totalB) ? -1 : (totalA == totalB ? 0 : 1);
                    
					if (state <= 0) 
					{
                        while (prevA > 0 && a[prevA].type != a[i].type) 
                            --prevA;

                        if (prevA == 0) 
                            finished = 1;
						else 
						{
                            totalA += a[prevA].count;
                            --prevA;
                        }
                    }

					if (state >= 0) 
					{
                        while (prevB > 0 && b[prevB].type != b[j].type)
                            --prevB;

                        if (prevB == 0)
                            finished = 1;
                        else 
						{
                            totalB += b[prevB].count;
                            --prevB;
                        }
                    }

					if (finished) 
                        break;
				}
			}
			else
			{
				dp[i][j] = dp[i - 1][j] > dp[i][j - 1] 
				         ? dp[i - 1][j] 
						 : dp[i][j - 1];
			}
		}
	}
}