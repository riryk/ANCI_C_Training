
/*


Problem


The city has built its first subway line, 
with a grand total of N stations, 
and introduced a new way of paying for travel. 

Instead of just paying for one ticket and making an arbitrary journey, 
the price you pay is now based on entry cards.

When entering the subway, 
each passenger collects an entry card, 
which specifies the station the passenger entered at. 

When leaving the subway, the passenger has 
to give up the entry card, 
and is charged depending on the distance 
(in stations traveled) 
between the entry station specified on the entry card, 
and the exit station on which the entry card is surrendered. 

The payment depends on the distance between these stations as follows:

if they are the same station, you don't pay;

if they are adjacent stations, you pay N pounds;

if the distance is two stations, you pay 2N - 1: 
a charge N for the first stop and N - 1 for the second;

the third station costs N-2 
(so you pay 3N - 3 for a three-station-long trip), 

the fourth stop N-3, and the ith stop N + 1-i;

thus, if you travel from one end of the subway to the other 
(a distance of N-1 stations), 

you pay 2 pounds for the last station traveled, 
and a grand total of (N2 + N - 2) / 2 in total.

After introducing this system the city noticed 
their gains are not as big as they expected. 

They figured out this might be due to people swapping their entry cards 
— so, for instance, 

if one person enters at station A, 
travels two stations to B and exits, 

while another person enters at B, 
travels three stations to C and exits, 

they would normally pay (in total) 
2N - 1 + 3N - 3 = 5N - 4. 

But if the two people swapped their entry cards at station B, 
then the first one would travel for free 
(as he would surrender an entry card specifying the station B 
 while exiting a station B, and so register a distance of zero); 
 
while the second person will exit at station C 
and surrender an entry card specifying station A, 
which is 5 stations away, and pays 5N - 10, 
at a net loss of six pounds to the city!

The city now wants to learn how much they can possibly lose 
if this practice becomes widespread. 

We will consider only one direction 
(from station 1 to station N, 
passing through all the stations in order) 
of the subway, 
and only one train on this line. 

We assume a passenger 
travelling from o to e 
obtains an entry card at o, 
can swap her entry card 
any number of times 
with any other passengers anywhere between o and e, 
including swapping with people 
who leave at o or those who enter at e, 
and then exit the train at e 
with some entry card 

(it is necessary to surrender 
some entry card to exit the subway). 

We also assume the passenger will not exit 
the train in the meantime 
(that is, will not surrender the currently held card and obtain a new one).

You are given a map of traffic 
(specifying how many passengers travel 
this train from which station to which), 
and you should calculate the city's financial loss, 
assuming passengers swap their cards to maximize this loss.


Input


The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case contains the number N of stops 
(the stops are numbered 1 to N),
and the number M of origin-endpoint pairs given. 

The next M lines contain three numbers each: 
the origin stop oi, 
the end stop ei 
and pi: 
the number of passengers that make this journey.


Output

For each test case, 
output one line containing "Case #x: y", 
where x is the case number 
(starting from 1) 
and y is 
the total loss the city 
can observe due to ticket swapping, modulo 1000002013.

Limits

1 ≤ T ≤ 20.
1 ≤ oi < ei ≤ N

Small dataset

2 ≤ N ≤ 100.
1 ≤ M ≤ 100.
1 ≤ pi ≤ 100.
Large dataset

2 ≤ N ≤ 109.
1 ≤ M ≤ 1000.
1 ≤ pi ≤ 109.
Sample


Input 
 	
Output 
 
3
6 2
1 3 1
3 6 1
6 2
1 3 2
4 6 1
10 2
1 7 2
6 9 1
Case #1: 6
Case #2: 0
Case #3: 10

The first test case is the case 
described in the problem statement 
- two passengers meet at station 3 and swap tickets. 

In the second test case the two passengers 
don't meet at all, 
so they can't swap tickets 
(and so the city incurs no loss). 

In the third case, 
only one of the early passengers 
can swap tickets with the later passenger.









The small dataset

Note that in this problem we treat 
the passengers as one "player" in a game, 
and assume they all cooperate to pay 
as little as possible in total to the city. 

This means it doesn't matter 
who actually pays for a given entry card. 

In particular, when the train leaves a station, 
the charge on each entry card in the train increases 
(by N - i, where i is the number of stations this card traveled so far). 

Since all passengers want to exit the subway eventually, 
all entry cards will have to be paid 
— so we might just as well immediately subtract 
this cash from the passenger "total" and move on.

As long as nobody exists the train, 
there's no need to exchange entry cards. 
Only once someone needs to exits, 
the passengers (along with the ones who just entered) 
need to gather and figure out 
which entry cards do the passengers who are just leaving take with them. 
They should choose the entry cards 
that have been on the train 
for the shortest amount of time so far, 
since on each subsequent stop the price 
for such an entry card will be larger 
than the price for any card 
that has been on the train longer 
(as the price is N - i, where i is the number of stations the card traveled so far). 

This means that at every station, 
the passengers should pool all the cards together, 
and then whoever wants to exit takes the entry cards with the smallest distance.

For the small dataset, a naive implementation of this algorithm will work. 
We can process station by station, 
holding a priority queue (or even just any collection) of the entry cards present. 
When anyone wants to exit, 
we iterate over the collection 
to find the card that has been on the train for the shortest amount, 
add its cost to the total passengers cost 
and remove it from the set. 

We also need to figure out how much the passengers should pay, 
but fortunately that's easy.





The large dataset

The large data set needs a bit more subtlety. 
With the insane amounts of passengers 
and stations we will need to avoid processing unnecessary events. 

First of all, we should process only the stations 
at which someone wants to enter or exit. 

This will make us process only O(M) stations, 
much less than the N we would process otherwise. 

Moreover, we should avoid processing passengers one by one.

To this end, notice that the order in which 
we want to give exit cards to passengers is actually a LIFO stack 
— whichever card came in last will go out first. 

So, we can keep the information 
about entry cards present in the train in a stack. 

Whenever a new group of passengers comes in, 
we take their entry cards 
and put them onto the stack 
(as one entry, storing the number of cards and the entry station). 

Whenever any group wants to leave, 
we go through the stack. 

If the topmost group of cards is big enough, 
we simply decrease its size, pay for what we took, and continue. 

If not, we take the whole group of cards, pay for it, 
decrease the amount of cards 
we need by the size of the group, 
and proceed through the stack.

This algorithm will take only O(M) time in total 
to process all the passengers 
— we will put on the stack at most M times, 
so we will take a whole group from the stack at most M times, 
and each group of passengers will decrease a group size 
(not take the whole group of cards) at most once, 
so in total 
— at most M such operations in the whole algorithm. 

Additionally, we need to sort all the events 
(a group of passengers entering or leaving) up front, 
so we are able to solve the whole problem in O(M logM).

Finally, when implementing, one needs to be careful. 
Due to the large amounts of stations and passengers involved, 
we have to use modular arithmetic carefully, 
because — as always with modular arithmetic — we risk overflow. 

In particular, whenever we multiply three numbers 
(which we do when calculating how much to pay for a group of tickets), 
we need to take care to apply the modulo after multiplying the first two.

*/

/*
#include <stdio.h>
#include <utility>
#include <algorithm>

using namespace std;

const int md = 1000002013;

int cost(int n, int len) {
  int a = n + (n - len + 1);
  int b = len;
  if (a % 2 == 0) a /= 2;
  else b /= 2;
  return (long long)a * b % md;
}

pair <int, int> a[424242], b[424242];

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq=1;qq<=tt;qq++) {
    printf("Case #%d: ", qq);
    int n, m;
    scanf("%d %d", &n, &m);
    long long ans1 = 0, ans2 = 0;
    for (int i=0;i<m;i++) {
      int q, w, e;
      scanf("%d %d %d", &q, &w, &e);
      a[i] = make_pair(q, e);
      b[i] = make_pair(w, e);
      ans1 = (ans1 + (long long)e * cost(n, w-q)) % md;
    }
    sort(a, a+m);
    sort(b, b+m);
    for (int i=0;i<m;i++) 
	{
      int where = b[i].first;
      int count = b[i].second;
      for (int j=m-1;j>=0;j--)
        if (a[j].first <= where) {
          int add = count;
          if (a[j].second < add) add = a[j].second;
          ans2 = (ans2 + (long long)add * cost(n, where-a[j].first)) % md;
          a[j].second -= add;
          count -= add;
        }
    }
    printf("%d\n", (ans1 - ans2 + md) % md);
  }
  return 0;
}
*/