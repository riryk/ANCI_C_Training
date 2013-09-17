
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
  in the y-axis by +E1 and -E1, denoted as L+E1 and L-E1 (see Figure 3), you will notice that the region between the two lines contains all the points (i, Ti); this example fits the definition of the problem which is a drum rhythm where each strike differs by at most E from some perfect rhythm. On the other hand, if we choose E5 as the error candidate, the region between the two lines L+E5 and L-E5 will not contain all the points (i, Ti) (see Figure 4).

*/

