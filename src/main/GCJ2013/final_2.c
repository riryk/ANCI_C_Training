
/*

  
  The drummer has a very important role in any band 
  -- keeping the rhythm. 
  If the drummer's rhythm is uneven, 
  it can ruin the entire performance.

  You are the lead singer of a very popular rock band, 
  and you have a bit of a problem. 
  Your drummer has just quit the band 
  to become a professional video gamer. 
  
  You need to find a new drummer immediately. 
  Fortunately, there is no shortage of candidates. 
  Everyone wants a chance to join your band. 
  
  Your task is to find the best drummer among the candidates, 
  and you want the person who can keep the most consistent rhythm.

  Your plan is as follows. 
  You will ask each candidate to audition individually. 
  During the audition, the candidate will play one drum 
  by striking it with a drum stick several times. 
  
  Ideally, the time difference between consecutive strikes 
  should be exactly the same, 
  producing a perfect rhythm. 
  
  In a perfect rhythm, the drum strikes 
  will have time stamps that follow an arithmetic progression like this: 
  T0, T0 + K, T0 + 2*K, ..., T0 + (N - 1)*K.

  In real life, of course, it is nearly impossible 
  for a human to produce a perfect rhythm. 
  Therefore, each candidate drummer will produce a rhythm 
  that has an error E, 
  such that each Ti differs by at most E 
  from some perfect rhythm. 
  
  Given a candidate's sequence of drum strikes, 
  find the smallest possible E among all perfect rhythms 
  that the candidate might have been trying to play.


  Input


  The first line of the input gives the number of test cases, T. 
  T test cases follow.
  Each one consists of two lines 
  and represents the audition of one candidate. 
  The first line contains a single integer -- N. 
  
  The next line contains N integers separated by spaces 
  -- the time stamps, in milliseconds, 
  of the drum strikes played by the candidate. 
  The time stamps are in increasing order.


  Output

  For each test case, output one line containing 
  "Case #x: E", where x is the case number (starting from 1) 
  and E is the smallest among all possible numbers 
  that describe the error of the candidate's drum strike sequence.


  Your answer will be considered correct 
  if it is within an absolute or relative error of 10-6 of the correct answer. 
  See the FAQ for an explanation of what that means, 
  and what formats of floating-point numbers we accept.


  Limits


  1 ≤ T ≤ 100.
  Small dataset

  2 ≤ N ≤ 10.
  0 ≤ Ti ≤ 100.
  Large dataset

  For 90% of the test cases, 2 ≤ N ≤ 1000.
  For all test cases, 2 ≤ N ≤ 50000.
  0 ≤ Ti ≤ 106.

  Sample

  Input 
 	
  Output 
 
  3
  2
  10 70
  4
  0 10 19 30
  6
  2 5 10 15 20 24

  Case #1: 0
  Case #2: 0.5
  Case #3: 0.75



  This problem can be viewed as a modified version 
  of finding the minimum width of a convex hull [1] 
  which can be solved using a modified version 
  of the rotating calipers technique [2,3]. 
  
  We present below the intuition on why the problem 
  is similar to finding the modified minimum width of the convex hull.

  First off, we can plot the sequence of drum strikes as (i, Ti )
  on the 2-dimensional plane. 
  
  If the drummer performs perfectly, 
  then let’s call the sequence Pi and (i, Pi) 
  falls on the same line L (see Figure 1 and Figure 2).

  Figure 1
  Figure 2

  Unfortunately the drummer does not perform perfectly 
  and has error Ei (which is |Ti - Pi|) 
  (see Figure 3).
  

  Figure 3


  So in the example in Figure 3, 
  we have our line L, 
  and it is clear that our answer is the maximum among all Ei, 
  which in this case is E1 (or E2 or E4). 
  
  If we draw two lines parallel to L shifted 
  in the y-axis by +E1 and -E1, 
  denoted as L+E1 and L-E1 (see Figure 3), 
  you will notice that the region between 
  the two lines contains all the points (i, Ti); 
  
  this example fits the definition of the problem 
  which is a drum rhythm 
  where each strike differs by at most E 
  from some perfect rhythm. 
  
  On the other hand, if we choose E5 as the error candidate, 
  the region between the two lines L+E5 and L-E5 
  will not contain all the points (i, Ti) (see Figure 4).


  Figure 4


  In essence, we want to find a line denoting 
  the perfect rhythm and two parallel lines 
  that are shifted in the y-axis by +E and -E 
  such that the two parallel lines contain all the points (i, Ti) 
  and also that E is the minimum possible value. 

  We would like to point out that instead of 
  trying to find the line with the perfect rhythm 
  which can be a hard problem due to the error 
  associated with each (i, Ti), 
  we can instead focus on the equivalent problem of finding the two parallel lines. 
  
  Note that each of these two lines must touch 
  at least one point otherwise 
  we can always reduce the distance between these two lines, 
  and further reduce the distance on y-axis to get a smaller error E.

  In fact, all candidate parallel lines touch (without intersecting) 
  the convex hull of the points (i, Ti). 
  Therefore we transform the problem to finding 
  the minimum distance between two parallel lines 
  in y-axis touching (without intersecting) the convex hull. 
  For example, the convex hull of the points in Figure 2 are shown in Figure 5.
  
  Figure 5

  Therefore to solve this problem, 
  we first compute the convex hull of the points (i, Ti). 
  Then we go through all the line segments on the boundary 
  of the convex hull and find the corresponding parallel line 
  on the opposite side of the convex hull 
  (see Figure 5 for some examples), 
  then get the y-distance between these two parallel lines, 
  and report the minimum among all such y-distances.

*/

/*void hull(const vector<int>& v, vector<int>& hx, vector<int>& hy, int mul) {
	for (int i = 0; i < v.size(); i++) {
		if (hx.size() >= 2) {
			int x1 = hx[hx.size()-2], x2 = hx[hx.size()-1], x3 = i;
			int y1 = hy[hy.size()-2], y2 = hy[hy.size()-1], y3 = v[i];
			if ((long long)(x2-x1)*(y3-y2)*mul >=
					(long long)(y2-y1)*(x3-x2)*mul) {
				hx.pop_back();
				hy.pop_back();
				i--;
				continue;
			}
		}
		hx.push_back(i);
		hy.push_back(v[i]);
	}
}*/


/*

 ARRAY points := {P1, P2, ..., PN};
 
 points.delete(middle vertices of any collinear sequence of three points);
  
 REAL p_a := index of vertex with minimum y-coordinate;
 REAL p_b := index of vertex with maximum y-coordinate;
 
 REAL rotated_angle := 0;
 REAL min_width := INFINITY;
 
 VECTOR caliper_a(1,0);    // Caliper A points along the positive x-axis
 VECTOR caliper_b(-1,0);   // Caliper B points along the negative x-axis
 
 WHILE rotated_angle < PI
   
   // Determine the angle between each caliper and the next adjacent edge in the polygon
   VECTOR edge_a(points[p_a + 1].x - points[p_a].x, points[p_a + 1].y - points[p_a].y);
   VECTOR edge_b(points[p_b + 1].x - points[p_b].x, points[p_b + 1].y - points[p_b].y);
   REAL angle_a := angle(edge_a, caliper_a);
   REAL angle_b := angle(edge_b, caliper_b);
   REAL width := 0;
   
   // Rotate the calipers by the smaller of these angles
   caliper_a.rotate(min(angle_a, angle_b));
   caliper_b.rotate(min(angle_a, angle_b));
   
   IF angle_a < angle_b
     p_a++;  // This index should wrap around to the beginning of the array once it hits the end
     width = caliper_a.distance(points[p_b]);
   ELSE
     p_b++;  // This index should wrap around to the beginning of the array once it hits the end
     width = caliper_b.distance(points[p_a]);
   END IF
   
   rotated_angle = rotated_angle + min(angle_a, angle_b);
   
   IF (width < min_width)
     min_width = width;
   
   END IF
 END WHILE
 
 RETURN min_width;
*/