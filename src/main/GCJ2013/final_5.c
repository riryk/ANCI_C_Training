
/*
 
 Problem

 The story goes...

 A long, long time ago, King Tyrone the Fair had 4 ministers. 
 The first minister (the king's top adviser) 
 was paid 7 gold pieces per week. 
 The second minister was paid 4 gold pieces per week. 
 The third and fourth ministers were each paid 6 gold pieces per week. 
 
 Unfortunately, Tyrone accidentally forgot the Ministerial Compensation List 
 in the photo copier one day, 
 and the List ended up on the front page of the Kingdom Times newspaper. 
 At this point, the second minister requested to speak to the king, 
 upset that his own salary was lower than that of the lower ranked third minister.

 His Fairness King Tyrone saw no other solution 
 than to fire the third minister. 
 After all, lowering the third minister's salary, 
 raising the salary of the second minister, 
 or changing job titles were all unfair solutions to the problem, 
 in the king's opinion. 
 
 And who are we to question King Tyrone? 
 
 Of course, firing the third minister did not solve the problem. 
 The second minister continued to complain 
 because his salary was still lower than that of the fourth minister. 
 So King Tyrone fired the fourth minister as well. 
 
 At this point, neither of the two remaining ministers complained, 
 and everyone lived happily ever after.

 ...wait a minute. 
 
 I messed that up. I'm sorry. 
 My memory is not what it used to be. 
 One moment please... Right. 
 King Tyrone the Fair. Four ministers. 
 Paid 7, 4, 6, and 6 respectively. Ah, yes. 
 The ending went like this...

 When the second minister complained of unfairness, 
 King Tyrone fired the first minister. 
 Some might say this was a bit harsh, 
 as the first minister wasn't involved in any way, 
 but we shouldn't question King Tyrone. 
 Obviously, the second minister still complained, 
 so King Tyrone simply fired him. 
 
 Of the remaining two ministers, 
 each one was being paid at least as much as any minister below him, 
 so none of them complained. 
 And everyone lived happily ever after.


 Much better... I think. Maybe? 
 Now I'm not sure anymore. 
 I know for certain that there were N ministers, 
 and I clearly remember their salaries. 
 I also know that every time a minister's salary was lower 
 than the salary of a minister below him, 
 somebody would complain, 
 and some minister got fired; 
 but that it could have been any minister, 
 regardless of whether that minister had anything at all to do with the problem. 
 Ministers continued to be fired until no one complained 
 because all of the salaries were non-increasing. 
 At that point, the firings stopped. 
 But I do not remember in which order the ministers got fired.

 Can you help me fix my story? 
 Or at least please tell me how many different stories I could have told. 
 Two stories are different if the sequences of fired ministers in them are not the same.


 Input


 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each one consists of two lines. 
 The first line will contain an integer N, 
 and the second line will contain N space-separated integers 
 denoting the ministers' salaries, 
 in order from the first minister to the N'th minister.


 Output

 For each test case, output one line containing "Case #x: y", 
 where x is the case number (starting from 1) and 
 y is the number of stories I could tell you, modulo 10007.


 Limits

 Each salary will be positive and at most 10000.

 Small dataset

  1 ≤ T ≤ 100.
  1 ≤ N ≤ 100.

 Large dataset

  1 ≤ T ≤ 20.
 For 80% of test cases, 1 ≤ N ≤ 2000.
 For all test cases, 1 ≤ N ≤ 8000.

Sample


Input 
 	
Output 
 
3
4
7 4 6 6
8
90 80 70 60 50 50 40 30
2
7 8





Dissecting the problem


The problem statement asks for the number 
of ways to remove elements from a sequence 
until we obtain a non-increasing sequence. 

Looking at the problem from the end, 
suppose we know the final non-increasing sequence, 
and it's of length K. 

Certain elements have been eliminated, 
and we removed N-K elements in total. 
There are (N-K)! 
(factorial of N-K) ways to remove those elements. 

So the first approximation seems to be to sum 
those factorials over all non-increasing 
subsequences of the the original sequence.


However, this is not entirely correct. 
Some non-increasing subsequences are not even reachable 
since we stop as soon as our sequence becomes non-increasing. 

For example, in the second example 
90 80 70 60 50 50 40 30

from the problem statement 
the sequence is non-increasing from the start, 
so no proper subsequence is reachable at all. 

For subsequences that are reachable, 
not every way of reaching them might be possible. 

For example, in the first example 

7 4 6 6

from the problem statement 
we can reach the '7 <first 6>' subsequence 
(note, it should be considered different from '7 <second 6>' subsequence) 
by eliminating the second 6, and then 4. 

However, it can't be reached by doing those operations in reverse order, 
since we'd stop right after eliminating 4.

Now instead of all (N-K)! ways to reach a certain subsequence, 
we need to count just the possible ways. 

The main trick in solving this problem is: 
let's count the impossible ways instead, 
and then subtract. 

The impossible ways are simply those 
when before the last removal, 
the subsequence was already a non-increasing sequence of length K+1. 

And for each such subsequence there 
are exactly K+1 ways to make an impossible removal 
and arrive at a subsequence of length K. 

That means the sum of numbers of impossible ways 
to reach all non-increasing subsequences of length K 
is equal to the total number 
of non-increasing subsequences of length K+1 times (N-K-1)! 
(the number of ways to reach the longer subsequence) times K+1.




Dissecting the problem


The problem statement asks for the number of ways 
to remove elements from a sequence until 
we obtain a non-increasing sequence. 

Looking at the problem from the end, 
suppose we know the final non-increasing sequence, 
and it's of length K. 

Certain elements have been eliminated, 
and we removed N-K elements in total. 

There are (N-K)! 
(factorial of N-K) 
ways to remove those elements. 

So the first approximation seems to be 
to sum those factorials over 
all non-increasing subsequences of the the original sequence.

However, this is not entirely correct. 
Some non-increasing subsequences 
are not even reachable since 
we stop as soon as our sequence becomes non-increasing. 

For example, in the second example from the problem statement 
the sequence is non-increasing from the start, 
so no proper subsequence is reachable at all. 

For subsequences that are reachable, 
not every way of reaching them might be possible. 

For example, in the first example 
from the problem statement we can reach 
the '7 <first 6>' subsequence 
(note, it should be considered different from '7 <second 6>' subsequence) 
by eliminating the second 6, and then 4. 

However, it can't be reached by doing 
those operations in reverse order, 
since we'd stop right after eliminating 4.


Now instead of all (N-K)! ways 
to reach a certain subsequence, 
we need to count just the possible ways. 
The main trick in solving this problem is: 
let's count the impossible ways instead, and then subtract. 

The impossible ways are simply those when before the last removal, 
the subsequence was already a non-increasing sequence of length K+1. 
And for each such subsequence there are exactly K+1 ways 
to make an impossible removal and arrive at a subsequence of length K. 

That means the sum of numbers of impossible ways 
to reach all non-increasing subsequences of length K 
is equal to the total number 
of non-increasing subsequences of length K+1 times (N-K-1)! 
(the number of ways to reach the longer subsequence) 
times K+1.

To summarize, 
suppose AK is the number of non-increasing subsequences of length K. 
Then the answer to this problem is sum over K of AK*(N-K)!-AK+1*(N-K-1)!*(K+1).


Solving the sub-problem


We've now reduced our problem to a much simpler one: 
find AK. 
This problem can be solved used a somewhat 
standard dynamic programming approach, 
with a twist to make it run faster.

First, let's assume that all input numbers are different, 
and between 0 and N-1. 

It's not hard to transform them 
in this way without changing the answer. 

If there are equal numbers, 
we'll slightly reduce the number to the right, 
so that non-increasingness of all subsequences is preserved.

Our dynamic programming problem will now be: 

what is the number of non-increasing subsequences of length P 
that end with number Q? 

Let's call that number BP,Q. 
We will find them in the order Qs appear in the sequence.

It's not hard to see that BP,Q is just 
the sum of BP-1,Q' for all numbers Q' 
that are greater than Q and appear before Q in the sequence. 

Since we process the states in the order Qs appear in the sequence, 
we just need to take the sum over all Q' that are greater than Q.

This is already a working solution for our problem, 
but it is a bit too slow: 
it runs in O(N3) which is a bit too much for N=8000. 
We have O(N2) states in our dynamic programming, 
and we need to find a sum of O(N) numbers to process each state. 

However, we can compute such sum faster! 
We just need an array-like data structure 
that supports changing elements 
and finding the sum of its suffix 
(over all Q' that are greater than Q), 

and the Fenwick tree is a data structure 
that does exactly that, 
performing each operation in O(logN) time, 
for a total running time of O(N2*logN).











 */
 
 
 
 
 
 // Solution 1
 
 
 /*
 
 #define _CRT_SECURE_NO_WARNINGS
//#pragma comment(linker, "/STACK:134217728")

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
using namespace std;

typedef long long Int;
typedef pair<int,int> PII;
typedef vector<int> VInt;

#define FOR(i, a, b) for(i = (a); i < (b); ++i)
#define RFOR(i, a, b) for(i = (a) - 1; i >= (b); --i)
#define CLEAR(a, b) memset(a, b, sizeof(a))
#define SIZE(a) int((a).size())
#define ALL(a) (a).begin(),(a).end()
#define PB push_back
#define MP make_pair

#define MAX 128
#define MOD 10007

int R[MAX][MAX][MAX];
int F[MAX];
int A[MAX];

int SolveTest(int test)
{
	int N;
	scanf("%d", &N);

	int i, j, k, l;
	FOR(i, 0, N)
		scanf("%d", &A[i]);

	CLEAR(R, 0);
	R[0][0][0] = 1;

	FOR(i, 0, N)
		FOR(j, 0, N)
			FOR(k, 0, N + 1)
				if(R[i][j][k] != 0)
				{
					int from = max(i, k);
					FOR(l, from, N)
					{
						if(i != 0 && A[l] > A[i - 1])
							continue;

						if(i < k && (i == 0 || A[i - 1] >= A[k - 1]) && A[k - 1] >= A[l])
							continue;

						R[l + 1][j + 1][k] += R[i][j][k];
						R[l + 1][j + 1][k] %= MOD;
					}

					if(k == 0)
						FOR(l, from, N)
						{
							R[i][j + 1][l + 1] += R[i][j][k];
							R[i][j + 1][l + 1] %= MOD;
						}
				}


	F[0] = 1;
	FOR(i, 1, N + 1)
		F[i] = F[i - 1]*i % MOD;

	Int res = 0;
	FOR(i, 0, N + 1)
		FOR(j, 0, N + 1)
			FOR(k, 0, N + 1)
				if(R[i][j][k] != 0)
				{
					if(k == 0 && j != N)
						continue;

					if(i < k && (i == 0 || A[i - 1] >= A[k - 1]))
						continue;
		
					res += (R[i][j][k])*F[N - j] % MOD;
				}

	if(N == 1)
		res = 1;

	printf("Case #%d: %d\n", test + 1, int(res % MOD));
	return 0;
}

int main()
{
	freopen("e.in", "r", stdin);
	freopen("e.out", "w", stdout);

	int T, t;
	char buf[1 << 7];
	gets(buf);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};







// Solution 2











#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <execinfo.h>

void my_sighandler (int sig) {
  void *array[100];
  size_t size;
  fprintf(stderr, "Error: signal %d:\n", sig);
  size = backtrace(array, 10);
  backtrace_symbols_fd(array, size, 2);
  exit(1);
}

int r[100][100][2];
int n;
int a[10000];

#define MOD 10007

void solve (void) {
  scanf ("%d", &n);
  assert (n <= 100);
  int i, j, k;
  for (i = 0; i < n; i++) {
    scanf ("%d", &a[i]);
  }
  int ok = 0;
  for (i = 1; i < n; i++) {
    if (a[i] > a[i - 1]) {
      ok = 1;
      break;
    }
  }
  if (!ok) {
    printf (" 1\n");
    return;
  }
  memset (r, 0, sizeof (r));
  int res = 0;
  int fac = 1;
  for (i = 0; i < n; i++) {
    r[i][1][0] = 1;
    for (j = 0; j < i; j++) {
      if (a[i] > a[j]) {
        r[i][1][1] ++;
      }
    }
    int count = 0;
    for (j = i - 1; j >= 0; j--) {
      if (a[i] > a[j]) {
        for (k = 0; k < n; k++) {
          r[j][k][1] += r[j][k][0];
          if (r[j][k][1] >= MOD) {
            r[j][k][1] -= MOD;
          }
        }
        count ++;
      } else {
        for (k = 2; k < n; k++) {
          r[i][k][0] += r[j][k - 1][0];
          if (r[i][k][0] >= MOD) {
            r[i][k][0] -= MOD;
          }
          r[i][k][1] = (r[i][k][1] + r[j][k - 1][1] + count * r[j][k - 1][0]) % MOD;
        }
      }
    }
  }
  fac = 1;
  for (i = n - 1; i >= 1; i--) {
    for (j = 0; j < n; j++) {
      res = (res + r[j][i][1] * fac) % MOD;
    }
    fac = (fac * (n - i)) % MOD;
  }
  printf (" %d\n", res);
 
}

int main () {
  signal (SIGSEGV, my_sighandler);
  signal (SIGFPE, my_sighandler);
  signal (SIGABRT, my_sighandler);
  int _, __;
  assert (scanf ("%d", &_) == 1);
  for (__ = 0; __ < _; __ ++ ) {
    printf ("Case #%d:", __ + 1);
    solve ();
  }
  return 0;
}

*/


/*
 
 Introduction

 We often need some sort of data structure to make our algorithms faster. 
 In this article we will discuss the Binary Indexed Trees structure. 

 According to Peter M. Fenwick, this structure was first used for data compression. 
 Now it is often used for storing frequencies and manipulating cumulative frequency tables.

 Let's define the following problem: 
 We have n boxes. 
 Possible queries are

   1. add marble to box i 
   2. sum marbles from box k to box l

 The naive solution has time complexity of O(1) for query 1 and O(n) for query 2. 
 Suppose we make m queries. 
 The worst case (when all queries are 2) 
 has time complexity O(n * m). 
 
 Using some data structure (i.e. RMQ) we can solve 
 this problem with the worst case time complexity of O(m log n). 
 Another approach is to use Binary Indexed Tree data structure, 
 also with the worst time complexity O(m log n) 
 -- but Binary Indexed Trees are much easier to code, 
 and require less memory space, than RMQ.


 Notation

  BIT - Binary Indexed Tree
  MaxVal - maximum value which will have non-zero frequency
  f[i] - frequency of value with index i, i = 1 .. MaxVal 
  c[i] - cumulative frequency for index i (f[1] + f[2] + ... + f[i])
  tree[i] - sum of frequencies stored in BIT with index i 
  (latter will be described what index means); 
  sometimes we will write tree frequency 
  instead sum of frequencies stored in BIT
  num¯ - complement of integer num 
  (integer where each binary digit is inverted: 0 -> 1; 1 -> 0 )
  NOTE: 
  Often we put f[0] = 0, c[0] = 0, tree[0] = 0, 
  so sometimes I will just ignore index 0.

  
  Basic idea

  Each integer can be represented as sum of powers of two. 
  In the same way, cumulative frequency can be represented 
  as sum of sets of subfrequencies. 
  
  In our case, each set contains some successive number of non-overlapping frequencies.

  idx is some index of BIT. 
  r is a position in idx of the last digit 1 
  (from left to right) in binary notation. 
  
  tree[idx] is sum of frequencies from index 
  (idx - 2^r + 1) to index idx (look at the Table 1.1 for clarification). 
  We also write that idx is responsible for indexes
  from (idx - 2^r + 1) to idx 
  
  (note that responsibility is the key in our algorithm 
   and is the way of manipulating the tree).

     r     0    1   1   2   0   1   0    3     0    1     0     2     0     1      0     4

    2^r    1    2   2   4   1   2   1    8     1    2     1     4     1     2      1     16
    2^r-1  0    1   1   3   0   1   0    7     0    1     0     3     0     1      0     15     
	i      1    2   3   4   5   6   7    8     9    10    11    12   13    14     15     16   
  i-2^r+1  1    1   2   1   5   5   7    1     9    9     11    9    13    13     15     1

    bin    1   10  11  100 101 110 111 1000  1001  1010  1011  1100  1101  1110  1111  10000
           1	2	3	4	5	6	7	8	   9	10	  11	12	  13	14	  15	16
    f	   1	0	2	1	1	3	0	4	   2	5	   2	2	   3	1	   0	2
    c	   1	1	3	4	5	8	8	12	   14	19	  21	23	  26	27	  27	29
   tree	   1	1	2	4	1	4	0	12	   2	7	   2	11	   3	4	   0	29

         1    2   3   4    5   6    7   8    9   10    11   12   13   14   15  16 
    tree 1   1..2 3  1..4  5  5..6  7  1..8  9  9..10  11  9..12 13 13..14 15 1..16 


	idx = 5 = 101, r = 3.  5 - 8 + 1 = -2.


    5 - 2^r + 1 = 5 = 2^r = 1 => r = 0
    
    
	Tree of responsibility:

    16(2) -                   |29|
    15(0) - |0|               |  |
	14(1) -     |4|           |  |
	13(3) - |3| | |           |  |
	12(2) -         |11|      |  | 
	11(2) - |2|     |  |      |  |
	10(5) -     |7| |  |      |  |
	9(2)  - |2| | | |  |      |  |
	8(4)  -              |12| |  |
	7(0)  - |0|          |  | |  |
	6(3)  -     |4|      |  | |  |
	5(1)  - |1| | |      |  | |  |
	4(1)  -         |4|  |  | |  |
	3(2)  - |2|     | |  |  | |  |
	2(0)  -     |1| | |  |  | |  |
	1(1)  - |1| | | | |  |  | |  |
	0(0) ----------------------------------------
    
 */


//int data[10] = { 23, -12, 1, 89, 10, -5, 25, 2, 23, 0, -3, 20, 4, 13, 4 };

/* at = 1;
 * at = 1 | 2 = 01 | 10 = 01 = 11 = 3
 *                        10 
 * at = 3 | 4 = 011 = 111 = 7
 *              100
 * at = 7 | 8 = 0111 = 1111 = 15
 *              1000        
 * at = 15 | 16 = 01111 = 11111 = 31
 *                10000
 * If we start from 
 * at = 0;
 * at = 0 | 1 = 00 = 1 
 *              01
 * If at = 2, at = 2 | 3 = 10 = 3
 *                         11
 * If at = 3, at = 3 | 4 = 011 = 111
 *                         100
 * 4|5 = 100 = 101 = 5
         101
   5|6 = 101 = 111 
         110 
 */
/*
void update(int* data, int dataLen, int at, int by) 
{
    while (at < dataLen) 
	{
        data[at] += by;
        at |= (at + 1);
    }
}

int query(int* data, int at) 
{
    int res = 0;

    while (at >= 0) 
	{
        res += data[at];
        at = (at & (at + 1)) - 1;
    }

    return res;
}

void testFenwikTree()
{
    int query1, query2, query3;

	update(data, 15, 1, 10);

	query1 = query(data, 2);
    query1 = query(data, 4);
    query1 = query(data, 8);
}
*/





