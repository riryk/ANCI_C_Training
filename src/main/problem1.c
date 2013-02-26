
/*
Problem1

Description
 
You are playing Zombie Smash: 
a game where the objective is to smash zombies with your trusty Zombie Smasher 
as they pop out of graves at the graveyard. 

The graveyard is represented by a flat 2D grid. 
Each zombie will pop out of a grave at some (X, Y) cell on the grid, 
stand in place for 1000 milliseconds (ms), 
and then disappear back into the grave. 
At most one zombie can stand around a grave at a time.
 
You can move to any one of the 8 cells adjacent to your location in 100ms; 
i.e., you can move North, East, South, West, NW, NE, SW, and SE of your current location.
You may move through or stand on a cell even if it is currently occupied by a zombie. 

You can smash a zombie instantly once you reach the cell that the zombie is standing on,
but once you smash a zombie it takes 750ms for your 
Zombie Smasher to recharge before you can smash another zombie. 

You may move around while Zombie Smasher is recharging. 

For example, immediately after smashing a zombie at (0, 0):
  •  It will take 750ms to reach and smash a zombie at (1, 1) or 
  •  2000ms to reach and smash a zombie at (20, 20). 


You start at cell (0, 0) at the beginning of the game (time=0).
After you play a level you would like to know how many zombies you could have smashed,
if you had played optimally.



Input
 
The first line will contain a single integer T, the number of test cases. 
It is followed by T test cases, 
each starting with a line containing a single integer Z, the number of zombies in the level.
 
The next Z lines contain 3 space-separated integers each, 
representing the location and time at which a given zombie will appear and disappear. 

The ith line will contain the integers Xi, Yi and Mi, where:

 • Xi is the X coordinate of the cell at which zombie i appears, 
 • Yi is the Y coordinate of the cell at which zombie i appears, 
 • Mi is the time at which zombie i appears, in milliseconds after the beginning of the game. 
 
The time interval during which the zombie can smashed is inclusive: 
if you reach the cell at any time in the range [Mi, Mi + 1000] with a charged Zombie Smasher, 
you can smash the zombie in that cell. 


Output
 
For each test case, output one line containing "Case #c: d", 
where c is the case number (starting from 1), 
and d is the maximum number of zombies you could have smashed in this level.
 
Limits
 
 1 ≤ T ≤ 100.
 -1000 ≤ Xi, Yi ≤ 1000.
 0 ≤ Mi ≤ 100000000 = 108.

 Two zombies will never be in the same location at the same time. 
 In other words, if one zombie appears at (x, y) at time t, 
 then any other zombie that appears at (x, y) 
 must appear at or before (t - 1001), or at or after (t + 1001).
 
Small dataset
 
1 ≤ Z ≤ 8.
 
Large dataset
 
1 ≤ Z ≤ 100.
 
Sample
 
 3

 4

 1 0 0
 -1 0 0
 10 10 1000
 10 -10 1000

 3

 1 1 0
 2 2 0
 3 3 0

 5

 10 10 1000
 -10 10 1000
 10 -10 1000
 -10 -10 1000
 20 20 2000
 

*/





/*
  Possible mathematical solution

  Firstly, it is worth noting that the small dataset with only 8 zombies 
  can be solved simply by evaluating each possible permutation for the order in which 
  to smash zombies and keeping the best one. 
  
  For each permutation, simply attempt to smash the zombies in the order given, 
  skipping any zombies that cannot be reached on time. 
  This simple approach has exponential time complexity and clearly will not scale for the large data set.


  For the more efficient approach, 
  let us start by considering the game state represented in an inefficient way 
  and see how it can be made more efficient. 
  
  We can represent the game state at any point in time with the following tuple:

  • (Time, Location, Set of zombies already smashed) 

  This certainly works - given this state, we essentially have the snapshot of the game 
  at any point in time, but this is very verbose. 
  
  Since we want to smash as many zombies as possible, 
  we want to smash zombies as soon as possible, 
  i.e. we want to arrive at the grave they pop out of as soon as possible. 
  
  With that in mind, we can make the following assumption: 

  we will arrive at the grave of the next zombie as soon as possible, 
  and potentially stand around waiting until that zombie can be smashed. 
  
  Once smashed, we will move to the next grave as quickly as we can and repeat the process. 
  That way, it becomes unnecessary to keep track of any states 
  where we are transiting between two graves 
  because those states can be derived from the states at the origin and destination graves. 
  
  We can change the state to be: 

  • (Time last zombie was smashed, Last zombie smashed, Set of zombies already smashed)

  This is an improvement, 
  but now the problem is that we keep track of all possible sets of zombies already smashed. 
  
  Let’s see what we can do about that.
 
  Consider the state (T1, Z1, {Z1 …}) 
  where we have just smashed zombie Z1 at time T1. 
  
  The set of zombies already smashed contains Z1, 
  and possibly a bunch of other zombies. 
  
  Suppose that Z0 is a zombie that we have smashed earlier. 
  
  There are two cases: 
  either Z0 appears at an interval overlapping with Z1, 
  or it has already appeared before Z1.

  1. If Z0 has already appeared before Z1 
     then by T1 it can no longer be at its grave 
	 (even if we haven’t smashed it) 
	 and explicitly tracking that it has already been smashed is unnecessary.

  2. Otherwise, if Z0 appears in an interval overlapping with Z1, 
     is it possible that we will attempt to smash Z0 again, if we don’t keep track of it? 
	 
	 Suppose that Z0 was smashed at T0, 
	 since the Zombie Smasher needs to recharge twice, 
	 Z0 will be already gone because it stands around for 1000ms 
	 and it takes 1500ms for the Zombie Smasher to recharge twice. 

	 Again, it is not necessary to explicitly track the set of zombies smashed 
	 to avoid smashing the same zombie twice.

  In light of the above observations we can simplify the state to be: 

  •(Time last zombie was smashed, Last zombie smashed, Number of zombies already smashed)

  It is easy to see that we prefer earlier times over later times for smashing a zombie 
  - the sooner we smash a zombie, the sooner we can move on to the next one, 
  so we are only interested in states with minimal time possible. 
  
  Let us model the state transitions as a graph and minimize on time.
 
  The game starts at time 0 and location (0, 0). 
  Based on this information, 
  we can generate the initial frontier of zombies that can be reached and smashed on time. 
  
  Given this frontier, the times and the locations at which zombies will appear, 
  we can apply a modified Dijkstra’s Algorithm to find the set of game states that are reachable. 
  
  Once we know those, we can simply return the maximum number of zombies smashed in a reachable state. 
  Here is the pseudo-code:
 
 generateStates():
  states = {}
  states.Add(0, nil, 0) // Include the initial state.
  for each zombie z:
    for zombies_killed from 1 to Z:
      // For other reachable states this will be revised later.
      earliest_smash_time = infinity
      if zombiles_killed = 1:
        earliest_arrival_time = dist((0, 0), z)
        if earliest_arrival_time ≤ z.appearance + 1000:
          earliest_smash_time = max(z.appearance,
                                    earliest_arrival_time)
      states.Add(earliest_smash_time, z, zombies_killed)
  return states


*/

/* This is the time while a corpse is standing before he lie down into a grave
 * It can be illustrated:
 * ........ is in the grave .....|... out of the grave ...| .. is in a grave 
 *                              t0                        t1
 * t1 - t0 = const = CORPSE_STAND_DURATION
 * There is a restriction that only one zombie can stand in time. The process can be illusrtated
 * .... means that corpse is in grave
 * #### means he is standing around a grave
 * Z1 .......|########|..........................
 * Z2 ..............|########|...................
 * Z3 ...................|########|..............
 *   t0      t1     t2  t3   t4   t5
 */
#define CORPSE_STAND_DURATION 1000

/* Speed, with which an alive person can move through a graveyard's cell
 * Example:  North, East, South, West, NW, NE, SW, and SE 
 *
 *  1 --- 2 --- 3   From position 5 we can move tp every adjacent position for the 
 *  |   N |     |   time SPEED
 *  |_W___|___E_| 
 *  4     5     6
 *  |   S |     |
 *  7_____8_____9
 */
#define SPEED 100
/* The time to recharge a rifle before smashing another zombi 
 * Example:   to reach two different edges of rectangle [6 X 2] we 
 * |_| | | | |  need: min {6, 2} * speed + (max {6, 2} - min {6, 2}) * speed
 * |_|_|_|_|_|  = (min {6, 2} + max {6, 2} - min {6, 2}) * speed
 * 1 2 3 4 5 6  = max {6, 2} * speed
 * Let's analyze and example:
 * 10 10 1000
 * -10 10 1000
 * 10 -10 1000
 * -10 -10 1000
 * 20 20 2000
 *           |
 *           |   .20
 *      -10. | .10   
 * ----------|-------------------
 *      -10. | .-10
 *           |
 */
#define RECHARGE_TIME 750
/* If we are at point [0, 0] we can build all possible points we could reach
 * For example: (x0,y0,t0) Set = (xi,yi,ti) i = 1,..,n - the set of all available 
 * corpses. We can compute time we could reach Ti
 * Ti = t0 + max {xi - x0, yi - y0} * Speed <= ti + StandDuration
 * and choose a restriction: Ti <= ti + StandDuration.
 * 
 */