/*
 
 Problem

 I have a really long password, 
 and sometimes I make a mistake when I type it. 
 Right now I've typed part of my password, 
 but I might have made some mistakes. 
 
 In particular, I might have pressed the wrong key 
 while typing one or more of the previous characters. 
 Given how likely I was to get each character right, what should I do?

 I have three options:

 Finish typing the password, then press "enter". 
 I know I'll type the rest of the characters perfectly. 
 
 If it turns out that one of the earlier characters was wrong, 
 I'll have to retype the whole thing and hit "enter" again 
 -- but I know I'll get it right the second time.

 Hit "backspace" some number of times, 
 deleting the last character(s) I typed, 
 and then complete the password and press "enter" as in option 1. 
 
 If one of the characters I didn't delete was wrong, 
 I'll have to retype the whole thing and press "enter", 
 knowing I'll get it right the second time.

 Give up by pressing "enter", retyping the password from the start, 
 and pressing "enter" again. I know I'll get it right this time.

 I want to minimize the expected number of keystrokes needed. 
 Each character in the password costs 1 keystroke; 
 each "backspace" costs 1 keystroke; 
 pressing "enter" to complete an attempt or to give up costs 1 keystroke.

 Note: 

 The "expected" number of keystrokes is the average number of keystrokes 
 that would be needed if the same situation occurred a very large number of times. 
 See the example below.

 Example

 Suppose my password is "guest" and 
 I have already typed the first two characters, 
 but I had a 40% chance of making a mistake when typing each of them. 
 Then there are four cases:

 I typed "gu" without error. This occurs with probability 0.6 * 0.6 = 0.36.

 I typed the 'g' correctly but I made a mistake typing the 'u'. 
 Then I have two letters typed still, 
 but the second one is wrong: "gX". 
 (Here, the 'X' character represents a mistyped letter.) 
 This occurs with probability 0.6 * 0.4 = 0.24.

 I typed the 'u' correctly but I made a mistake typing the 'g': "Xu". 
 This occurs with probability 0.4 * 0.6 = 0.24.

 I made a mistake typing both letters, so I have two incorrect letters: "XX". 
 This occurs with probability 0.4 * 0.4 = 0.16.

 I don't know how many mistakes I actually made, but for any strategy, 
 I can calculate the expected number of keys required to use it. This is shown in the table below:

                                         "gu"   	"gX"   	"Xu"   	"XX"   	Expected
 Probability	                         0.36	    0.24	0.24	0.16	  -
 Keystrokes if I keep typing	          4	        10	     10	     10	     7.84
 Keystrokes if I press backspace once	  6	        6	     12	     12	     8.4
 Keystrokes if I press backspace twice    8      	8	     8	     8	     8
 Keystrokes if I press enter right away	  7	        7	     7	     7	     7

 If I keep typing, then there is an 0.36 probability 
 that I will need 4 keystrokes, 
 and an 0.64 probability that I will need 10 keystrokes. 
 
 If I repeated the trial many times, 
 then I would use 4 keystrokes 36% of the time, 
 and 10 keystrokes the remaining 64% of the time, 
 so the average number of keystrokes needed would be 0.36 * 4 + 0.64 * 10 = 7.84. 
 In this case however, it is better to just press enter right away, which requires 7 keystrokes.

 Input

 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case begins with a line containing two integers, A and B. 
 A is the number of characters that I have already typed, 
 and B is the total number of characters in my password.

 This is followed by a line containing A real numbers: p1, p2, ..., pA. 
 pi represents the probability that I correctly typed the ith letter in my password. 
 These real numbers will consist of decimal digits and at most one decimal point. 
 The decimal point will never be the first or the last character in a number.

 Output

 For each test case, output one line containing "Case #x: y", 
 where x is the case number (starting from 1) 
 and y is the expected number of additional keystrokes I need, 
 not counting the letters I have typed so far, 
 and assuming I choose the optimal strategy. 
 y must be correct to within an absolute or relative error of 10-6.

 Limits

 1 ≤ T ≤ 20.
 0 ≤ pi ≤ 1 for all i.

 Small dataset

 1 ≤ A ≤ 3.
 A < B ≤ 100.

 Large dataset

 1 ≤ A ≤ 99999.
 A < B ≤ 100000.

 Sample


 Input 
 	
 Output 
 
	3
	2 5
	0.6 0.6
	1 20
	1
	3 4
	1 0.9 0.1
	Case #1: 7.000000
	Case #2: 20.000000
	Case #3: 4.500000








   The first challenge with Password Problem 
   is wrapping your head around expected values. 
   
   These things come up all the time and are extremely useful, 
   so they are well worth learning. 
   
   (For example, expected value is key to solving our last problem on the 2011 finals!)

   Once you understand what the problem is asking, 
   you mainly need to evaluate the expected number of keystrokes for each strategy:


   Strategy 1: 


   Finish typing the password, then press "enter".

   The probability that this works on the first try is 
   x = p1 * p2 * ... * pA. 
   
   A - is the total number of characters I have already typed

   In this case, you need B - A + 1 keystrokes. 
   
   In the other case, you need 2B - A + 2 keystrokes. B - A + 1 + (B + 1)
   
   Therefore, the expected number of keystrokes for this strategy is: 

   B - A + 1 + (B + 1) * (1 - x). 

   1 - x  is the probability of failure.


   Strategy 2: 

   
   Hit backspace k times, then proceed as in Strategy 1.

   The probability that this works on the first try is 
   xk = p1 * p2 * ... * pA-k.  - the first A-k symbols I must have typed correctly.
   
   In this case, you need B - A + 2k + 1 keystrokes. 
   (B - A + 1) + k + k.   press k times backspaces and after that return back
   
   In the other case, you need 2B - A + 2k + 2 keystrokes. 

   (B - A + 1) + 2k + (B + 1)

   Therefore, the expected number of keystrokes for this strategy is 

   (B - A + 1) + 2k + 1 + (B + 1) * (1 - xk). 


   Strategy 3: 
   
   Press enter immediately, and retype the whole password.

   This always takes B + 2 keystrokes.

   The problem is asking you to calculate the minimum of all these values. 
   There is one more catch though: 

   if you compute each xi from scratch, 
   your program will probably be too slow. 
   
   It might take 99999 multiplications to calculate x, 
   99998 multiplications to calculate x1,
   99997 multiplications to calculate x2, and so on. 
   
   Instead, you should calculate them all together:

   xA = 1
   xA-1 = xA * p1
   xA-2 = xA-1 * p2
   etc.

   Making sure your solution is fast enough to complete in the time limit 
   is an important part of the Google Code Jam!

   Here is a short Python solution:

   import sys

   for tc in xrange(1, int(sys.stdin.readline())+1):
      A, B = [int(w) for w in sys.stdin.readline().split()]
      p = [float(w) for w in sys.stdin.readline().split()]
      best, x = B + 2.0, 1
      for i in xrange(A):
         x *= p[i]
         best = min(best, (B - i) + (A - i - 1) + (B + 1) * (1 - x))
         print 'Case #%d: %f' % (tc, best)

   The Large Input and Underflow

   There is one trick on this problem that caught quite a few contestants, 
   and that involves a subtlety with how floating point numbers work.
   Let's suppose you calculate xi in a slightly different way:

   x = p1 * p2 * ... * pA
   x1 = x / pA
   x2 = x1 / pA-1
   etc.

   At first glance, this looks completely equivalent to the solution above. 
   Unfortunately, it is wrong to do this for two reasons. 
   
   First, if one of the pi values is 0, 
   you are in trouble. 
   
   Second, it turns out that even a 64-bit floating point number reserves only 11 bits for the exponent. 
   This means that it cannot store values much less than 2-1000. 
   These very small values get rounded down to 0. 
   
   Normally, you wouldn't care about values this small, 
   but if you are multiplying 100,000 probabilities together, 
   it becomes an issue. 
   
   After rounding to 0, you will end up with every x value, 
   including xA, being 0, 
   and then you are in trouble!

   Some people reported failing tests due to this bug, 
   and were then able to fix it by switching to the "long double" data type in C++. 
   However, they were lucky! 
   
   Even long double has the same issues.

   If you ever need to combine a lot of multiplications and divisions in the future, 
   where intermediate values might get very small, it helps to work in log space:

   A * B / C = exp( log(A) + log(B) - log(C) )

   Do you see why this approach avoids the problem?


 */


pr21Solve() 
{
  double p[100001];
  int T;
  int A; 
  int B; 
  int prob = 1;

  // Read T
  int i;
  int j;

  T = 12;

  for (j = 0; j < T; j++) 
  {
    double result;
	double probability;
	double estimatedNum;
	// Read A and B
    // cin >> A >> B;
    A = 12;
	B = 10;

    for (i = 0; i < A; i++) 
	{
		// cin >> p[i];
		// Read probabilities
        p[i] = 0.122;
	}

    result = 2 + B; //Give up from the start, press enter and type the password 
	                //again from the start
    probability = 1.0; 

	// i means how many times we need to press backspace
    for (i = 0; i <= A; i++) 
	{
        estimatedNum = probability * (A - i + B - i + 1) +
			(1 - probability) * (A - i + B - i + 1 + B + 1);

		result = result < estimatedNum ? result : estimatedNum;

        probability *= p[i];
    }
  }

}

