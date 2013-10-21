
/*
 
 Problem


 The story goes...

 A long, long time ago, King Tyrone the Fair had 4 ministers. 

 The first minister (the king's top adviser) 
 was paid 7 gold pieces per week. 
 
 The second minister was paid 4 gold pieces per week. 
 The third and fourth ministers were each paid 6 gold pieces per week. 
 
 Unfortunately, Tyrone accidentally forgot 
 the Ministerial Compensation List in the photo copier one day, 
 and the List ended up on the front page of the Kingdom Times newspaper. 
 
 At this point, the second minister requested to speak to the king, 
 upset that his own salary was lower than 
 that of the lower ranked third minister.

 His Fairness King Tyrone saw no other solution 
 than to fire the third minister. 
 
 After all, lowering the third minister's salary, 
 raising the salary of the second minister, 
 or changing job titles 
 were all unfair solutions to the problem, in the king's opinion. 
 
 And who are we to question King Tyrone? 
 
 Of course, firing the third minister did not solve the problem. 
 The second minister continued to complain because his salary 
 was still lower than that of the fourth minister. 
 
 So King Tyrone fired the fourth minister as well. 
 
 At this point, neither of the two remaining ministers complained, 
 and everyone lived happily ever after.

 ...wait a minute. I messed that up. I'm sorry. 
 My memory is not what it used to be. 
 One moment please... Right. 

 King Tyrone the Fair. Four ministers. 
 Paid 7, 4, 6, and 6 respectively. Ah, yes. The ending went like this...

 When the second minister complained of unfairness, 
 King Tyrone fired the first minister. 
 
 Some might say this was a bit harsh, 
 as the first minister wasn't involved in any way, 
 but we shouldn't question King Tyrone. 
 
 Obviously, the second minister still complained, 
 so King Tyrone simply fired him. 
 
 Of the remaining two ministers, 
 each one was being paid at least as much as any minister below him, 
 so none of them complained. 
 
 And everyone lived happily ever after.

 Much better... I think. Maybe? 
 Now I'm not sure anymore. 
 I know for certain that there were N ministers, 
 and I clearly remember their salaries. 
 
 I also know that every time a minister's salary 
 was lower than the salary of a minister below him, 
 somebody would complain, 
 and some minister got fired; 
 
 but that it could have been any minister, 
 regardless of whether that minister 
 had anything at all to do with the problem. 
 
 Ministers continued to be fired 
 until no one complained because 
 all of the salaries were non-increasing. 
 
 At that point, the firings stopped. 
 But I do not remember in which order the ministers got fired.

 Can you help me fix my story? 
 
 Or at least please tell me how many different stories I could have told. 
 Two stories are different if the sequences of fired ministers in them are not the same.


 Input


 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each one consists of two lines. 
 The first line will contain an integer N, 
 and the second line will contain N space-separated integers 
 denoting the ministers' salaries, 
 in order from the first minister to the N'th minister.


 Output


 For each test case, output one line containing "Case #x: y", 
 where x is the case number 
 (starting from 1) 
 and y is the number of stories I could tell you, modulo 10007.


 Limits
 

 Each salary will be positive and at most 10000.

 Small dataset
  
 1 ≤ T ≤ 100.
 1 ≤ N ≤ 100.

 Large dataset

 1 ≤ T ≤ 20.

 For 80% of test cases, 1 ≤ N ≤ 2000.
 For all test cases, 1 ≤ N ≤ 8000.

 Sample


 Input 
 	
 Output 
 
 3
 4
 7 4 6 6
 8
 90 80 70 60 50 50 40 30
 2
 7 8

 Case #1: 14
 Case #2: 1
 Case #3: 2
 



 Dissecting the problem


 The problem statement asks for the number of ways 
 to remove elements from a sequence 
 until we obtain a non-increasing sequence. 
 
 Looking at the problem from the end, 
 suppose we know the final non-increasing sequence, 
 and it's of length K. 
 
 Certain elements have been eliminated, 
 and we removed N-K elements in total. 
 
 There are (N-K)! (factorial of N-K) ways to remove those elements. 

 So the first approximation seems to be to sum 
 those factorials over all non-increasing 
 subsequences of the the original sequence.

 However, this is not entirely correct. 
 Some non-increasing subsequences 
 are not even reachable since we stop 
 as soon as our sequence becomes non-increasing. 
 
 For example, in the second example 
 from the problem statement the sequence 
 is non-increasing from the start, 
 
 90 80 70 60 50 50 40 30

 so no proper subsequence is reachable at all. 
 For subsequences that are reachable, 
 not every way of reaching them might be possible. 
 
 For example, in the first example from the problem statement 
 
 7 4 6 6

 we can reach the '7 <first 6>' subsequence 
 (note, it should be considered different from '7 <second 6>' subsequence) 
 by eliminating the second 6, and then 4. 
 
 However, it can't be reached by doing those operations in reverse order, 
 since we'd stop right after eliminating 4.


*/








