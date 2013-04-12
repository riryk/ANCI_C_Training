
/*

Problem

You just moved from your hometown to a big metropolitan city! 
You love everything about your new environment, except for the food. 
Your hometown provides the best food in the region (called "quality food") 
and you sure will miss it.


Fortunately, the largest restaurant in your hometown 
provides a food delivery service. 
You can purchase any amount of food in one delivery. 

There is a constant delivery fee for every delivery, 
regardless of the amount of food purchased in the delivery.

This restaurant serves different types of food. 
Each type of food has two properties: a price-per-meal, and a time-to-stale. 

One "meal" of food will feed you for one day; 
once a meal has been eaten, it cannot be eaten again. 

The time-to-stale of a type of food is the maximum number of days 
for which that food can still be eaten, counting from when you received it. 
A time-to-stale of zero means you must eat that type of food on the day of delivery.

In a single delivery you can purchase as many different types of food, 
and as many meals of each of those types, as you have money for. 

Note that if a particular type of food has a time-to-stale of t, 
it doesn't make any sense to order more than t+1 meals of that food in one delivery: 
at least one meal would go stale before you could eat it.

This restaurant has a very fast delivery service, 
so you will receive all the food in a delivery on the same day that you purchased it, 
and you may eat some of the food on the same day. 
Food delivery is the only way for you to receive quality food.

Given an amount of money, which you can spend on meal prices and delivery fees, 
what is the maximum number of days for which you can eat quality food every day?

Input

The first line of input gives the number of test cases, T.
T test cases follow. 
Each test case will begin with three integers, M, F and N, 
denoting the amount of money you have, the delivery fee, 
and the number of types of food provided by the restaurant, respectively. 
N lines follow, each will consist of two integers, Pi and Si, 
denoting respectively the price-per-meal and time-to-stale of one type of food.


Output


For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the maximum number of days that you can keep eating at least one meal 
of quality food everyday.


Limits

1 ≤ T ≤ 50.
1 ≤ F ≤ M.
1 ≤ N ≤ 200.
1 ≤ Pi ≤ M.
Small dataset

0 ≤ Si ≤ 2,000,000.
1 ≤ M ≤ 2,000,000.
Large dataset

0 ≤ Si ≤ 1018.
1 ≤ M ≤ 1018.
Sample


Input 
 	
Output 
 
3
32 5 2
5 0
10 2
10 10 1
10 10
10 1 1
1 5
Case #1: 3
Case #2: 0
Case #3: 8


An example scenario for the first case is by purchasing 
one meal of the first type and one meal of the second type 
during your first day in the city (costing a total of 20). 

Eat the first type of food that day, 
and eat the second type the next day. 
During your third day, 
purchase one meal of the first type and eat it on the same day. 
This accounts for three days.







Multiple Solutions


Sometimes our problemsetters like to come up with big, 
complicated solutions to their problems, 
and sometimes it turns out that there are simpler alternatives. 
We'll present two solutions here, 
including the simple one that many problems solvers used, 
and the more complex one used by the author.


How much does a single delivery cost?

Let's look at the cost of a delivery containing K meals. 

It's easy to see that we should order 

the cheapest meal with a time-to-stale of at least 0, 
the cheapest meal with a time-to-stale of at least 1, 
the cheapest meal with a time-to-stale of at least 2, 

and so on until we order the cheapest meal with a time-to-stale of at least K. 
Since K could be very large, we'll solve this problem in O(Nlog(N)).

First we'll sort the kinds of food by price, 
and construct an array that contains 
[(0, 0), (d1, c1), (d2, c2), ...], 

where we should buy food that costs ci for the meals between 
di-1 and di after the delivery arrives. 

We can then process that into another array containing (di, Ci), 
where Ci is the total cost of a single delivery that lasts di days, 
and (Ci-Ci-1) / (di-di-1) = ci.

We only had to do those above steps once. 
Now that we have that array, it's an O(log(N)) 

binary search 
(
or a simpler linear search, 
since we'll find that we don't need 
the efficiency for either of our solutions) 

to find out how much it costs to have a delivery that lasts K days: 
find i such that di-1 ≤ K < di, and charge Ci-1 + (K-di-1)*ci-1.

We'll call that function SingleDeliveryCost(days), 
and for convenience later in this editorial 
we'll also define SingleDayCost(day) 
to be the cost to buy the cheapest meal with a time-to-stale of at least day. 

Observe that SingleDayCost(a) ≤ SingleDayCost(b) if a ≤ b, 
and that SingleDeliveryCost(days+1) = SingleDeliveryCost(days) + SingleDayCost(days+1).

Deliveries should be (almost) the same size

Let's show that all of your deliveries should be almost the same size. 
Let's suppose that we have a solution that has a delivery, A, 
containing a meals, and another delivery, B, 
that contains b meals, with b ≥ a+2. 

Then the cost for those two deliveries is SingleDeliveryCost(a) + SingleDeliveryCost(b). 
If instead we increased delivery A's size to a+1 
and decreased delivery B's size to b-1, 
we'd have the same total number of days, but the delivery cost would be:

SingleDeliveryCost(a+1) + SingleDeliveryCost(b-1)
= SingleDeliveryCost(a) + SingleDayCost(a+1) + SingleDeliveryCost(b) - SingleDayCost(b-1)
≤ SingleDeliveryCost(a) + SingleDeliveryCost(b).


This shows that all our deliveries should be of size a or a+1 for some value of a.
Deliveries should be of an "interesting" size

Let's say that we want to buy a total of D days' worth of food, 
and we want to know how big our deliveries should be. 
We'll start by considering a number of deliveries X that 
we're going to show is "uninteresting": 
one such that if we look the size of the largest delivery, 
ceil(D/X), 
then ceil(D/(X-1)) = ceil(D/X) = ceil(D/(X+1)). 
In such a case, adding a delivery changes the cost by 

SingleDeliveryCost(ceil(D/X)) - ceil(D/X)*SingleDayCost(ceil(D/X)). 

If we add a delivery, we'll change the cost by the negation of that amount. 
One of those two quantities has to be non-negative, 
which means that we never need to consider making X deliveries.

That means that we only need to look at numbers of deliveries 
such that if we changed the number of deliveries, 
we'd be changing SingleDayCost(ceil(D/X)). 

There are only 2N such delivery sizes, 
and trying all of them solves the problem in O(N log(N)).



The other approach


A competitor who couldn't convince himself or herself of the above 
still has a good chance of solving the problem. 

We'll start by presenting a trick ("Changing the Question") 
that you can use whether or not you have the above realization–
it might make the math easier–and a totally different approach that, admittedly, requires some math of its own.
Changing the Question

An observation we can make is that instead of solving the problem that was asked, 
we can play a trick to let ourselves solve a simpler problem: 
"Can I eat quality food for D days?" 
We do this by binary searching on the answer. 

We know the answer is between 0 and M, 
so we can start by solving that problem O(log(M)) times.


How many deliveries should there be?

If the logic in the previous solution was too complicated, 
here's an alternative that probably isn't going to make you any happier. 
On the other hand, if you were thinking that there weren't enough logarithmic factors 
in the complexity analysis, or enough -ary searches, this could make you happy. 
This was our original solution for the problem, 
and it makes the I/O author perversely proud to have been part of making 
a problem to which a binary search in a ternary search in a binary search 
is a perfectly reasonable solution.

The argument here is that we can ternary search for the number of deliveries 
that minimizes the cost to buy quality food for D days. 
In order for a ternary search to be possible, 
the function has to be strictly decreasing, 
then strictly increasing (or vice versa). 

In this case, the domain of the function 
is from where the number of deliveries X 
is the minimum possible number at which we could order D meals 
(regardless of how expensive they are), to X=D. 

We'll show that the function decreases, then increases over this domain.

First let's take the SingleDayCost function and extend it over the real numbers. 
We already know what it is for the integers; 
let's just linearly interpolate for the real numbers in between, 
and call the result G. 
This has the nice result that the cost for X deliveries and D days, 
which we'll call H(X), 
is H(X) = G(D/X)*X + X*F.

Now, we're about to start taking derivatives, 
and although G is continuous, it isn't differentiable. 

There are a couple of ways of getting around this, 
and we're going to take a physicists' route. 

We'll define a function G'' to be a sum of delta functions 
such that the double-integral of G'' over X is our original G. 

Then we'll define G' to be G'''s integral, 
and G to be G''s integral. 
That lets us say that the first derivative of G 
is non-negative everywhere, and so is its second derivative. 
Don't try this at home, folks, especially if you live in a math class.

What we're really interested in doing here is proving 
that H(X) is decreasing and then increasing,
which we can do by proving that it has a positive second derivative:

H(X) = G(D/X)*X + X*F
H'(X) = G(D/X) - X*G'(D/X)*X-2 + F
H'(X) = G(D/X) - G'(D/X)/X + F
H''(X) = -X-2G'(D/X) + X-2G'(D/X) + G''(D/X)X-3
H''(X) = G''(D/X)X-3 ≥ 0
Therefore H(X) is decreasing and then increasing 
(or just decreasing or increasing, which is also fine), 
and can be ternary searched on for the minimum cost of surviving D days. 
This algorithm's complexity works out to O(Nlog(N) + log2(M)log(N)).


*/

/*
3
32 5 2
5 0
10 2
10 10 1
10 10
10 1 1
1 5
Case #1: 3
Case #2: 0
Case #3: 8
*/

struct Food
{
	long Price;
    long Time;
};

void Pr9Swap(struct Food Foods[], int i, int j)
{
    struct Food temp;
	temp = Foods[i];
    Foods[i] = Foods[j];
    Foods[j] = temp;
}

/* Here we need to choose what food is better for us.
 * What food do we need to eat first? 
 *  Of course the food with the smaller time-to-stale
 *  we need to it first.
 * When food's time 
 */ 
int CompareFood(struct Food* A, struct Food* B)
{
    return A.Time < B.Time 
		|| A.Time == B.Time && A.Price > B.Price;
}

void HeapSort(struct Food Foods[], int start, int end)
{
	int medium, i, j;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
	Pr9Swap(Foods, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
       if (Activities[i].A * Activities[medium].B < 
		   Activities[medium].A * Activities[i].B)
	   {
		   Pr9Swap(Activities, i, j++);
	   }    
	}
  
    Pr9Swap(Foods, start, j - 1);
    HeapSort(Foods, start, medium - 1);
    HeapSort(Foods, medium + 1, end);
}

void Solve()
{
    int money = 32;
	int fee = 5;
	int n = 2;

	struct Food FoodChoises[2] = 
	{
		{ 5, 0 + 1 },
		{ 10, 2 + 1 }
	};

    
}


/*

#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int t;
long long money;
long long fee;
int n;



struct fun
{
    long long time, price;
} data[200];

bool operator < (const fun& a, const fun& b)
{
    return a.time < b.time || a.time == b.time && a.price > b.price;
}


long long getdays (long long i)
{
    long long tmp = money - i * fee;

    long long days = 0;
    for (int j = 0; j < n; j ++)
    {
        long long deltat = (data[j].time - (j > 0? data[j-1].time : 0));

//            cout << "j = " << j << ", deltat = " << deltat << endl;

        if (tmp / data[j].price / i / deltat >= 1)
        {
            days += deltat * i;
            tmp -= (data[j].price * i * deltat);
        }
        else
        {
            days += tmp / data[j].price;
            break;
        }
    }
    
    return days;
}

long long work ()
{
    sort (data, data + n);
    {
        int m = 0;
        for (int i = 0; i < n; i ++)
        {
            while (m > 0 && data[i].price <= data[m - 1].price)
                m --;
            data[m ++ ] =data[i];
        }
        n = m;
    }

    long long ll = 1, rr = money / fee;

    while (ll < rr)
    {
        long long mm1 = (ll * 2 + rr) / 3;
        long long mm2 = (ll + rr * 2 + 2) / 3;

        long long d1 = getdays (mm1);
        long long d2 = getdays (mm2);
        if (d1 > d2)
            rr = mm2 - 1;
        else
            ll = mm1 + 1;
    }

    return getdays(ll);
}

int main ()
{
    cin >> t;
    for (int ct = 1; ct <= t; ct ++)
    {
        cin >> money >> fee >> n;
        for (int i = 0; i < n; i ++)
        {
            cin >> data[i].price >> data[i].time;
            data[i].time ++;
        }

        cout << "Case #" << ct << ": " << work() << endl;
    }

    return 0;
}

*/