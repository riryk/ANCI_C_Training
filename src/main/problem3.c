/*
Long ago, an alien civilization built a giant monument. 
The floor of the monument looked like this:
 ###############
#.............#
#.###########.#
#.#.........#.#
#.#.#######.#.#
#.#.#.....#.#.#
#.#.#.###.#.#.#
#.#.#.#.#.#.#.#
#.#.#.###.#.#.#
#.#.#.....#.#.#
#.#.#######.#.#
#.#.........#.#
#.###########.#
#.............#
###############

Each '#' represents a red tile, and each '.' represents a blue tile. 
The pattern went on for miles and miles 
(you may, for the purposes of the problem, assume it was infinite). 

Today, only a few of the tiles remain.
The rest have been damaged by methane rain and dust storms. 
Given the locations and colours of the remaining tiles, 
can you find the center of the pattern?
 
Input
 
The first line of the input gives the number of test cases, T. 
T test cases follow. 

Each one starts with a line containing N, 
the number of remaining tiles. 

The next N lines each contain Xi, Yi, 
and the tile colour (either '#' or '.').


Output
 
For each test case, output one line containing "Case #c: X Y", 
where c is the case number (starting from 1) 
and (X, Y) is the location of the center of the pattern. 

If there is more than one possible answer, 
output the (X, Y) closest to (0, 0) in Manhattan distance 
(the distance in x, plus the distance in y). 

If there are still ties, output the one with the largest X. 
If there are still ties after that, 
output the one with the largest Y. 

If there is no possible answer, output "Case #c: Too damaged".
 
Limits
 
1 ≤ T ≤ 50.
 The list of coordinates in each test case will not contain duplicates. 

Small dataset
 
1 ≤ N ≤ 100.
 -100 ≤ Xi ≤ 100.
 -100 ≤ Yi ≤ 100. 

Large dataset
 
1 ≤ N ≤ 1000.
 -1015 ≤ Xi ≤ 1015.
 -1015 ≤ Yi ≤ 1015. 

Sample

Input 
Output 

6
 1
 0 0 .
 1
 0 0 #
 3
 0 0 #
 0 1 #
 1 0 #
 5
 50 30 #
 49 30 #
 49 31 #
 49 32 #
 50 32 #
 2
 -98 0 #
 99 50 .
 4
 88 88 .
 88 89 .
 89 88 .
 89 89 .
 

Case #1: 0 0
 Case #2: 1 0
 Case #3: 1 1
 Case #4: 50 31
 Case #5: 1 0
 Case #6: Too damaged
 

Solution:

To begin with, let's try to formalize the rules of forming the pattern of tiles. 

.    - Blue
#, + - Red

If the center is at some position x, y, 
then the red tiles are those in positions x', y' 
for which the number max(|x - x'|, |y - y'|) is odd, 
while the blue tiles are those for which this number is even. 

This is because the formula max(|x - x'|, |y - y'|) = C for any C 
describes a square ring around x, y, and the rings alternate color with the parity of C

 For the small problem, we can prove that if there exists a solution, there exists one with |X| + |Y| < 202. Thus, we can check all candidates for the center, for each one check whether all the tiles have the right colors, and output the best candidate. This will not run in time for the large data, of course, as we will have over 1030 candidate centers to check.


My own comments:
 . . . .
 + + + .   +  max(|x-x0|, |y-y0|) is odd
 + . + .   .  max(|x-x0|, |y-y0|) is even
 + + + .
*/