
/*
  
  Problem

  Given a list X, consisting of the numbers (1, 2, ..., N), 
  an increasing subsequence is a subset of these numbers 
  which appears in increasing order, 
  and a decreasing subsequence is a subset of those numbers which appears in decreasing order. 
  
  For example, (5, 7, 8) is an increasing subsequence of (4, 5, 3, 7, 6, 2, 8, 1).

  Nearly 80 years ago, two mathematicians, 
  Paul Erdős and George Szekeres proved a famous result: 

  X is guaranteed to have 
  either an increasing subsequence of length at least sqrt(N) 
         or a decreasing subsequence of length of at least sqrt(N). 
		 
  For example, (4, 5, 3, 7, 6, 2, 8, 1) has a decreasing subsequence of length 4: (5, 3, 2, 1).

  I am teaching a combinatorics class, 
  and I want to "prove" this theorem to my class by example. 
  
  For every number X[i] in the sequence, I will calculate two values:

  A[i]: The length of the longest increasing subsequence of X 
        that includes X[i] as its largest number.

  B[i]: The length of the longest decreasing subsequence of X 
        that includes X[i] as its largest number.

  The key part of my proof will be that the pair (A[i], B[i]) is different for every i, 
  and this implies that either A[i] or B[i] must be at least sqrt(N) for some i. 
  For the sequence listed above, here are all the values of A[i] and B[i]:

  i  |  X[i]  |  A[i]  |  B[i] 
-----+--------+--------+--------
  0  |   4    |   1    |   4
  1  |   5    |   2    |   4
  2  |   3    |   1    |   3
  3  |   7    |   3    |   4
  4  |   6    |   3    |   3
  5  |   2    |   1    |   2
  6  |   8    |   4    |   2
  7  |   1    |   1    |   1
  

  I came up with a really interesting sequence to demonstrate this fact with, 
  and I calculated A[i] and B[i] for every i, 
  but then I forgot what my original sequence was. 
  Given A[i] and B[i], can you help me reconstruct X?

  X should consist of the numbers (1, 2, ..., N) in some order, 
  and if there are multiple sequences possible, 
  you should choose the one that is lexicographically smallest. 
  This means that X[0] should be as small as possible, 
  and if there are still multiple solutions, 
  then X[1] should be as small as possible, and so on.

  Input

  The first line of the input gives the number of test cases, T. 
  T test cases follow, each consisting of three lines.
  The first line of each test case contains a single integer N. 
  The second line contains N positive integers separated by spaces, 
  representing A[0], A[1], ..., A[N-1]. 
  The third line also contains N positive integers separated by spaces, 
  representing B[0], B[1], ..., B[N-1].

  Output

  For each test case, output one line containing "Case #x: ", 
  followed by X[0], X[1], ... X[N-1] in order, 
  and separated by spaces.

  Limits

  1 ≤ T ≤ 30.
  It is guaranteed that there is at least one possible solution for X.

  Small dataset

  1 ≤ N ≤ 20.

  Large dataset

  1 ≤ N ≤ 2000.





  Solution:


  Get the information

  The real trick to this problem is squeezing as much information 
  as possible from the sequences A[i] and B[i]. 
  We will get information out in the form of inequalities 
  between various elements of the sequence X.

  First notice that if we have two indices i < j, and A[i] ≥ A[j], then X[i] > X[j]. 
  Indeed, if it were otherwise, 
  then the increasing sequence of length A[i] with X[i] as its largest element 
  could be extended by adding X[j] at the end 
  to obtain an increasing sequence with X[j] at the end, 
  and we would have A[j] ≥ A[i] + 1. 
  
  This allows us to add some inequalities X has to satisfy. 
  We can add symmetric ones for B — if i < j and B[i] ≤ B[j], then X[i] < X[j].

  These inequalities are not enough, however, to reconstruct X. 
  Indeed, if we take A[i] = i + 1 and B[i] = N - i, 
  we get no inequalities to consider, 
  but at the same time not all permutations X are going to work. 
  
  The problem is that while we know what we need to do with X 
  so that no longer subsequences exist, 
  we still need to guarantee that long enough sequences exist.

  This is relatively simple to accomplish, though. 

  If A[i] > 1, then the increasing subsequence ending at X[i] 
  is formed by the extension of some sequence of length A[i] - 1. 
  
  This means that X[i] has to be larger than X[j] 
  for some j < i with A[j] = A[i] - 1. 
  
  The previous set of inequalities guarantees 
  that of the set of all such j 
  (that is, j which are smaller than i and have A[j] = A[i] - 1) 
  
  the one with the smallest X[j] is the largest j. 
  
  Thus, it is enough to find the largest j 
  with j < i and A[j] = A[i]-1 and add X[j] < X[i] 
  to our set of inequalities. 
  
  We again do the symmetric thing for B.



  Use the information


  Notice that the inequalities we have are indeed enough to guarantee 
  that any sequence X satisfying them will lead to the A and B we want. 
  
  It's relatively easy to check the first set of inequalities guarantees 
  the numbers A and B will not be larger than we want, 
  while the second set of inequalities guarantee they will be large enough.

  We now reduced the problem to finding the lexicographically 
  smaller permutation satisfying a given set of inequalities. 
  
  To find the lexicographically smallest result 
  we are foremost interested in minimizing the first value in X. 

  To this end, we will simply traverse all the inequalities 
  that the first value in X has to satisfy 
  (that is, iterate over all the elements we know to be smaller, 
   then all elements we know to be smaller than these elements, etc.). 
   
  We can do this, e.g., through a DFS. 
  After learning how many elements of X have to be smaller than X[1], 
  we can assign the smallest possible value (this number + 1) to X[1]. 

  Now we need to assign numbers smaller than X[1] to all these elements, 
  in the lexicographically smallest way.

  Note that how we do this assignment will not affect the other elements 
  (since they are all going to be larger than X[1], 
   and so also larger than everything we assign right now). 
   
  Thus, we can assign this set of elements so that it is lexicographically smallest. 
  This means taking the earliest of these elements, 
  and repeating the same procedure recursively 
  (find all elements smaller than it, assign the appropriate value, recurse). 
  Note that once some values have been assigned, 
  we need to take that into account when assigning new values 

  (so, if we already assigned 1, 3 and 10; 
   and now we know that an element we're looking at is larger than 4 others, 
   the smallest value we can assign to it is 7).

  Such a recursive procedure will allow us to solve the problem. 
  For each element, we are going to traverse the graph of inequalities to find 
  all the elements smaller than it is 
  (O(M time if we do a DFS, where M is the number of inequalities we have), 
   then see what's the smallest value we can assign 
   (O(N) with a linear search, we can also go down to O(logN), but don't need to), and recurse. 
   
  This gives us, pessimistically, O(N3) time 
  — easily good enough for the small testcase, but risky for the large.



  Compress the information


  Following the exact procedure above we can end up with O(N2) inequalities. 
  This will be too much for us (at least for the large dataset), 
  so we will compress a bit.

  The problem is with inequalities of the first type — 
  for indices smaller than a given i there can be many with A larger or equal to A[i]. 
  
  A trick we can use, though, is to take only one inequality 
  — find the largest j < i with A[j] = A[i], 
  and insert only the inequality X[j] &gt X[i].

  Any other k < j with A[k] = A[i] will follow from transitivity 
  — there will be a sequence of indices with A equal to A[i] connecting k to i. 
  
  Any k with A[k] > A[i] will also follow, 
  since X[k] will have to be greater than some X[l] for A[l] = A[i] and l < k (and thus < i). 

  This means we can reduce our set of inequalities down to a set of O(N), 
  which means that each DFS traversal step will take only O(N) time, 
  and the solution to the whole problem will run in O(N2).

  Interesting fact: It is also possible to solve this problem in O(N logN). 
  Can you figure it out?


 */

 /*

  i  |  X[i]  |  A[i]  |  B[i] 
-----+--------+--------+--------
  0  |   4    |   1    |   4
  1  |   5    |   2    |   4
  2  |   3    |   1    |   3
  3  |   7    |   3    |   4
  4  |   6    |   3    |   3
  5  |   2    |   1    |   2
  6  |   8    |   4    |   2
  7  |   1    |   1    |   1

  A[i]: The length of the longest increasing subsequence of X 
        that includes X[i] as its largest number.

  */ 

 /*
    1. if (i < j and A[i] >= A[j]) => X[i] > X[j]


       Provement: Suppose opposite. X[i] <= X[j] 
       If we look at the largest subsequence which ends up in j,
                 and the largest subsequence which ends up in i,
	   we will notice that they are connected. We can build a new 
	   subsequence for j item by adding j item to the subsequence for i
	   largest subsequence (i)   = {........,X[i]}
	           subsequence {j+1} = {........,X[i],X[j]}
	   That means we have found a sequence for j item with the length
	   more than the length of the longest subsequence for ith item.
	   which contradicts: A[i] >= A[j]
 
                A[i]   A[j]
	      .....  i .... j ..... 
                X[i]   X[j]
        
	   Consider a situation when i < j. A[i] >= A[j]
       A[i] is the length of longest increasing subsequence which ends up in X[i]
	   A[j] is the length of longest increasing subsequence which ends up in X[j]
       Suppose that X[i] <= X[j].
       If the sequence of length A[i] looks like: ......X[i].
       This sequence can be extended to: .......X[i],X[j].
	   So that A[j] > A[i] and we have received a contradiction.



    2. The same theorem we have for decreasing subsequences.
	   if (i < j and B[i] <= B[j]) => X[i] < X[j]


    3. Contr example: 
	   if (A[i] = i + 1 and B[i] = N - i)
	   if i < j than A[i] <= A[j] always, we will not get any inequality
                than B[i] >= B[j] so we will not get any inequality

    4. We need to describe an algorithm for building A[i] and B[i]
	   i = 0: {X[0]} is a subsequence A, A[0] = 1
	   i = 1: if X[0] < X[1], A1 = { X[0], X[1] }
	   If we have A[i] > 0, than its subsequence looks like:
	   {X[i0], X[i1], X[i2], ..., X[j],X[i]}
	   So X[i] has to be larger than some j with A[j] = A[i] - 1

	   Let's take an index i. A[i], X[i]
               A[j]       A[i]
	   ....... X[j] ..... X[i] .....
       
	   A[j] = A[i] - 1. And j < i. X[j] < X[i]

	   This means that X[i] has to be larger than X[j] 
	   for some j < i with A[j] = A[i] - 1.

       Suppose that we have j: j1, j2, ..., jk. 
       and j1 <= j2 <= j3 <= ... <= jk-1 <= jk.
       A[j1] = A[j2] = A[j3] = ... = A[jk-1] = A[jk] = A[i] - 1
       X[i] has to be larger than X[j].
	   So that:

	   X[i] > X[j1], X[i] > X[j2], X[i] > X[j3], ... , X[i] > X[jk].

	   by adding the first inequalities we have:
       X[j1] > X[j2] > X[j3] > ... > X[jk-1] > X[jk]
       
	   The smallest X[jk] is matched to the largest jk.
	   So at least X[i] has to be larger than the X[j] where 
	   j is the largest index when A[j] = A[i] - 1
    5. 


	Compress the information

	The problem is with inequalities of the first type — 
    for indices smaller than a given i there can be many with A larger or equal to A[i]. 
    
	if (i < j and A[i] >= A[j]) => X[i] > X[j]
	 f (i < j and B[i] <= B[j]) => X[i] < X[j]

	Let's take i and consider all j: j < i => A[j] >= A[i] and X[j] > X[i]

    Divide the whole set of js into two groups:

    {i: A[j] >= A[i]} = {i: A[j] = A[i]} U { i: A[j] > A[i] }

    1.  {j: A[j] = A[i]}
    
	    j1 <  j2   <   j3  <  ... <  jk-1   <  jk
	  A[j1] = A[j2] = A[j3] = ...  = A[jk-1] = A[jk].
    
      X[j1] > X[j2] > X[j3] > ... > X[jk-1] > X[jk].  > X[i]
      So that for all that js we take the largest j and X[j] > X[i]

    2.  {j: A[j] > A[i]}
      A[j] > A[l], l < j and A[l] = A[j] - 1.
	  So that there exists l < j < i, A[j] > A[l], A[l] = A[i].
      
	  

	 A trick we can use, though, is to take only one inequality 

     — find the largest j < i with A[j] = A[i], 
       and insert only the inequality X[j] &gt X[i].

       Any other k < j with A[k] = A[i] will follow from transitivity 
     — there will be a sequence of indices with A equal to A[i] connecting k to i. 

	   Any k with A[k] > A[i] will also follow, 
       since X[k] will have to be greater than some X[l] 
	   for A[l] = A[i] and l < k (and thus < i). 


	   So that our set of inequalities:

	   1. For each i = 1..n   
	        jx = max {j: j < i}  
			X[jx] > X[i]

	   2. For each i = 1..n
	        jy = min {j: j > i}
            X[jy] < X[i]

	   3. For each i = 1..n
	        jz = max {j: j < i and A[j] = A[i] - 1}
            X[jz] > X[i] 

	   4. For each i = 1..n
	        jk = min {j: j > i and A[j] = A[i] + 1}
            X[jk] < X[i]


  The next task is to find the lexographically smaller items.
  First of all we need to minimize the first item X[0].
  
    X[i1] < X[0], 
	X[i2] < X[0]

	Then

	X[i3] < X[i1],
    X[i4] < X[i1],
    X[i5] < X[i2],
	X[i6] < X[i2]

                    X[5]---  
	              /    \    \
	             X[1]  X[4]  X[6]
               /            /    \ 
		   --X[0]--        X[7]  X[8] 
		  /         \
	    X[i1]       X[i2]
      /     \      /    \
    X[i3]  X[i4] X[i5]  X[i6]
                          \
                          X[i7]


  We now reduced the problem to finding the lexicographically 
  smaller permutation satisfying a given set of inequalities. 
  
  To find the lexicographically smallest result 
  we are foremost interested in minimizing the first value in X. 

  To this end, we will simply traverse all the inequalities 
  that the first value in X has to satisfy 
  (that is, iterate over all the elements we know to be smaller, 
   then all elements we know to be smaller than these elements, etc.). 

  We can do this, e.g., through a DFS. 
  After learning how many elements of X have to be smaller than X[1], 
  we can assign the smallest possible value (this number + 1) to X[1]. 

  Now we need to assign numbers smaller than X[1] to all these elements, 
  in the lexicographically smallest way.

  Note that how we do this assignment will not affect the other elements 
  (since they are all going to be larger than X[1], 
   and so also larger than everything we assign right now). 
  
  Thus, we can assign this set of elements so that it is lexicographically smallest. 
  This means taking the earliest of these elements, 
  and repeating the same procedure recursively 
  (find all elements smaller than it, assign the appropriate value, recurse). 
  Note that once some values have been assigned, 
  we need to take that into account when assigning new values 

  (so, if we already assigned 1, 3 and 10; 
   and now we know that an element we're looking at is larger than 4 others, 
   the smallest value we can assign to it is 7).

  Such a recursive procedure will allow us to solve the problem. 
  For each element, we are going to traverse the graph of inequalities to find 
  all the elements smaller than it is 
  (O(M time if we do a DFS, where M is the number of inequalities we have), 
   then see what's the smallest value we can assign 
   (O(N) with a linear search, we can also go down to O(logN), but don't need to), and recurse. 
   
  This gives us, pessimistically, O(N3) time 
  — easily good enough for the small testcase, but risky for the large.


    
  */ 


/*#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <numeric>
#include <string>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <iterator>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cassert>

using namespace std;

#define REP(i,n) for (int i=0,_n=(n); i < _n; i++)
#define REPD(i,n) for (int i=(n)-1; i >= 0; i--)
#define FOR(i,a,b) for (int _b=(b), i=(a); i <= _b; i++)
#define FORD(i,a,b) for(int i=(a),_b=(b);i>=_b;i--)
#define ALL(c) (c).begin(), (c).end()
#define SORT(c) sort(ALL(c))

#define CLEAR(x) memset(x,0,sizeof x);
#define CLEARA(x) memset(&x,0,sizeof x);
#define FILL(x,v) memset(x,v,sizeof x);
#define FILLA(x,v) memset(&x,v,sizeof x);

#define VAR(a,b) __typeof(b) a=(b)
#define FOREACH(it,c) for(VAR(it,(c).begin());it!=(c).end();++it)

#define REVERSE(c) reverse(ALL(c))
#define UNIQUE(c) SORT(c),(c).resize(unique(ALL(c))-(c).begin())
#define INF 0x7fffffff
#define X first
#define Y second
#define pb push_back
#define SZ(c) (int)(c).size()
#define MP make_pair
#define eps 1.0e-11
const double pi = acos(-1.0);

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long LL;

#define FN "C-large"

#define N 2048
int n;
VI a[N];
int bb[N],cc[N];

void edge(int x, int y, bool sw)
{
	if (sw) edge(n-1-x,n-1-y,false);
	else a[y].pb(x);
}

void add(int v[], bool sw)
{
	int last[N];
	FILL(last,-1);
	REP(i,n)
	{
		if (last[v[i]] != -1)
			edge(i,last[v[i]],sw);
		if (last[v[i]-1] != -1)
			edge(last[v[i]-1],i,sw);
		last[v[i]]=i;
	}
}

bool mark[N];
int res[N];

int main()
{
	freopen(FN ".in","r",stdin);
	freopen(FN ".out","w",stdout);

	int tests;
	cin >> tests;
	for (int test = 1; test<=tests; test++)
	{
		fprintf(stderr,"=== %d\n",test);
		printf("Case #%d:",test);
		scanf("%d",&n);
		REP(i,n) a[i].clear();
		REP(i,n) scanf("%d",bb+i);
		REP(i,n) scanf("%d",cc+n-1-i);
		add(bb,false);
		add(cc,true);
		REP(i,n) UNIQUE(a[i]);
		CLEAR(mark);



		REP(step,n)
		{
			int sel = -1;
			REP(i,n) if (!mark[i])
			{
				bool ok = true;
				REP(j,SZ(a[i]))
					if (!mark[a[i][j]])
					{
						ok=false;
						break;
					}
				if (ok) 
				{
					sel = i;
					break;
				}
			}
			if (sel == -1) { fprintf(stderr,"SHIT!\n"); continue; }
			mark[sel]=true;
			res[sel]=step;
		}
		REP(i,n)
			printf(" %d",res[i]+1);
		printf("\n");
	}
	return 0;
}*/