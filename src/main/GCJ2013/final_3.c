
/*
 
 Problem

 Fair King Tyrone and his four sons conquered the nation of Carrania. 
 His four sons immediately started to squabble about 
 dividing the land between the four of them. 
 
 The main point of contention was the gold mines of Carrania 
 - each son wanted to have no fewer gold mines than any other.

 Fair King Tyrone soon got tired of the squabbling, 
 especially when he learned the number of mines is 4N, 
 so dividing them should be easy. 
 
 He gathered his sons, took a map, drew an X on it 
 and declared each son would get one quarter of the nation, 
 with borders defined by the X he drew.

 Unfortunately, Fair King Tyrone is a bit shortsighted, 
 and the map he drew on was not a map of Carrania. 
 His first minister quickly hid the map, 
 and now tries to draw an identical X on the map of Carrania 
 so that each son gets the same number of gold mines. 
 
 Unfortunately all sons saw King Tyrone draw the X, 
 and know the borders should be two perpendicular straight lines 
 - so the minister has to make them so.

 Help him! 
 Your task is to draw two perpendicular straight lines 
 such that no gold mine lies on a border, 
 and the borders divide the gold mines equally.


 Input


 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case begins with a number N, 
 describing the number of gold mines each son should get. 
 
 4N lines follow, each containing two integers, 
 being the coordinates xi, yi of one of the gold mines. 
 No three gold mines are co-linear.


 Output


 For each test case, output one line containing "Case #x: xa ya xb yb", 
 where x is the case number (starting from 1), 
 (xa, ya) are the coordinates of the point 
 where the two borders intersect, 
 and (xb, yb) are the coordinates of some other point on the X.

 All coordinates must be between -109 and 109, 
 have at most 9 digits after the decimal point, 
 and not use exponential notation. 
 
 They must be exact: 
 the resulting X will be drawn exactly at these coordinates. 
 You should output IMPOSSIBLE instead if there is no good placement of borders.



 Median choice

 Since the four quarters formed 
 by the two lines have to contain 
 the same number of mines, 
 each of the two lines has to split the set in half. 
 
 Thus, if we know the inclination of the two lines 
 (given as, e.g., the directed angle the first line forms with the horizontal axis), 
 we can position the each line in any place 
 such that it splits the points in half 
 
 (by, for instance, sorting the points by the value 
  of the cross-product with the direction of the line). 
 
 Let's begin by choosing any angle alpha as our initial angle, 
 and draw the two lines according to the procedure above.

 
 Suppose that the first quarter contains X points. 
 The second quarter contains 2N - X, 
 since there have to be 2N points above the red line. 
 
 The third quarter will again contain X points 
 (because there are 2N points to the right of the green line), 
 and the fourth will contain 2N - X. 
 
 So if X = N, our two lines are a correct solution. 
 This doesn't need to be the case, 
 though, as we can see on the figure above.


 Rotating the lines


 If the angle alpha we chose happens not to be a correct solution to the problem, 
 we will try rotating the lines (by increasing alpha) until it becomes valid.

 Let's consider what happens when we rotate the lines. 
 At some moment, one or both of the lines will rotate to a point 
 where instead of splitting the set neatly in half, 
 the "splitting line" passes through two mines, 
 
 with 2N-1 mines on either side 
 (note that we are taking advantage of the fact 
 that no three points are collinear, 
 so we know the line passes through exactly two points, and not, say, four). 
 
 We will call the moment at which there are two points 
 on at least one of the splitting lines a "discontinuity". 
 
 After we rotate a tiny bit more, the lines split the set neatly again.



 Discontinuity


 Let's analyze what happened after the discontinuity. 
 Obviously, only the points that were on the dividing lines 
 could have changed quarters at the discontinuity. 
 
 One of the points that were on the red line 
 crosses from one of quarters (1, 2) to one of quarters (4, 3), 
 
 and the other point crosses in the other direction. 
 Thus, if the discontinuity had points on one line only, 
 X changes by at most one.

 We will see that even if there were points on both lines at the discontinuity, 
 X will still change only by one. 
 The points on the red line go from quarters (1, 4) to quarters (2, 3). 
 
 So, for X to, say, grow by two, 
 we would have to have a point from 2 go to 3 and point from 4 go to 1 on the red line, 
 while a point from 4 went to 3 and a point from 2 went to 1 on the green line. 

 However, the red line 
 (and the green line as well, 
  but for now it's the red line that matters) 
  is rotating clockwise 
  
  - thus, it's the more leftward point that will go down, 
  and the more rightward point that will go up 
  — so the situation described above is impossible.


 */