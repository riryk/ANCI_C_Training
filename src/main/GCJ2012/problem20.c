/*

  
  Problem

  I have a set of positive integers S. 
  Can you find two non-empty, distinct subsets with the same sum?

  Note: 
  A subset is a set that contains only elements from S, 
  and two subsets are distinct 
  if they do not have exactly the same elements.

  Input

  The first line of the input gives the number of test cases, T. 
  T test cases follow, one per line. 
  Each test case begins with N, 
  the number of positive integers in S. 
  It is followed by N distinct positive integers, all on the same line.

  Output

  For each test case, first output one line containing "Case #x:", 
  where x is the case number (starting from 1).

  If there are two different subsets of S that have the same sum, 
  then output these subsets, one per line. 
  Each line should contain the numbers in one subset, separated by spaces.

  If it is impossible, then you should output the string "Impossible" on a single line.

  If there are multiple ways of choosing two subsets with the same sum, any choice is acceptable.

  Limits

  No two numbers in S will be equal.
  1 ≤ T ≤ 10.

  Small dataset

  N is exactly equal to 20.
  Each number in S will be a positive integer less than 105.

  Large dataset

  N is exactly equal to 500.
  Each number in S will be a positive integer less than 1012.

  Sample

  Input
  2
  20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  20 120 266 858 1243 1657 1771 2328 2490 2665 2894 3117 4210 4454 4943 5690 6170 7048 7125 9512 9600


  Output

  Case #1:
  1 2
  3

  Case #2:
  3117 4210 4943
  2328 2894 7048



  The Small Input

  If you want to forget about the large input 
  and go straight for the small, 
  then Equal Sums might look like a classic dynamic programming problem. 
  Here is a sketch of one possible solution in Python:


  def GetSetWithSum(x, target):
      if target == 0: return []
      return GetSetWithSum(x, target - x[target]) + [x[target]]

  def FindEqualSumSubsets(S):
     x = [0] + [None] * (50000 * 20)
     for s in S:
        for base_sum in xrange(50000 * 20, -1, -1):
          if x[base_sum] is not None:
             if x[base_sum + s] is None:
                x[base_sum + s] = s
             else:
                subset1 = GetSetWithSum(x, base_sum + s)
                subset2 = GetSetWithSum(x, base_sum) + [s]
          return subset1, subset2
  return None

  The idea is that, in x, 
  we store a way of obtaining every possible subset sum. 
  If we reach a sum in two different ways, 
  then we can construct two subsets with the same sum.

  For the small input, this approach should work fine. 
  However, x would have to have size 500 * 1012 for the large input. 
  That is too big to work with, and you will need a different approach there.
  


  The Large Input

  The first step towards solving the large input 
  is realizing that the statement is very misleading! 
  
  Let's suppose you have just 50 integers less than 10^12. 
  There are 2^50 ways of choosing a subset of them, 
  and the sum of the integers in any such subset is at most 50 * 10^12 < 2^50. 
  
  By the Pigeonhole Principle, this means that 
  there are two different subsets with the same sum.

  So what does that mean for this problem? 
  Well, even if you had just 50 integers to choose from instead of 500, 
  the answer would never be "Impossible". 
  
  You need to find some pair of subsets that have the same sum, 
  and there is a lot of slack to work with. 
  The trick is figuring out how to take advantage of that slack.

  The Birthday "Paradox" Method

  The simplest solution is based on a classic mathematical puzzle: the birthday paradox.

  The birthday paradox says that if you have just 23 people in a room, 
  then some two of them probably have the same birthday. 
  This can be surprising because there are 365 possible days, 
  and 23 is much smaller than 365. 
  
  But it is true! 

  One good way to look at is this: 
  
  We have n people and how many different pairs of people can be chosen 
  We choose the first people and we have (n - 1) candidates to make pair.
  The first pair can be chosen n(n - 1) variants
  (n - 2) people left. The second pair can be chosen by (n - 3) variants

  6 people: Pair = [LeftPeople][RightPeople] 23 * 22 / 2 - we do not take order into account

  there are 23 choose 2 = 253 pairs of people, 

  and each pair of people has a 1/365 chance of having the same birthday. 

  In particular, the expected (aka average) number of pairs of people 
  with the same birthday is 253 / 365 = 0.693... 
  
  Once you write it that way, it is not too surprising that 
  the probability of having at least one pair matching is about 0.5.

  It turns out this exact reasoning applies to the Equals Sums problem just as well. 
  Here is a simple algorithm:

  Choose 6 random integers from S, add them up, and store the result in a hash set. 
  (Why 6? We'll come back to that later...)

  Repeat until two sets of 6 integers have the same sum, then stop.

  After choosing t sets, there will be t choose 2 pairs, 
  and each set will have sum at most 6 * 10^12. 
  
  Choose a random 6 numbers from the set - the max number value is 10^12.
  The max value for the sum is 6 * 10^12.

  sum(set(1)), sum(set(2)), ..., sum(set(t).)
  How many different pairs  t^2. 

  Therefore, the expected number of collisions would be approximately t^2 / ( 2 * 6 * 10^12). 

  When t is around 10^6, this expectation will be near 1, 
  and the reasoning from the birthday paradox says that we'll likely have our collision.

  That's it! Since we can quickly generate 10^6 small subsets and put their sums into a hash-set, 
  this simple algorithm will absolutely solve the problem.
  You may be worried about the randomness, but for this problem at least, you should not be. 
  This algorithm is extremely reliable. 
  By the way, this method will work on the small input as well, 
  so you don't need to do the dynamic programming solution if you do not want to.

  Note: There are many similar approaches here that can work. 
  For example, in our internal tests, one person solved the problem by only focusing on the first 50 integers, 
  and trying random subsets from there. 
  The proof below works for this algorithm, as well as many others.


  A Rigorous Proof


  If you have some mathematical background, 
  we can actually prove rigorously that the randomness is nothing to worry about. 
  It all comes down to the following version of the birthday theorem:

  Lemma: 

  Let X be an arbitrary collection of N integers with values in the range [1, R]. 
  Suppose you choose t + 1 of these integers independently at random. 
  If N ≥ 2R, then the probability that these randomly chosen integers 
  are all different is less than e^(-t^2 / 4R).

  Proof: 

  Let xi denote the number of integers in X with value equal to i. 
  The number of ways of choosing t + 1 distinct integers from X is precisely:

  sum(xi1 * xi2 * ... * xit+1), 1 ≤ i1 < i2 < ... < it+1 ≤ R

  For example, if t=1 and R=3, the sum would be x1 * x2 + x1 * x3 + x2 * x3. 

  R = 3: 1, 2, 3. We have X = 1,....,1,2,....,2,3,....,3 
                                 x1       x2       x3

  We need to choose 2 distinct integers:

 
  Each term here represents the number of ways 
  of choosing integers with a specific set of values. 
  
  Unfortunately, this sum is pretty hard to work with directly, 
  but Maclaurin's inequality states that it is at most the following:

  (R choose t+1) * [ (x1 + x2 + ... + xR) / R ]t+1 = (R choose t+1) * (N/R)t+1.

  On the other hand, the number of ways of choosing any t + 1 integers out of X 
  is equal to N choose t+1. 
  
  Therefore, the probability p that we are looking for is at most:

  [ (R choose t+1) / (N choose t+1) ] * (N/R)t+1
   = R/N * (R-1)/(N-1) * (R-2)/(N-2) * ... * (R-t)/(N-t) * (N/R)t+1.

  Now, since N ≥ 2R, we know the following is true for all a ≤ t:

  (R-a) / (N-a) < (R - a/2)^2 / [ R * (N-a) ]     

  ( this is because (R - a/2)^2 ≥ R(R-a) )

  ≤ (R - a/2)^2 / [ N * (R - a/2) ] = (R - a/2) / N.

  Therefore, p is less than:

  R * (R - 1/2) * (R - 2/2) * ... * (R - t/2) / Rt+1.

  It is now easy to check that (R-a/2) * (R-t/2+a/2) ≤ (R-t/4)2, from which it follows that p is also less than:

  (R - t/4)t+1 / Rt+1
   = (1 - t/4R)t+1
   < (1 - t/4R)t.

  And finally, we use the very useful fact that 1 - x ≤ e-x for all x. 
  This gives us p < e-t2 / 4R as required.


  In our case, X represents the sums of all 6-integer subsets. 
  We have N = 500 choose 6 and R = 6 * 1012. 
  You can check that N ≥ 2R, so we can apply the lemma 
  to estimate the probability that the algorithm will still be going after t+1 steps:

  If t = 106, the still-going probability is at most 0.972604477.
  If t = 5*106, the still-going probability is at most 0.499351789.
  If t = 107, the still-going probability is at most 0.062176524.
  If t = 2*107, the still-going probability is at most 0.000014945.
  If t = 3*107, the still-going probability is at most 0.00000000001.

  In other words, we have a good chance of being done after 5,000,000 steps, 
  and we will certainly be done after 30,000,000 steps. 
  Note this is a mathematical fact, regardless of what the original 500 integers were.

  Comment: 
  What happens if you look at 5-element subsets instead of 6-element subsets? 
  The mathematical proof fails completely because N < R. 
  In practice though, it worked fine on all test data we could come up with.


  A Deterministic Approach

  The randomized method discussed above is certainly 
  the simplest way of solving this problem. 
  However, it is not the only way. 
  Here is another way, this time with no randomness:

  Take 7,000,000 3-integer subsets of S, 
  sort them by their sum, 
  and let the difference between the two closest sums be d1. 
  
  Let X1 and Y1 be the corresponding subsets.

  Remove X1 and Y1 from S, 
  and repeat 25 times to get Xi, Yi and di for i from 1 to 25.

  Let Z = {d1, d2, ..., d25}. 

  Calculate all 225 subset sums of Z.

  Two of these subset sums are guaranteed to be equal. 
  
  Find them and trace back through Xi and Yi to find two corresponding subsets of S with equal sum.
  There are two things to show here, in order to justify that this algorithm works:

  S will always have at least 7,000,000 3-integer subsets. 
  This is because, even after removing Xi and Yi, S will have at least 350 integers, and 350 choose 3 > 7,000,000.

  Z will always have two subsets with the same sum. 
  First notice that the subsets in the first step have sum at most 3 * 10^12, 
  and so two of the sums must differ by at most 3 * 10^12 / 7,000,000 < 500,000. 
  
  Therefore, each di is at most 500,000. 
  Now, Z has 225 subsets, and each has sum at most 25 * 500,000 < 2^25, 
  and so it follows from the Pigeonhole Principle that two subsets have the same sum.

  Other similar methods are possible too. This is a pretty open-ended problem!
 
*/

void Pr20SmallSolve()
{
	int n, i;
	int s[100];
	int mark[1000000];
    int bitMask;
	int found;
    int m1, m2;


    memset(mark, 0, sizeof(mark));
    found = 0;

	/* We need the list of all possible bitMasks
	 * n = 5
	 * bitMask: 00001
	 * bitMask: 00010
	 * bitMask: 00011
	 * bitMask: 00100
	 * bitMask: 00110
	 * bitMask: 00111
	 * ....
	 * bitMask: 10000
	 * bitMask: 11000
	 * ....
	 * bitMask: 11111
	 */
	for (bitMask = 1; bitMask < (1 << n) && !found; bitMask++)
	{
        int sum = 0;
	    for (i = 0; i < n; i++)
		   if (bitMask & (1 << i)) sum += s[i];    

		if (mark[sum] == 0)
			mark[sum] = bitMask;
		else
		{
			m1 = mark[sum];
			m2 = bitMask;
			found = 1;
		}
	}
}

int* gen(int N)
{
	int res[100];
	int k;

	for(k = 0; k < 40; k++)
		res[k] = rand()%N;

	//sort(ALL(res)); sort all ekements 
	//res = unique(ALL(res);  delete all consecutive duplicates
	//res.erase(unique(ALL(res)), res.end()); delete the last element

	return res;
}

int Pr20ElemExist(int** M, int s)
{
    return -1;
}

void Pr20LargeSolve()
{
    int T = 5;
	int N;
	int t, i;
	int a[507];
	//cin >> T;
	for (t = 1; t < T; t++)
	{
		int M[100][100];
		int v[100];
		int VCount = 40;
		//cin >> N;
		N = 10;
		for (i = 1; i < N; i++)
		{
			//cin >> a[i];
            a[i] = 12;
		}
		while(1)
		{
			int s=0;
            //v = gen(N);  generate random range v

			for (i = 1; i < VCount; i++);
               s += a[v[i]];

		    if (Pr20ElemExist(M, s) && M[s] != v)
			   break;

			//M[s]=v; set vector
		}
	}
}



