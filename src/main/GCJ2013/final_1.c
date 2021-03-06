
/*

  
  Problem

  Before graduating from Awesome Programmer University, 
  students traditionally perform certain "graduation requirements". 
  One of these is driving around a traffic circle backwards. 
  For most people, this is crazy enough, but as an extra challenge, 
  you want to see if you can go backwards around the traffic circle multiple times without stopping.

  The traffic circle consists of N intersections, spaced evenly around the circle. 
  A car would normally enter the traffic circle at one intersection, 
  and then every second, 
  it will move to the next counter-clockwise intersection, 
  until eventually it reaches its destination and leaves.

  You have been watching cars enter and leave the traffic circle for X seconds. 
  For each car, you record the time it enters the circle, 
  as well as the intersections it enters and leaves at. 
  All cars are moving counter-clockwise at the rate of 1 intersection per second. 
  Each car you watched exited the circle 
  before coming back to the intersection it entered at. 
  There are multiple lanes on the traffic circle, 
  so multiple cars can occupy the same position at the same time.

  If you had planned it just right, 
  how long could you have driven clockwise in the traffic circle during this time? 
  
  You must enter the circle at some integer time >= 0, 
  leave at time <= X, 
  and once you leave, you are not allowed to come back. 
  
  When in the traffic circle, you must travel clockwise 
  at the rate of 1 intersection per second. 
  You want to play it safe 
  (well, as safe as driving backwards on a traffic circle can be), 
  so you must never touch or pass by another car. 
  
  In particular, you cannot leave the circle at an intersection 
  at which another car is entering at the same moment, 
  and you cannot enter the circle at an intersection 
  at which another car is leaving at the same moment. 
  You can choose when and where to enter and leave the circle.


  Input


  The first line of the input gives the number of test cases, T. 
  T test cases follow. 
  The first line of any test case describes the number C of cars you observed. 
  The second line contains two integers, X and N 
  — the time (in seconds) for which you observed the circle, 
  and the number of intersections on the circle. 
  Next C lines describe the cars you have seen. 
  Each of those lines contains three integers si, ei and ti — 
  the intersection at which the car entered the circle, 
  the intersection on which it left and the time at which it entered. 
  The intersections are numbered from 1 to N, counterclockwise 
  (that is, the intersection number 2 is the next intersection counterclockwise from number 1).


  Output

  For each test case, output one line containing "Case #x: y", 
  where x is the case number (starting from 1) 
  and y is the maximum number of seconds you can travel on the circle. 
  Note that y can be zero both in the case 
  where you cannot enter the circle at all 
  and in the case when you can enter it, 
  but can't travel even one intersection.


  Remember that you are required to enter the circle 
  at a time expressed as an integer number of seconds 
  — you must enter at an integer time, 
  and thus arrive at each intersection at an integer time.


  Limits

  1 ≤ T ≤ 100
  1 ≤ si, ei ≤ N
  si ≠ ei 
  0 ≤ ti

  Each observed car leaves the circle at time X or earlier.
  Small dataset

  3 ≤ N ≤ 10
  1 ≤ X ≤ 10
  0 ≤ C ≤ 10

  Large dataset

  3 ≤ N ≤ 1010
  1 ≤ X ≤ 1010
  0 ≤ C ≤ 1000

  Sample


Input 
 	
Output 
 
5
1
3 4
1 4 0
6
3 5
5 2 0
5 1 2
1 3 0
1 2 2
2 3 0
3 4 0
3
2 3
1 3 0
2 1 0
3 2 0
0
6 4
1
2 3
1 3 0
Case #1: 1
Case #2: 2
Case #3: 0
Case #4: 6
Case #5: 0



In the first sample case, we have one car, 
going as in the picture in the statement. 
There are a number of ways allowing us to travel backwards 
for one second — 
for instance, we can enter at intersection 1 at time 1 
(we can't enter at time zero, because the other car is there), 
and travel to intersection 4 
(we can't go on to intersection 3, 
as we would pass the other car which will be going from 3 to 4). 
Another option is to enter at intersection 4 at time 0, 
and travel to intersection 3 (and then exit).


In the second sample case, we can travel for two seconds 
by entering at intersection 5 at time 1, 
and traveling backwards to intersection 3. 

In the third sample case, we can't even enter the circle 
- there are cars at all intersections at every full second. 

In the fourth case there are no cars, 
so we can just enter the circle at any point at time 0 
and travel round and round till time 6. 

In the fifth case we can enter the circle, 
but since there are only three intersections, 
we will always collide with the other car if we try to move to the next one.

Note: 

Driving against the direction of the traffic on a traffic circle 
is typically not a wise thing to do 
and may cause harm to you or other people. 
Google (and Google Code Jam in particular) 
encourages you not to try this.


Solution:


In this problem we are interested in finding 
the maximum time we can drive against the direction in a traffic circle. 

For small test cases, the size of the input is small enough 
to try all possible intersections, 
time to start, 
and check the length to drive without touching any other car.


For large test cases, the above approach is not good enough 
due to the extremely large N and X (up to 10^10). 

Therefore in order to solve the large test cases, 
we transform the problem into the 2-dimensional plane 
with intersection and time as axes. 

Then, each car can be represented as line segments; 
your car will be represented as a line segment 
orthogonal to other line segments formed by other cars. 

Figure 1 shows the second sample test case. 
Lined segments corresponding to different cars 
are denoted with different colors 
while the black dotted segment corresponds to your car. 

Note that as the intersections are in a traffic circle, 
we show it by repeating intersection 5 and 1 on the two ends in Figure 1.


So the problem can be reformulated as follows: 
given a set of line segments, 
find the maximum possible length of a perpendicular segment 
which does not touch any other segment. 

Note that if two segments have a point in common 
the corresponding cars touch each other.

To solve the large test case one needs to notice 
that a line segment of maximum possible length can be chosen 
so that it goes near one of the endpoints of another segment. 

By “near” we mean distance = 1 in one of the axes. 
Indeed, if we have a segment of maximum possible length, 
that does not go near one of the endpoints, 
we can always move it so that it does, see Figure 2 for an example.

Figure 3

The statement is also true in examples such as in Figure 3 
because intersections are arranged in a circular manner 
and the solution segment is near the top right endpoint which is shown with an arrow.

Figure 4

Thus we note that changes in the length of segments happen near these endpoints. 
Therefore to solve the problem we need to go over all endpoints 
of car segments and consider its neighbourhood (-1 and +1 in both of the axes). 

Note that we also need to consider segments that go through endpoints +/-(1,1), 
an example is shown in Figure 4. 

Then we can compute all segments that pass through 
these near points and pick the longest segment as the answer.

For each such candidate point, 
we need to check how far up and down a segment 
that go through the candidate point can reach without touching other segments. 

This can be done simply by going over all car segments 
and checking if and where our candidate segment intersect them. 

The complexity of this algorithm is O(C2), which is enough to solve the large case.

*/

/*

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

long long C, X, N;
long long cs[1001], ce[1001], ct[1001];
long long ret;

void doit(long long t, long long x) {
//cout << x+1 << '@' << t;
	long long t1 = 0, t2 = X;
	x = (x+t)%N;
	for (long long i = 0; i < C; i++) {
		long long st = ct[i];
		long long et = ct[i] + (ce[i]+N-cs[i])%N;
		long long p1 = ((x-st)%N+N)%N;
		long long p2 = ((x-et)%N+N)%N;
		long long s = cs[i];
		long long e = ce[i];
		while (e < s) e += N;
		while (p2 > p1) p2 -= N;
		while (p1 < e) {p1 += N; p2 += N;}
		long long clash1a = -1, clash1b = -1, clash2a = -1, clash2b = -1;
		if (p2 <= e) {
			clash2a = et - (e-p2+1)/2;
			clash2b = et - (e-p2)/2;
			if (clash2a == clash2b && clash2a == t) return;
		}
		while (p2 > s) {p1 -= N; p2 -= N;}
		if (p1 >= s) {
			clash1a = st + (p1-s)/2;
			clash1b = st + (p1-s+1)/2;
			if (clash1a == clash1b && clash1a == t) return;
		}
		if (clash1a != -1) {
			if (t <= clash1a) t2 = min(t2, clash1a - (clash1a == clash1b));
			else t1 = max(t1, clash1b + (clash1a == clash1b));
		}
		if (clash2a != -1) {
			if (t <= clash2a) t2 = min(t2, clash2a - (clash2a == clash2b));
			else t1 = max(t1, clash2b + (clash2a == clash2b));
		}
	}
//cout << ' ' << t1 << ' ' << t2 << endl;
	ret = max(ret, t2-t1);
}

main() {
	long long T, prob=1;
	for (cin >> T; T--;) {
		cin >> C >> X >> N;
		for (long long i = 0; i < C; i++) {
			cin >> cs[i] >> ce[i] >> ct[i];
			cs[i]--; ce[i]--;
		}
		ret = 0;
		doit(0, 0);
		doit(X, 0);
		for (long long i = 0; i < C; i++) {
			doit(ct[i], (cs[i]+(N-1))%N);
			doit(ct[i], (cs[i]+(N-2))%N);
			doit(ct[i] + (ce[i]+N-cs[i])%N, (ce[i]+1)%N);
			doit(ct[i] + (ce[i]+N-cs[i])%N, (ce[i]+2)%N);
		}
		cout << "Case #" << prob++ << ": " << ret << endl;
	}
}


*/

