/*
 

  Problem



  You are asked to help diagnose class diagrams to identify instances of diamond inheritance. 
  The following example class diagram illustrates the property of diamond inheritance. 
  There are four classes: A, B, C and D. 
  An arrow pointing from X to Y indicates that class X inherits from class Y.

  In this class diagram, 
  D inherits from both B and C, 
  B inherits from A, 
  and C also inherits from A. 
  
  An inheritance path from X to Y is defined as a sequence of classes 
  X, C1, C2, C3, ..., Cn, Y 
  where X inherits from C1, Ci inherits from Ci + 1 for 1 ≤ i ≤ n - 1, 
  and Cn inherits from Y. 
  
  There are two inheritance paths from D to A in the example above. 
  The first path is D, B, A and the second path is D, C, A.

  A class diagram is said to contain a diamond inheritance 
  if there exists a pair of classes X and Y such that there are 
  at least two different inheritance paths from X to Y. 
  
  The above class diagram is a classic example of diamond inheritance. 
  Your task is to determine whether or not a given class diagram contains a diamond inheritance.



  Input



  The first line of the input gives the number of test cases, T. 
  T test cases follow, each specifies a class diagram. 
  The first line of each test case gives the number of classes in this diagram, N. 
  The classes are numbered from 1 to N. 
  N lines follow. 
  The ith line starts with a non-negative integer 
  Mi indicating the number of classes that class i inherits from. 
  This is followed by Mi distinct positive integers each from 1 to N representing those classes. 
  You may assume that:

  If there is an inheritance path from X to Y then there is no inheritance path from Y to X.
  A class will never inherit from itself.

  Output

  For each diagram, output one line containing "Case #x: y", 
  where x is the case number (starting from 1) and y is "Yes" 
  if the class diagram contains a diamond inheritance, "No" otherwise.

  Limits

  1 ≤ T ≤ 50.
  0 ≤ Mi ≤ 10.

  Small dataset

  1 ≤ N ≤ 50.

  Large dataset

  1 ≤ N ≤ 1,000.

  Sample

  Input 
 	
  Output 
 
  3
  3
  1 2 
  1 3 
  0
  5
  2 2 3
  1 4
  1 5
  1 5
  0
  3
  2 2 3
  1 3
  0

  Case #1: No
  Case #2: Yes
  Case #3: Yes


  Solution


  We are given a directed graph, 
  and have to determine if there is a pair of nodes (X,Y) 
  such that there are two or more paths from X to Y.


  For each node, we do a depth-first search with that node as the root. 
  If during the depth-first search we reach the same node twice, 
  then we must have followed two different paths to get to 
  that node from the root node, 
  so we have found a pair (X,Y). 
  
  Conversely, if there are two paths between X and Y, 
  we will reach Y at least twice when doing a DFS from X. 
  So if this algorithm finds no pair (X,Y), then none exists in the graph.

  If there are V nodes and E edges in a graph, 
  then a DFS is O(V+E) in general. 
  But each DFS will never follow more than V edges, 
  because after following that many edges, 
  some node will have been reached twice, 
  so we can stop at that point. 
  Therefore this algorithm is O(V2).

  We can also just use a variation of Floyd's algorithm,
  which is O(V3), 
  but very simple to write. 
  With only 1000 nodes in the graph, 
  a fast implementation will finish inside 8 minutes.

 */

int Pr15IsQueueEmpty() { }
void Pr15Pop() { }
void Pr15Push(int k) {  }
int Pr15Front() { return 1; }

void Pr15Solve()
{
	int i, j, t, p, m;
	int TT;
	int N;
	int ncon[1000];
	int con[1000][10];
	int vis[1000];
	scanf("%d",&TT);
    for (i = 0; i < TT; i++)
	{
        while (!Pr15IsQueueEmpty())
		   Pr15Pop();
        scanf("%d",&N);
		for (j = 0; j < N; j++)
	    {
		   scanf("%d",&ncon[j]);
		   for (t = 0; t < ncon[j]; t++)
		   {
			   scanf("%d",&con[j][t]);
			   con[j][t]--;
		   }
	    }
		for (j = 0; j < N; j++)
		{
		   for (t = 0; t < N; t++) vis[t] = 0;
		   Pr15Push(j); 
           while (!Pr15IsQueueEmpty())
		   {
               p = Pr15Front();
			   Pr15Pop();
			   if (vis[t] == 1) { /* We have found  */ }
			   vis[j]=1;
			   for (m = 0; m < ncon[p]; m++)
                  Pr15Push(con[p][m]);
		   }
		}
	}	
}