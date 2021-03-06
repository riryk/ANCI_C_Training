/*


Problem


We have come up with the best possible language here at Google, called Googlerese. 
To translate text into Googlerese, 
we take any message and replace each English letter with another English letter. 
This mapping is one-to-one and onto, 
which means that the same input letter always gets replaced with the same output letter, 
and different input letters always get replaced with different output letters. 
A letter may be replaced by itself. 
Spaces are left as-is.

For example (and here is a hint!), 
our awesome translation algorithm includes the following three mappings:
'a' -> 'y', 'o' -> 'e', and 'z' -> 'q'. 
This means that "a zoo" will become "y qee".


Googlerese is based on the best possible replacement mapping, 
and we will never change it. 
It will always be the same. 
In every test case. 
We will not tell you the rest of our mapping 
because that would make the problem too easy, 
but there are a few examples below that may help.

Given some text in Googlerese, can you translate it to back to normal text?

Solving this problem

Usually, Google Code Jam problems have 1 Small input and 1 Large input. 
This problem has only 1 Small input. 
Once you have solved the Small input, you have finished solving this problem.

Input

The first line of the input gives the number of test cases, T. 
T test cases follow, one per line.

Each line consists of a string G in Googlerese, 
made up of one or more words containing the letters 'a' - 'z'. 
There will be exactly one space (' ') character 
between consecutive words and no spaces at the beginning or at the end of any line.

Output

For each test case, output one line containing 
"Case #X: S" where X is the case number 
and S is the string that becomes G in Googlerese.

Limits

1 ≤ T ≤ 30.
G contains at most 100 characters.
None of the text is guaranteed to be valid English.
Sample

Input
3
ejp mysljylc kd kxveddknmc re jsicpdrysi
rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd
de kr kd eoya kw aej tysr re ujdr lkgc jv


Output
Case #1: our language is impossible to understand
Case #2: there are twenty six factorial possibilities
Case #3: so it is okay if you want to just give up


In most Google Code Jam problems, 
each test case is completely separate 
and nothing you learn from one will be useful in another. 
This problem was different however:


"Googlerese is based on the best possible replacement mapping, 
and we will never change it. 
It will always be the same. 
In every test case."


We meant it when we said this! 
There really is just one mapping, 
and the main challenge here is figuring out what it is. 
Fortunately, there is a lot we can learn from the sample input and output. 
For example, by looking at the first word in the first line, 
we know that "our" becomes "ejp" in Googlerese, 
so 'o' -> 'e', 'u' -> 'j', and 'r' -> 'p'. 
If you go through the entire text, 
you will that there is almost enough information to reconstruct the entire mapping:


'a' -> 'y'
'b' -> 'n'
'c' -> 'f'
'd' -> 'i'
'e' -> 'c'
'f' -> 'w'
'g' -> 'l'
'h' -> 'b'
'i' -> 'k'
'j' -> 'u'
'k' -> 'o'
'l' -> 'm'
'm' -> 'x'
'n' -> 's'
'o' -> 'e'
'p' -> 'v'
'q' -> ???
'r' -> 'p'
's' -> 'd'
't' -> 'r'
'u' -> 'j'
'v' -> 'g'
'w' -> 't'
'x' -> 'h'
'y' -> 'a'
'z' -> ???

We just need to figure out how to translate 'q' and 'z'. 
But if you read the problem statement carefully, 
you will notice there was one more example we gave you! 
"a zoo" gets translated to "y qee".
This means that 'z' gets mapped to 'q'.

Next we have to figure out what 'q' gets mapped to. 
For this part, you need to remember that every letter gets mapped to something different. 
And if you look carefully, there is already a letter getting mapped to everything except for 'z'. 
This leaves only one possibity: 'q' must get mapped to 'z'.

And now we have the full translation mapping, 
and all we need to do is write a program to apply it to a bunch of text. 
Here is a solution in Python:

translate_to_english = {
    ' ': ' ', 'a': 'y', 'b': 'h', 'c': 'e', 'd': 's',
    'e': 'o', 'f': 'c', 'g': 'v', 'h': 'x', 'i': 'd',
    'j': 'u', 'k': 'i', 'l': 'g', 'm': 'l', 'n': 'b',
    'o': 'k', 'p': 'r', 'q': 'z', 'r': 't', 's': 'n',
    't': 'w', 'u': 'j', 'v': 'p', 'w': 'f', 'x': 'm',
    'y': 'a', 'z': 'q'}

for tc in xrange(1, int(raw_input()) + 1):
  english = ''.join(
      [translate_to_english[ch] for ch in raw_input()])
  print 'Case #%d: %s' % (tc, english)



 */



/*



You're watching a show where Googlers 
(employees of Google) dance, 
and then each dancer is given a triplet of scores by three judges. 
Each triplet of scores consists of three integer scores from 0 to 10 inclusive. 
The judges have very similar standards, 
so it's surprising if a triplet of scores contains two scores that are 2 apart. 
No triplet of scores contains scores that are more than 2 apart.


For example: 
(8, 8, 8) and (7, 8, 7) are not surprising. 
(6, 7, 8) and (6, 8, 8) are surprising. 
(7, 6, 9) will never happen.

The total points for a Googler is the sum of the three scores 
in that Googler's triplet of scores. 

The best result for a Googler is the maximum of the three scores 
in that Googler's triplet of scores. 

Given the total points for each Googler, 
as well as the number of surprising triplets of scores, 
what is the maximum number of Googlers that could have had a best result of at least p?

For example, suppose there were 6 Googlers, 
and they had the following total points: 29, 20, 8, 18, 18, 21. 
You remember that there were 2 surprising triplets of scores, 
and you want to know how many Googlers could have gotten a best result of 8 or better.

With those total points, and knowing that two of the triplets were surprising, 
the triplets of scores could have been:

10 9 10
6 6 8 (*)
2 3 3
6 6 6
6 6 6
6 7 8 (*)

The cases marked with a (*) are the surprising cases. 
This gives us 3 Googlers who got at least one score of 8 or better. 
There's no series of triplets of scores 
that would give us a higher number than 3, so the answer is 3.

Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case consists of a single line containing integers separated by single spaces. 
The first integer will be N, the number of Googlers, 
and the second integer will be S, 
the number of surprising triplets of scores. 
The third integer will be p, as described above. 
Next will be N integers ti: the total points of the Googlers.

Output

For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the maximum number of Googlers 
who could have had a best result of greater than or equal to p.

Limits

1 ≤ T ≤ 100.
0 ≤ S ≤ N.
0 ≤ p ≤ 10.
0 ≤ ti ≤ 30.
At least S of the ti values will be between 2 and 28, inclusive.
Small dataset

1 ≤ N ≤ 3.
Large dataset

1 ≤ N ≤ 100.
Sample


Input 
 	
Output 
 
4
3 1 5 15 13 11
3 0 8 23 22 21
2 1 1 8 0
6 2 8 29 20 8 18 18 21
Case #1: 3
Case #2: 2
Case #3: 1
Case #4: 3





Clever contestants will have noticed 
that this problem might be based on an actual television show. 

The author of the problem found himself 
watching an episode of Dancing With the Stars 

(the show and its distributor, BBC Worldwide, 
do not endorse and are not involved with Google Code Jam), 

and having forgotten whether any of the contestants 
had earned a score of at least 8 from any of the judges.

The judges on that show very often agree within 1 point of each other, 
and it really is surprising when they disagree by so much. 
There are a few key observations that help us solve this problem:


Any non-surprising score is expressed uniquely 
by an unordered triplet of judges' scores. 

21 must be 7 7 7; 
22 must be 7 7 8; 
23 must be 7 8 8.

There only a few kinds of surprising scores. 
21 could be 6 7 8; 
22 could be 6 8 8; 
23 could be 9 7 7. 

There's a repeating pattern of surprising scores 
that extends from 2 (0, 0, 2) to 28 (8, 10, 10).

The repeating pattern stops at 2 and 28 
because 1 and 29 can't be surprising: 

we can't use (-1, 1, 1) for 1: 
-1 is not allowed as a score. 
We also can't use (9, 9, 11) for 29.

Putting all of these facts together, 
we can easily construct a mapping 
from each total score to its best result if it's surprising, 
and to its best result if it isn't surprising:

unsurprising(0) = 0
unsurprising(1) = 1
unsurprising(2) = 1
unsurprising(n) = unsuprising(n-3) + 1
unsurprising(n) = ceiling(n/3), for 0 <= n <= 30

surprising(2) = 2
surprising(3) = 2
surprising(4) = 2
surprising(5) = 3
surprising(n) = surprising(n-3) + 1
surprising(n) = ceiling((n-1)/3) + 1, for 2 <= n <= 28

Many contestants wrote an initial solution 
that didn't take into account that 0, 1, 29 and 30 can't be surprising, 
and so failed one of our sample inputs. 
That's what they're there for!


One way to build the tables of unsurprising(n) and surprising(n) 
that doesn't require so much thought 
could involve writing three loops 
to go through all possible sets of judges' scores, 
checking whether each combination of three was valid or surprising, 
and building the maps that way.


Now, for each value ti we have one of three cases:

unsurprising(ti) >= p. 

This Googler can be "good" 
(i.e. have a maximum score of at least p) 
even with an unsurprising triplet.

2 <= ti <= 28 and surprising(ti) >= p > unsurprising(ti). 

This Googler can be "good" only by using a surprising triplet.

Otherwise, the Googler can't be "good".

A simple greedy algorithm works: 
take all of the Googlers of the first type, 
and as many as possible (at most S) of the second type.




Problem 3


Problem


Do you ever become frustrated with television 
because you keep seeing the same things, 
recycled over and over again? 

Well I personally don't care about television, 
but I do sometimes feel that way about numbers.

Let's say a pair of distinct positive integers (n, m) 
is recycled if you can obtain m by moving 
some digits from the back of n 
to the front without changing their order. 

For example, (12345, 34512) is a recycled pair since 
you can obtain 34512 by moving 345 from the end of 12345 to the front. 

Note that n and m must have the same number of digits 
in order to be a recycled pair. 

Neither n nor m can have leading zeros.

Given integers A and B with the same number of digits 
and no leading zeros, 

how many distinct recycled pairs (n, m) 
are there with A ≤ n < m ≤ B?


Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case consists of a single line containing the integers A and B.

Output

For each test case, output one line containing 
"Case #x: y", where x is the case number (starting from 1), 
and y is the number of recycled pairs (n, m) with A ≤ n < m ≤ B.

Limits

1 ≤ T ≤ 50.
A and B have the same number of digits.

Small dataset

1 ≤ A ≤ B ≤ 1000.

Large dataset

1 ≤ A ≤ B ≤ 2000000.

Sample


Input 
 	
Output 
 
4
1 9
10 40
100 500
1111 2222
Case #1: 0
Case #2: 3
Case #3: 156
Case #4: 287

Are we sure about the output to Case #4?

Yes, we're sure about the output to Case #4.




Many contestants got stuck in this problem 
because of the sample test case number 4.

Let's say n is 1212, then after moving 1 or 3 digits 
you will get 2121, 

hence the pair (1212, 2121) will be counted twice 
if you count all possible moves. 

You can avoid this by breaking out of the loop 
once you reach the original number again, 
which will happen after moving 2 digits in the above example.

For the small input you can simply check for each pair (n, m), 
A ≤ n < m ≤ B, 
whether it satisfies the conditions mentioned in the problem statement, 
and whether you can obtain m by moving some digits from the back of n 
to the front without changing their order. 

To check if you can obtain m from n 
simply try to move all possible groups of digits from n
and check if what you get is m. 

The digit shifting can be done 
using string manipulation or mathematical expressions, 
both will run within the time limit.


But the previous solution will not run 
within the time limit for the large test cases. 

So here is another solution which should run within the time limit. 

For each number n, A ≤ n ≤ B, try to move all possible groups of digits 
from its back to its front 
and check if the resulting number satisfies the conditions or not. 

If it does satisfy the conditions then increment the result. 
Don't forget to avoid counting the same number twice.


Here is a sample solution:


*/

int solve(int A, int B) 
{
    int power = 1, temp = A, n;
    int result = 0;

    while (temp >= 10) 
	{
        power *= 10;
        temp /= 10;
    }

    for (n = A; n <= B; ++n) 
	{
		/* Let's n = 1223 */
        temp = n;

        while (1) 
		{
			/* 
			 * 
			 */
            temp = (temp / 10) + ((temp % 10) * power);
            if (temp == n)
                break;
            if (temp > n && temp <= B)
                result++;
        }
    }
    return result;
}








