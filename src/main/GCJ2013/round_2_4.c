/*


 Problem

 Two teams of players play pong. 
 Pong is a simple computer game, 
 where each player controls a paddle 
 (which we assume to be a point), 
 and a little ball bounces back and forth. 

 The players in one team are required 
 to bounce the ball in a fixed cyclic order 
 
 (so in a three-player team, 
  the first one to touch the ball would be P1, 
  then P2, 
  then P3 
  and only then P1 again), 

 until one of the players doesn't manage to bounce it, 
 at which point the ball leaves the playing field 
 and this player's team loses.

 To be more precise: 
 the playing field is a rectangle of size AxB. 
 On each vertical wall (of length A) 
 there are a number of paddles, 
 one for each player of the team guarding this wall. 
 
 Each paddle is a point. 
 All the paddles of the players 
 on one team move vertically 
 at the same speed (in units per second), 
 and can pass each other freely. 
 
 There is also a ball, 
 for which we are given its initial position 
 (horizontal and vertical, counted from the lower-left corner) 
 and initial speed (horizontal and vertical, again in units per second). 
 
 The players are allowed to choose the initial positioning 
 of their paddles on their vertical walls 
 knowing the initial position of the ball. 
 
 Whenever the ball reaches a horizontal wall, 
 it bounces off 
 (with the angle of incidence equal 
  to the angle of reflection). 
 
 Whenever it reaches a vertical end of the field, 
 if the paddle of the player 
 who is supposed to touch the ball now is there, 
 it bounces off, while if there isn't, 
 the team of the player whose paddle 
 was supposed to be there loses.

 The game can take quite a long time, 
 with the players bouncing the ball back and forth. 
 Your goal is to determine the final result 
 (assuming all players play optimally).



 Input


 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case consists of four lines. 
 The first line contains two integers, A and B, 
 describing the height and width of the playing field. 
 The second line contains two integers, N and M, 
 describing the sizes of the two teams: 
 N is the number of players on the team with paddles 
 on the X = 0 wall, 
 and M is the number of players on the team with paddles on X = B wall. 
 The third line contains two integers, V and W, 
 describing the speed of the paddles of players 
 in the first and second team, respectively. 
 
 The fourth line contains four integers: 
 Y, X, VY and VX, 
 describing the initial position (vertical and horizontal) 
 and initial speed of the ball 
 (the ball moves by VY units up and VX 
  to the right each second, until it bounces).


 Output


  For each test case, output one line containing 
  "Case #x: y", where x is the case number (starting from 1) 
  and y is one of the three possible outputs: 
  "DRAW"    (if the game can proceed forever), 
  "LEFT z", if the team with paddles on x = 0 wins, 
            and the opposing team can bounce the ball at most z times, or 
  "RIGHT z" if the team with paddles on X = B wins 
            and the opposing team can bounce the ball at most z times.

 Limits

 1 ≤ T ≤ 100.
 0 < X < B
 0 < Y < A

 Small dataset

 1 ≤ N, M ≤ 106
 1 ≤ V, W ≤ 1012
 -1012 ≤ VY ≤ 1012
 -106 ≤ VX ≤ 106
 2 ≤ A, B ≤ 106

 Large dataset

 1 ≤ N, M ≤ 10100
 1 ≤ V, W ≤ 10100
 -10100 ≤ VY, VX ≤ 10100
 2 ≤ A, B ≤ 10100

 
 The picture depicts the gameplay in the first sample case. 
 The ball bounces off the right wall at time 0.375 

 (the first RIGHT player intercepts it, 
  for instance by beginning with her paddle 
  there and not moving it), 
  
 then off the left wall at 0.875 
 (the LEFT player bounces it), 
 again on the right at time 1.375 
 (the second RIGHT player can position 
  his paddle at the bounce point), 
  again on the left 
  (where the LEFT player gets just in time to catch it 
  — she covers the three units of distance 
  exactly in one second in which she needs to get there) 
  and then hits the right wall too far 
  for the first RIGHT player to get there. 
  Note the second RIGHT player could catch the ball, 
  but is not allowed by the rules to do so. 
  
  Also note that if RIGHT team had one player more, 
  she could bounce the ball, 
  and then LEFT would lose 
  — the ball would come too far up 
  for the single LEFT player to get there in time.





 Preliminaries

We hope you took our Fair Warning, which we fairly repeated this year — we consider Big Integers to be fair game, so if your language doesn't natively support them, you'd better have a library to handle them ready.

This problem seems to be about fractions initially, since the ball can hit the walls at fractional positions. There's a way to move it to integers (man, Big Fractions would be too much). The way to go about it is to scale things. Let's scale the time, so now there are VX units to a second, and scale vertical distances, so that there are VX units to the old unit. This means the vertical speeds stay the same, horizontal speeds get VX times smaller, horizontal distances stay the same, and vertical distances grow VX times larger. In implementation terms, this means we shrink VX to 1 and grow A times VX — and now suddenly the ball moves a integral number of units upwards and one unit to the side in each unit of time (and so will hit the vertical walls in integral moments of time).

The above assumes VX is positive. If it's zero, then the ball will never hit the walls, and so the game ends in a draw. If it's negative, we can flip the whole board across the Y axis and swap the teams to make VX change signs. Similarly, we can assume VY positive — if it's zero, putting all paddles in the single impact point will guarantee a draw, and if it's negative, we can flip the board vertically.

So now the ball will hit a given vertical wall every 2B units of time. It's also relatively easy to figure out what is the position of the impact. If there were no vertical walls, the ball would hit at the initial hit position (Y + (B - X) VY), and then at 2BVY intervals from there. To calculate the positions in real life, notice that every 2A upwards the ball is in the same position again (so we can take impact points module 2A), and if that number is larger than A, the ball goes in the other direction, and the impact position is 2A minus whatever we calculated.

Many bounces

So now we know how to calculate hit positions, so we can just simulate and see who loses first, right? Well, wrong, because the ball can bounce very many times. Even in the small dataset, the number of bounces can go up to 1011, too much for simulation.

Notice that the positions of the paddles are pretty much predetermined. The paddle of a given player has to be exactly at the point of impact when it is the player's turn to bounce it, and then the only question is whether the player will have enough time to reach the next point of impact before the ball. With N paddles on a team, and V being the speed of the paddle, the player can move the paddle by 2BVN before the next impact. The ball, in the same time, will move by 2BVYN, but while the paddle moves in absolute numbers, the ball moves "modulo 2A with wrapping", as described above.

If the distance the ball moves (modulo A) is smaller or equal to the distance the paddle moves, the paddle will always be there in time. The interesting case is when it is not so. In this case, there is still a chance for the paddle to be in the right place on time due to the "wrapping effect". For instance, if the ball moves by A+1 with each bounce, and the first bounce happens at A / 2, the next one will happen at A / 2 - 1, the next one at A / 2 + 2, and so on — so the initial bounces will be pretty close by. We can calculate exactly the set of positions where the ball hitting the wall will allow the paddle to catch up, and it turns out that it is two intervals modulo 2A.

So now the question becomes "how long can the ball bounce without hitting a prohibited set of two intervals", which is easy to reduce to "without hitting the given interval". This is a pure number-theoretic question: we have an arithmetic sequence I + KS, modulo 2A, and we are interested in the first element of this sequence to fall into a given interval.

Euclid strikes again

There is a number of approaches one can take to solving this problem. We will take an approach similar to the Euclidean algorithm. First, we can shift everything modulo 2A so that I is zero, and we are dealing with the sequence KS. Also, we can shift the problem so that S ≤ A — if not, we can (once again) flip the problem vertically. Thus, the ball will bounce at least twice before wrapping around the edge of 2A.

We can calculate when is the first time the ball will pass the beginning of the forbidden interval (by integral division). If at this point the ball hits the forbidden interval, we are done. Otherwise, the ball will travel all the way to 2A, and then wrap around and hit the wall again at some position P smaller than S. Notice that in this case the interval obviously is shorter than S.

Now, the crucial question is what P is. It's relatively easy to calculate for what values of P will the next iteration land in the interval (if the interval is [kS + a, kS + b] for some k, a, b, then the interesting set of values of P is [a, b]). If P happens to be in this interval, we can again calculate the answer fast. If not, however, we will do another cycle, and then hit the wall (after cycling) at the point 2P, mod S. Notice that this is very similar to the original problem! We operate modulo S, we increment by P with each iteration, and we are interested in when we enter the interval [a,b].

Thus, we can apply recursion to learn after how many cycles will we finally be in a position to fall into the original interval we were interested in. So we make that many full cycles, and then we finish with a part of the last cycle to finally hit the interval.

Complexity analysis

All the numbers we will be operating on will have at most D = 200 digits in the large dataset, so operations on them will take at most O(D2 logD) time (assuming a quadratic multiplication implementation and a binary-search implementation of division). Each recursion step involves a constant number of arithmetic operations plus a recursive call, for which the number A (the modulo) decreases at least twice. This means we make at most O(D) recursion steps — so the whole algorithm will run in O(D3 logD) time, fast enough allowing even some room for inefficiency.





*/