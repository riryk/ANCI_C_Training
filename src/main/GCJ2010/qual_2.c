

/*

Problem


On our planet, Jamcode IX, 
three Great Events occurred. 
They happened 26000, 11000 and 6000 slarboseconds ago. 

In 4000 slarboseconds, 
the amount of time since all of those events 
will be multiples of 5000 slarboseconds, 
the largest possible amount... 

and the apocalypse will come.

Luckily for you, you live on Jamcode X! 
The apocalypse came on Jamcode IX less than a year ago. 
But Jamcode X has a worrying prophecy: 

"After the moment of reckoning, 
on the first optimum anniversary of the N Great Events, 
the apocalypse will come. 
64 bits will not save you. You have been warned."

The people of Jamcode X are very concerned by this prophecy. 
All of the Great Events have already happened, 
and their times have been measured to the nearest slarbosecond; 

but nobody knows when their optimum anniversary will occur. 
After studying the diary of a scientist from Jamcode IX, 
scientists working on the problem have come up with a theory:

The moment of reckoning is now, 
the moment you solve this problem. 
At some time y ≥ 0 slarboseconds from now, 
the number of slarboseconds since each of the Great Events 
will be divisible by some maximum number T. 

If you can find the smallest value of y 
that gives this largest possible T, 
that will give you the optimum anniversary when the apocalypse will come.

On Jamcode IX, for example, 
there were 3 Great Events 
and they happened 26000, 11000 and 6000 slarboseconds 
before the moment of reckoning. 

4000 slarboseconds later, 
the amount of time since each event 
was a multiple of T=5000 slarboseconds, 
and the apocalypse came.

Your job is to compute the amount of time 
until the apocalypse comes. 

But remember the prophecy: 
even though the people of Jamcode X 
have been solving problems for two years, 
and 64-bit integers have always been enough, 
they might not always be enough now or in the future.


Input

The first line of the input gives the number of test cases, C. 
C lines follow. 
Each starts with a single integer N, 
which is followed by a space 
and then N space-separated integers ti, 
the number of slarboseconds since Great Event i occurred.

Output

For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the minimum number of slarboseconds 
until ti + y is a multiple of the largest possible integer factor T for all i.

Limits

1 ≤ C ≤ 100.
ti ≠ tj for some i, j.

Small dataset

2 ≤ N ≤ 3.
1 ≤ ti ≤ 108.

Large dataset

2 ≤ N ≤ 1000.
1 ≤ ti ≤ 1050.

Sample


Input 
 
3
3 26000000 11000000 6000000
3 1 10 11
2 800000000000000000001 900000000000000000001

Output 
 
Case #1: 4000000
Case #2: 0
Case #3: 99999999999999999999

Epilogue

Fortunately for the peoples of the Jamcode system, 
"the apocalypse" turned out to be a mistranslation of "the giant party." 
Nobody from Jamcode IX bothered to pass this along, 
because they were having so much fun.




This turned out to be the hardest problem in the qualification round. 
One of the reasons may have been 
that the solution involves big arithmetic precision. 

Using big numbers in timed programming contests 
is sometimes not considered fair 
because some languages like python or java 
have internal libraries that deal with this 
while for other languages you may have needed 
to write your own or search 
for a external one like the GNU Multi-Precision Library. 

Considering that the qualification round was 24 hours long 
we took this chance to give you a warning and one which is fair ... 
that big numbers are fair game for now on, so have a library on hand!

This problem talks about divisors 
and multiples so it hints at using the greatest common divisor concept in some way. 
To solve the problem we need to find T and afterwards we can easily find y.

Let's simplify the problem and just look at two numbers a and b. 
In this case we need to find the largest T 
so that some positive y exists where a + y and b + y are multiples of T. 

a + y = k * T
b + y = m * T

In computing, the modulo (sometimes called modulus) 
operation finds the remainder of division of one number by another.

So (a + y) modulo T = (b + y) modulo T 
which means that (a - b) modulo T = 0. 
Thus T must be a divisor of |a - b|.

Coming back to the problem with N numbers, 
we have proved that T must divide every |ti - tj|. 

This means that the solution 
is the greatest common divisor of all |ti - tj| numbers. 

A simple observation can improve our algorithm from O(N2) 
to O(N) iterations of the Euclidean algorithm. 

Let's a and b are integer numbers and a != 0, b != 0 

 a > b > r1 > r2 > r3 > r4 > ... > rn

 a  = b  * q0 + r1
 b  = r1 * q1 + r2
 r1 = r2 * q2 + r3
 ....
 rk-2 = rk-1 * qk-1 + rk
 rn-2 = rn-1 * qn-1 + rn
 rn-1 = rn * qn

 Suppose a,b∈Z and a∨b≠0.

 From the Division Theorem, a=qb+r where 0≤r<|b|.
 From GCD with Remainder, the GCD of a and b is also the GCD of b and r.

 Theorem

 Let a,b∈Z.
 Let q,r∈Z such that a=qb+r.
 Then gcd{a,b}=gcd{b,r}
 where gcd{a,b} is the greatest common divisor of a and b.

 gcd{a,b}∖a ∧ gcd{a,b}∖b		     GCD is a common divisor	         
 gcd{a,b}∖(a−qb)			         Common Divisor Divides Integer Combination	         

 gcd{a,b}∖r			                 as r=a−qb	         
 gcd{a,b}\b               

 gcd{a,b}≤gcd{b,r}			         Definition of gcd{b,r} as the greatest common divisor of b and r	 


 The argument works the other way about:

 gcd{b,r}∖b ∧ gcd{b,r}∖r			 GCD is a common divisor	         
 gcd{b,r}∖(qb+r)			         Common Divisor Divides Integer Combination	         

 gcd{b,r}∖a			                 as a=qb+r	         
 gcd{b,r}∖b                          

 gcd{b,r}≤gcd{a,b}			         Definition of gcd{a,b} as the greatest common divisor of a and b	 


 How to find the greatest common divisor for n numbers.

 Since GCD is associative, GCD(a,b,c,d) is the same as GCD(GCD(GCD(a,b),c),d)


 Let's say a ≤ b ≤ c, then 

 gcd(b - a, c - a) = gcd(b - a, c - a, c - b). 
 
 To prove this we look at the first iteration of the Euclidean algorithm. 
 Since a ≤ b ≤ c it means 
 that b - a ≤ c - a so in the first step 
 we subtract b - a from c - a: 
 
 gcd(b - a, c - a) = gcd(c - b, b - a), this proves our observation. 
 
 Now we can just compute the greatest common divisor of all |ti - t1| to find T.

 Here's some python code from Xiaomin Chen that solves one test case:


 def Gcd(a, b):
  if b == 0:
    return a
  return Gcd(b, a % b)

def Solve(L):
  y = L[0]
  L1 = [abs(x - y) for x in L]
  g = reduce(Gcd, L1)
  if y % g == 0:
    return 0
  else:
    return g - (y % g)



*/