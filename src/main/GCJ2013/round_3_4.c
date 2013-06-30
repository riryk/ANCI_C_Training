
/*


Problem

An observation wheel consists of N passenger gondolas arranged in a circle, 
which is slowly rotating. 

Gondolas pass the entrance one by one, 
and when a gondola passes the entrance, 
a person may enter that gondola.


In this problem, the gondolas are so small 
that they can take just one person each, 
so if the gondola passing by the entrance is already occupied, 
the person waiting at the entrance 
will have to wait for the next one to arrive. 

If that gondola is also occupied, 
the person will have to wait for the next one after that, 
and so on, until a free gondola arrives. 

For simplicity, we will not consider people exiting 
the gondolas in this problem 
— let's assume that all people do 
is enter the gondolas, 
and then rotate with the wheel for an arbitrarily long time.

We want to make sure people are not disappointed 
because of long waiting times, 
and so we have introduced a flexible pricing scheme: 

when a person approaches the wheel, 
and the first gondola passing by the entrance is free, 
she pays N dollars for the ride. 

If the first gondola is occupied and she has to wait for the second one, 
she pays N-1 dollars for the ride. 

If the first two gondolas are occupied 
and she has to wait for the third one, 
she pays N-2 dollars for the ride. 

Generally, if she has to wait for K occupied gondolas to pass by, 
she pays N-K dollars. 

In the worst case, when she has to wait for all but one gondola to pass, 
she will pay just 1 dollar.

Let's assume that people approach our wheel at random moments in time, 
so for each person approaching the wheel, 
the first gondola to pass the entrance is picked uniformly and independently. 

Let's also assume that nobody will come to the wheel 
while there's already at least one person waiting to enter, 
so that we don't have to deal with queueing. 

A person will always take the first free gondola that passes the entrance.

You are given the number of gondolas and which gondolas are already occupied. 
How much money are we going to make, on average, until all gondolas become occupied?


Input


The first line of the input gives the number of test cases, T. 
T lines follow. 
Each line describes one test case and contains only '.' (dot) 
or 'X' (capital letter X) characters. 
The number of characters in this line gives you N. 
The i-th character is 'X' 
when the i-th gondola is already occupied, 
and '.' when it's still free. 
The gondolas are numbered in the order 
they pass the entrance, 
so the 1st gondola is followed by the 2nd gondola, 
and so on, starting over 
from the beginning after the last gondola passes.


Output

For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the average amount of money we will get, in dollars. 
Answers with absolute or relative error no larger than 10-9 will be accepted. 
See the FAQ for an explanation of what that means, 
and what formats of floating-point numbers we accept.

Limits

1 ≤ T ≤ 50.

Small dataset

1 ≤ N ≤ 20.

Large dataset

1 ≤ N ≤ 200.

Sample


Input 
 	
Output 
 
5
.X.
X.X.
.XX.
X..XX.
.XX..X
Case #1: 4.66666666666667
Case #2: 6.00000000000000
Case #3: 5.75000000000000
Case #4: 13.4722222222222
Case #5: 13.5277777777778

Notes

Here's how the first example works. There are nine possibilities, each with probability 1/9:

The first person comes. If the next gondola to pass the entrance is:

The 1st gondola, which is free, the first person enters it and pays 3 dollars. 
   Then, some time later, the second person comes. 
   If the next gondola to pass the entrance is:
     The 1st gondola, which is occupied, 
	 and so is the 2nd gondola, 
	 the second person has to wait until the 3rd gondola, 
	 and thus she pays just 1 dollar before entering it. 
	 In total, we've earned 4 dollars.

     The 2nd gondola, which is occupied, 
	 the second person has to skip it 
	 and enter the 3rd gondola and thus pays 2 dollars. 
	 In total, we've earned 5 dollars.

     The 3rd gondola, which is free, 
	 so the second person pays 3 dollars. 
	 In total, we've earned 6 dollars.

The 2nd gondola, which is occupied, 
the first person has to skip it 
and enter the 3rd gondola, paying 2 dollars. 

Then, some time later, the second person comes. 
If the next gondola to pass the entrance is:

   The 1st gondola, which is free, 
   the second person pays 3 dollars. 
   In total, we've earned 5 dollars.

   The 2nd gondola, which is occupied (as is the 3rd gondola), 
   the second person has to wait until the 1st gondola, 
   and thus she pays just 1 dollar before entering it. 
   In total, we've earned 3 dollars.

   The 3rd gondola, which is occupied, 
   the second person has to skip it 
   and enter the 1st gondola and thus pays 2 dollars. 
   In total, we've earned 4 dollars.

The 3rd gondola, which is free, 
the first person enters it and pays 3 dollars. 
Then, some time later, the second person comes. 
If the next gondola to pass the entrance is:

    The 1st gondola, which is free, 
	the second person pays 3 dollars. 
	In total, we've earned 6 dollars.

    The 2nd gondola, which is occupied (as is the 3rd gondola), 
	the second person has to wait until the 1st gondola, 
	and thus she pays just 1 dollar before entering it. 
	In total, we've earned 4 dollars.

    The 3rd gondola, which is occupied, 
	the second person has to skip it 
	and enter the 1st gondola and thus pays 2 dollars. 
	In total, we've earned 5 dollars.

We have nine possibilities, 
earning 3 dollars in one of them, 
4 dollars in three of them, 
5 dollars in three of them, 
and 6 dollars in two of them. 
On average, we earn (1*3+3*4+3*5+2*6)/9=42/9=4.6666666666... dollars.

*/