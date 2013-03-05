
/* Problem description

Konstantin and Ilia live in the same house. 

Konstantin lives upstairs, and enjoys activities that 
involve jumping, moving furniture around, and - in general - making noise. 

Ilia lives downstairs, and enjoys sleep.

In order to have a good evening, Konstantin wants to do at least K activities. 
Last night, Ilia asked Konstantin to try not to wake him up; 
and because Konstantin is a very nice neighbor, he agreed. 

Unfortunately, he took Ilia's request a bit too literally, 
and he will choose his activities in such a way as to minimize 
the probability that Ilia is woken up after falling asleep.

Each possible activity for Konstantin has an associated probability ai/bi. 
If Konstantin performs this activity, 
then at the end of it, Ilia will be awake with probability ai/bi, 
and asleep otherwise, regardless of whether he was asleep at the start. 

Moreover, for each possible activity Konstantin can perform it at most ci times 
(more than that would be boring, and Konstantin won't have a good evening if he's bored).

Konstantin wants to choose a number of activities to do, in order, so that:

The total number of activities done is at least K.
The ith activity is performed no more than ci times.
The probability Q that Ilia is woken up one or more times 
during the course of the activities is as small as possible.

Ilia starts awake, so in order for him to be woken up, 
he must be asleep at the end of some activity, and then awake at the end of the next activity.

What is the smallest Q Konstantin can achieve while having a good evening? 
Note that Konstantin cannot tell whether Ilia is awake or asleep, 
and so he cannot adapt his activities using that information.

Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case starts with a pair of integers, N, K, on a line by themselves. 
N lines follow, each of which represents an activity that Konstantin can choose. 
Each of those lines is formatted as "ai/bi ci", 
indicating that there is an activity which would 
leave Ilia awake with probability ai/bi 
and which Konstantin can perform at most ci times without being bored.

Output

For each test case, output one line containing 
"Case #x: Q", where x is the case number (starting from 1) 
and Q is the smallest probability of Ilia waking up during 
the course of the activities that Konstantin performs. 
Answers with absolute or relative error no larger than 10-6 will be accepted.

Limits

1 ≤ T ≤ 100.
0 ≤ ai ≤ bi ≤ 1000000 for all i.
1 ≤ bi and 1 ≤ ci for all i.
1 ≤ K ≤ the sum of all ci in that test case.

Small dataset

1 ≤ N ≤ 100.
The sum of all ci is no larger than 100 in each test case.
Large dataset

1 ≤ N ≤ 10000.
The sum of all ci is no larger than 106 in each test case.

nput 
 	
Output 
 
3

4 1

1/2 3
1/5 2
2/5 1
2/2 2

3 2

1/2 2
1/3 2
3/4 2

3 3

99/100 1
1/2 2
1/50 3

Case #1: 0.000000000
Case #2: 0.083333333
Case #3: 0.015000000

Solution

Upstairs/Downstairs was a last-minute addition to the Finals, 
replacing a year-old problem proposal 
that had appeared on a separate contest two months before. 

This problem loosely mirrors an experience the author had once while on vacation. 
He was downstairs.


Solving this problem involves two observations and an algorithm. 
Before making our observations, 
let's start by writing out the formula for the quantity we want to minimize. 
pi will represent the probability that the ith activity 
Konstantin performs will result in Ilia being awake:

P(woken up) =
(1-p0) * (1 - (1-p1)(1-p2)*...* (1 - pK)) +
p0(1-p1) * (1 - (1-p2)(1-p3)*...*(1 - pK)) +
p0p1(1-p2) * (1 - (1-p3)(1-p4)*...*(1 - pK)) +
...

We need firstly to prove this formula. Lets suppose that we only have 2 
p0 - is probability when activity 0 will result in Illia being waking up
p1 - is probability when activity 1 will result in Illia being waking up
When Konstantin is doing this activities consequently: 0 and then 1. We need 
to compute the probability of Illia being waked up.

2 activities:

      0                       1
 t1 ----------------- t2 --------------t3
 [t1 t2] Konstantin is performing activity x 
 Illia was not sleeping in the beginning - after t2 he falls asleep
 K1 - is the possibility when he will be woken up after t3.
 (1 - p0) wont be wake up and p1 - will be wake up during 1



 Explanation:
 pi - is a probability when after i-th activity Illia will be awake.
 Opposite probability means that
 1 - pi - is a probability when after i-th activity Illia will be sleeping.
 According to definition of Illia being woken up we have:
 ....... i-th activity........ | ....... (i + 1)-th activity ........| 
 The necessary condition for Illia being woken up in the end of the (i + 1)th
 activity is he is sleeping after i - th activity.
 (i - pi)*pi+1 = the possibility of being woken up.
 Formula computation:

 1. Illia starts being awake in the beginning of activity 0.
    p0 - is the probability that Illia will be awake after activity 0
	(1 - p0) - is the probability that Illia will be sleeping after activity 0.
 Provided that we need Illia being awake after activity 1 or 2 or 3 or .... k.
 (1 - p1) Illia is sleeping after activity 1
 (1 - p2) Illia is sleeping after activity 2
 (1 - p3) Illia is sleeping after activity 3
 .....
 (1 - pk) Illia is sleeping after activity k
 (1 - p1)*(1 - p2)*(1 - p3)*...*(1 - pk) Illia is sleeping after activity 1 and 2 and 3 ... and k
 1 - (1 - p1)*(1 - p2)*(1 - p3)*...*(1 - pk) Illia is awake after one of the following activities:
 1 or 2 or 3 or .... or k.
 (1 - p0)*(1 - (1 - p1)*(1 - p2)*(1 - p3)*...*(1 - pk)) - a probability when Illia is sleeping after 
 activity 0 and is being awake after one of the activities: 1, 2, 3, ...., k

 2. Illia is awake after the activity 0 p0.
 Then he will be woken up when:
 1. After activity 1 he is sleeping and is being awake after one of the activities: 2, 3, ..., k
    p0*(1 - p1)*(1 - (1 - p2)*(1 - p3)*...*(1 - pk))
 2. After activity 1 he is awake. In this case there are also 2 possible cases:
  2.1 He is sleeping after 2 and awake after one of (3, 4, ... , k)
    p0*p1*(1 - p2)*(1 - (1 - p3)*(1 - p4)*...*(1 - pk))
  Continue this process 
  ....
  The latest 
  When Illia is awake after activity 0, activity 1,..., k - 2.
  Then he sleeps after activity k - 1 and is awake after activity k
  p0*p1*p2*p3*...*pk-2*(1 - pk-1)*(1 - (1 - pk))
  
 3. Lets see how changing order of activities can influence on total probability.
 Parts of the formula which contain pi and pi+1:

 p0*p1*...*(1 - pi)*(1 - (1 - pi+1)*(1 - pi+2)*...*(1 - pk)) +
 p0*p1*...*pi*(1 - pi+1)*(1 - (1 - pi+2)*...*(1 - pk)) = 

 p0*p1*...*pi-1*( 
   (1 - pi)*(1 - (1 - pi+1)*(1 - pi+2)*...*(1 - pk)) 
 + pi*(1 - pi+1)*(1 - (1 - pi+2)*...*(1 - pk))   ) = 

 For simlifying this formula we need to make some definition:
 A = p0*p1*...*pi-1 > 0
 B = (1 - pi+2)*...*(1 - pk)
 Then the formula looks like here:
 
 A*((1 - pi)*(1 - (1 - pi+1)*B) + pi*(1 - pi+1)*(1 - B)) = 
 (1 - pi)(1 - B + Bpi+1) + pi(1 - pi+1)(1 - B) = 
 1 - B + Bpi+1 - pi(1 - B) - Bpipi+1 + pi(1 - B) - pipi+1(1 - B) = 
 1 - B + Bpi+1 - Bpipi+1 - pipi+1(1 - B) = 
 1 - B + Bpi+1 - Bpipi+1 - pipi+1 + Bpipi+1 =
 1 - B + Bpi+1 - pipi+1 

 This means that if we swap i and j activity we decrease the formula.
 In optimal solution activities must be applied from noisiest to quietest.
 And we need to sort them from the noisiest to quietest.
 
 4. Analizing the formula for minimum or maximum.
 P(wake up) = f(p0, p1, p2, p3,...,pk-1, pk)
 0 <= p0 <= 1, ... , 0 <= pi <= 1, ... , 0 <= pk <= 1.

 Statement: For f = fmin we need pi to be max or min. 
 Provement: Lets suppose opposite. f = fmin and 
 min(pi) < pi < max(pi). We fix all other parameters and we get linear function.
 This function can be linear increasing or linear decreasing.
 We have received a contradiction.

 Note that here we're considering an activity that 
 can be repeated c times as c different activities.

 1/3 = p0    Sort them from noisiest to quietest: 
 1/3 = p1    3/4 1/2 1/3 1/3 1/4 1/7
 1/4 = p2
 1/2 = p3
 1/7 = p4
 3/4 = p4


 */