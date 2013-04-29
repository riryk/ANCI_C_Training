/*


Problem
 
 You are standing on a ledge in the jungle, 
 and your one true love is standing on a similar ledge 
 at the other side of a swamp infested with snakes, 
 crocodiles and a variety of other unpleasant denizens. 
 
 Fortunately, there is a number of vines hanging from the canopy of the jungle 
 over the swamp, even more fortunately, 
 you somehow managed to get hold of the first of these vines (see figures below). 
 
 The canopy of the jungle is at a constant height, 
 and both the ledges are at the same height as the canopy. 
 The vines are simply lines hanging from the canopy at certain points, 
 with differing lengths.
 
 If you happened to be a fictional hero, 
 you would just go swinging wildly and yelling, 
 at some point let go of the vine you hold, 
 fly in the air for some time, catch another vine, 
 swing again, and after a few repetitions you would 
 be holding your one true love in your arms. 
 
 Unfortunately, you are not a fictional hero, 
 and if you tried that, probably yelling would 
 be the only part you would manage well.
 
 Your plan is a bit more cautious. 
 You will swing on the vine you hold, 
 but instead of letting go, 
 you will catch hold of another vine. 
 
 Then you will slowly and carefully climb up your original vine, 
 so that the new vine you are holding will become horizontal 
 - either to its full length, or up to the distance 
 between the two vines, whichever is smaller. 
 
 Then you will rest for a bit, and swing again, to repeat the process. 
 Note that you do not have to catch the first vine 
 you come up against while swinging, 
 you might prefer to swing a bit further 
 and catch some further-off vine instead. 
 
 You can also climb up the vine you're currently swinging back and forth on 
 to reduce the distance between you and the root of the vine. 
 
 In effect, this means that you can catch any vine 
 that your vine crosses while swinging. 
 Note that you will not climb down a vine while swinging.
 
 One other thing that sets you apart from any fictional hero 
 is that before you start the whole rather risky procedure 
 you would like to know whether it is actually possible 
 to reach the other side of the jungle this way. 
 
 And this is the question you have to answer in this problem.
 
 Input
 
 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 The first line of each test case contains the number N of vines. 
 N lines describing the vines follow, 
 each with a pair of integers di and li 
 - the distance of the vine from your ledge, 
 and the length of the vine, respectively. 
 
 The last line of the test case contains the distance D 
 to the ledge with your one true love. 
 You start by holding the first vine in hand.
 
 Output
 
 For each test case, output one line containing "Case #x: y", 
 where x is the case number (starting from 1) 
 and y is a YES or a NO. 
 Indicating whether it is possible for you to reach your one true love, 
 given the rules above.
 
 Limits
 
 0 < di, li, D ≤ 109.
 T ≤ 30. 
 di < di+1.
 As you hold the first vine, d0 ≤ l0.
 dN-1 < D. 

 Small dataset
 
 1 ≤ N ≤ 100.

 Large dataset
 
 1 ≤ N ≤ 10000.
 There will be at most 60000 vines in all the test cases in total. 

 Sample
 Input 

 Output

 4
 3
 3 4
 4 10
 6 10
 9
 3
 3 4
 4 10
 7 10
 9
 2
 6 6
 10 3
 13
 2
 6 6
 10 3
 14
 

 Case #1: YES
 Case #2: NO
 Case #3: YES
 Case #4: NO

 In the first case, you hold the first vine 3 units away 
 from where it is attached. 
 You swing wildly, bypass the second vine 
 and just barely catch the third. 
 
 The picture below depicts the starting situation, 
 and you are able to reach any vine 
 that is rooted anywhere within the red interval:



 Translating to a graph problem
 
 To analyse the problem, 
 let us begin with describing the state we can be in 
 at any particular point during traversing the swamp. 
 
 It is easy to see that we need two items of information to describe this state 
 - which vine are we currently holding on to, 
 and how far away from the root are we holding it.
 
 Once we have such a description, 
 we can frame the problem as a path-finding problem in a graph. 
 We start in the state (0, d0), 
 and we want to any state (i, p) with p + di ≥ D 

 (to simplify, we can also add an artificial N+1st vine 
  where our true love stands and demand that we reach the state (N, p) for any p).
 
 The transitions between states 
 (or, in other words, edges in the graph) 
 are allowed swings we can make. 
 
 If we currently hold vine i, at p units away from the root, 
 we can swing to any vine j rooted between di - p and di + p, 
 and the new length will be the minimum of |di - dj| and lj. 
 Note that the only use of climbing up a vine is 
 to catch a vine that would be too short to be within our swing path 
 - so we implicitly include it in the transitions described above 
 (and thus do not need extra transitions from (i, p) to (i, p-1)).
 
 Limiting the number of nodes
 
 As described, we could have a large number of states 
 (as there are very many possible vine lengths. 
  A crucial fact to notice is that the second part of the state 
  (the length away from root) 
  is uniquely determined by the previous vine we got here from; 
  and it is independent of where did we hold it 
  (assuming we held it far enough to actually reach this state). 
  This means that there are at most N2 states we will ever consider, 
  as each is determined by a pair of vine indices.
 
 We can now solve the small input. 
 We have a graph with N2 nodes and at most N edges from each node, 
 and we want to verify whether a path between some two nodes exists 
 (we can merge all the target nodes into one node for simplicity). 
 As we have at most N3 edges in total, any standard graph traversal algorithm 
 (like BFS or DFS) will allow us to solve the problem.
 
 Limiting the number of edges
 
 For the large input, a O(N3) solution will be unsatisfactory, 
 and we need a bit more subtlety. 
 There is a number of tricks one can use to beat down the complexity. 
 One is to make use again of the fact that the target node depends 
 only on the vine we start from, 
 and not on the position at which we hold it. 
 
 This means that there are in fact at most N edges from a given vine 
 - if we manage to reach some vine j from a given vine i 
 when holding it at position A, 
 we do not need to check this edge when considering moves from vine i held 
 at position B 
 (because even if we reach vine j, we will arrive in a state that we have already analysed). 
 Thus, we need to make at most N2 edge traversals to visit all reachable nodes. 
 There are various techniques to actually code this 
 (for instance, for each vine, we could order the other vines by distance from this vine, 
  and each time process this list from the closest vine and remove all traversed edges), 
  we encourage you to explore the options.
 
 An alternative for the large problem
 
 An alternative is to notice another fact - 
 the position part of the state 
 (that is, the distance away from the root that you hold the vine at) never increases. 
 This is because if you swing from vine i to vine j, it means you were holding i 
 at least |di - dj| away from the root, 
 while this quantity is at the same time an upper bound on the position you will hold vine j at.
 
 This means that we can use Dijkstra's algorithm to find, 
 for each vine, the maximum position we can hold this vine at 
 - we treat the decreasing vine position as increasing time, 
 and in each step we analyse what lengths could we obtain 
 for each vine by moving from the current vine, 
 and then choose the vine with the largest length to analyse. 
 This will give us a O(N2logN) solution, which should be fast enough.
 
 Going even faster
 
 A fun fact is that this problem can be solved faster than O(N2), 
 although you didn't need to notice this to solve our data sets. 
 The key fact here is that if you can pass the swamp at all, 
 you can always do it without going backwards 
 (that is, you always catch a vine that's in front of you, you never swing back). 
 An easy way to use this observation is to modify the Dijkstra's algorithm 
 mentioned above to process the vines from first to last, which will turn O(N2logN) into O(N2).
 
 To go down to O(N), we need one more trick. 
 Notice that if we can reach a vine, 
 we will get the largest (meaning best) position 
 if we swing to it from a vine that is as far away as possible. 
 As we move only forward, this means that as soon as we can reach any particular vine, 
 we should note the position achieved and we never need to check any other way of reaching it. 
 This means we can get an O(N) solution by keeping track of the vine 
 we are currently processing and farthest we have reached so far, 
 and from each vine trying to update only vines that we have not reached as yet. 
 As each vine will be updated at most once, and read at most once, we will do O(N) operations.
 
 We encourage you to flesh out the details and try to prove the "never go back" lemma 
 - it's not trivial!


// another fine solution by misof
// #includes {{{
#include <algorithm>
#include <numeric>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define REP(i,n) for(int i=0;i<(int)(n);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        int N; cin >> N;
        vector<int> D(N+1), L(N+1);
        REP(n,N) cin >> D[n] >> L[n];
        cin >> D[N]; L[N] = 0;
        vector<int> B(N+1,-1);
        B[0] = D[0];
        deque<int> Q; Q.push_back(0);

        for (int c=1; c<=N; ++c) {
            while (!Q.empty() && D[Q.front()]+B[Q.front()] < D[c]) Q.pop_front();
            if (Q.empty()) break;
            B[c] = min(L[c], D[c] - D[Q.front()]);
            if (Q.empty()) Q.push_back(c);
            if (D[c]+B[c] > D[Q.back()]+B[Q.back()]) Q.push_back(c);
        }
        printf("Case #%d: %s\n",t,B[N]==-1?"NO":"YES");
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread



#include <stdio.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <assert.h> 
#include <map> 
#include <vector> 
#include <string> 
#include <algorithm> 
using namespace std; 

typedef long long ll; 

#define modp(x) (((x)%P+P)%P) 
#define mod(x,y) (((x)%(y)+(y))%(y)) 

main(){
  int testcases;
  scanf("%d",&testcases);
  for(int casenum=1;casenum<=testcases;casenum++){
    int n;
    scanf("%d",&n);
    int pos[20000],l[20000];
    for(int i=0;i<n;i++)scanf("%d%d",&pos[i],&l[i]);
    int goal;
    scanf("%d",&goal);
    int dp[20000];
    for(int i=n-1;i>=0;i--){
      dp[i]=goal-pos[i];
      for(int j=i+1;j<n;j++){
	if(dp[j]>l[j])continue;
	int tmp=min(pos[j]-pos[i],l[j]);
	if(tmp>=dp[j]){dp[i]=min(dp[i],pos[j]-pos[i]);break;}
      }
      //printf("dp %d = %d\n",i,dp[i]);
    }
    printf("Case #%d: ",casenum);
    if(dp[0]<=pos[0])puts("YES");
    else puts("NO");
  }
}


 Input
 
 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 The first line of each test case contains the number N of vines. 
 N lines describing the vines follow, 
 each with a pair of integers di and li 
 - the distance of the vine from your ledge, 
 and the length of the vine, respectively. 
 
 The last line of the test case contains the distance D 
 to the ledge with your one true love. 
 You start by holding the first vine in hand.


 */


#include <iostream>
#include <cstdio>
#include <vector>
#include <cassert>

using namespace std;

void solve() {
    bool can = false;

    int n, D;
    cin >> n;
    vector<int> l(n);
    vector<int> d(n);

    for (int i = 0; i < n; i++)
        cin >> d[i] >> l[i];
    assert(d[0] <= l[0]);
    cin >> D;

    vector<int> best(n, -1);
    best[0] = d[0];

    for (int i = 0; i < n; i++) 
		if (best[i] != -1) 
		{
           for (int j = i + 1; j < n; j++) 
		   {
             if (d[i] + best[i] >= d[j]) 
			 {
                best[j] = max(best[j], min(l[j], d[j] - d[i]));
             }
           }

           if (d[i] + best[i] >= D) can = true;
        }


    static int test;
    cout << "Case #" << ++test << ": ";
    if (can) cout << "YES"; else cout << "NO";
    cout << endl;
}

int main() {
    int t;
    cin >> t;
    while (t--)
        solve();
    return 0;
}



void Solve()
{
    int testcases; // Number of testcases
	int casenum;
	int i;
	int j;
    scanf("%d",&testcases);
    for (casenum = 1; casenum <= testcases; casenum++)
    {
        int n;          //The number of lines
        int pos[20000]; //The distance from my current ledge to the vine
        int l[20000];   //The length of the vine
        int dp[20000];  //Distance from goal to a vine 
		int goal;       //The distance from the current ledge to the ledge 
		                //we need to go to.
		int tmp;        
        int best[20000];
       
        scanf("%d",&n); //Read the number of lines  
        for(i = 0; i < n; i++) 
		   scanf("%d%d",&pos[i],&l[i]);

        scanf("%d",&goal);

        for (i = 0; i < n; i++)
			best[i] = -1;

        best[0] = pos[0];

        for (i = 0; i < n; i++) 
		{
			if (best[i] != -1) 
			{
               for (j = i + 1; j < n; j++) 
			   {
                   if (d[i] + best[i] >= d[j]) 
				   {
                      best[j] = max(best[j], min(l[j], d[j] - d[i]));
                   }
               }

               if (d[i] + best[i] >= D) can = true;
			}
        }


        for (i = n - 1; i >= 0; i--)
		{
           dp[i] = goal - pos[i];
           for(j = i + 1; j < n; j++)
		   {
	          if (dp[j] > l[j])
				  continue;

	          tmp = min(pos[j] - pos[i], l[j]);
	          if (tmp >= dp[j])
			  {
				  dp[i]=min(dp[i],pos[j]-pos[i]);
				  break;
			  }
           } 
        }
    }
}