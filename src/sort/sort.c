
#include "sort.h"


/***
*int InsertElementIntoSortedArray(int Arr[], int SortedCount, int Elem, int ArrLength) 
* - Inserts a new element into a sorted part of array and move elemenrts right
*
*Purpose:
*       This is an auxiliary function for the simple insertion sort algorythm
*
*Entry:
*       int Arr[]                 - an input sorted array
*       int SortedCount           - we suppose that [0,...,SortedCount - 1] part of array is sorted
*       int Elem                  - a new element to insert into an array. 
*                                   It must be enough space where to insert it. SortedCount <= ArrLength
*       int ArrLength             - the length of array 
*       int step -                - the distance between elements in the array for sorting
* 
*******************************************************************************/
int InsertElementIntoSortedArray(int Arr[], int SortedCount, int Elem, int ArrLength, int step)
{
	int i;

	/* Step cannot be less then 1 */
	if (step < 1)
	{
		return 0;
	}

	/* If the whole array is sorted, there are no free space where to insert the 
	 * element. So in this case we return false.
	 */
	if (SortedCount >= ArrLength)
	{
		return 0;
	}

	/* We must identify a position where to insert the new element.
	 * We go from the biggest element in the sorted array to the smallest
	 * until we find an appropriate place, where insert to.
	 * 
	 * Now i = SortedCount points to a vacant position. 
	 * After insertion the length of sorted array must be increased.
     *  
	 * And SortedCount - 1 points to the biggest element in sorted array.
	 */
	i = SortedCount;

	/* During the loop we shifts the lelemnts bigger than Elem for 1 position right */
	for (; i > 0 && Arr[i - step] > Elem; i-=step)
	{
		Arr[i] = Arr[i - step];
	}

	/* After this loop we have found the position or 0
	 * And now we insert the new element into this position
	 */

	Arr[i] = Elem;
}

/***
*void InsertinSort(int Arr[], int n) 
* - Sort an array using auxiliary function InsertElementIntoSortedArray
*
*Entry:
*       int Arr[]                 - an input array to sort
*       int n                     - the length of array
*       int step -                - the distance between elements in the array for sorting       
*
*******************************************************************************/
void InsertinSort(int Arr[], int n, int step)
{
	int i;
	int elem;

	/* It is obvious that a part of array consists of only one element is sorted 
	 * From start SortedCount = 0 and [Arr[0]] - is sorted.
	 * Then we choose the next element A[1] and insert it into the sorted part of array
	 */
	i = step;

	/* This loop goes through all elements from [1 ,..., n - 1] and insert them
	 * into a sorted part of the array
	 */
	for (; i < n; i++)
	{
		elem = Arr[i];
		InsertElementIntoSortedArray(Arr, i, elem, n, step);
	}
}

/***
*int BubbleMaxElement(int Arr[], int SortedPointFromEnd, int ArrLength) 
* - Analyzes a part [0,...,SortedPointFromEnd] and shifts all elements 
*   to move the biggest item into SortedPointFromEnd position.
*
*Purpose:
*       This is an auxiliary function for the simple bubble sort algorythm
*
*Entry:
*       int Arr[]                 - an input sorted array
*       int SortedPointFromEnd    - we suppose that [SortedPointFromEnd + 1,...,ArrLength - 1] 
*                                   part of array is sorted
*       int ArrLength             - the length of array 
* 
*******************************************************************************/
int BubbleMaxElement(int Arr[], int SortedPointFromEnd, int ArrLength)
{
	int j;
	int temp;

	/* If sorted position is out of available length, we return an error */
	if (SortedPointFromEnd > ArrLength - 1)
	{
		return 0;
	}

	/* Loop through all array and change neighbour elements until the biggest item
	 * will be in SortedPointFromEnd position
	 */
	for (j = 0; j < SortedPointFromEnd; j++)
    {
		/* If the j - th element is lower than the (j + 1) - th we swap them */
        if (Arr[j] > Arr[j + 1])
        {
            temp = Arr[j];
            Arr[j - 1] = Arr[j];
            Arr[j] = temp;
        }
    }

	return 1;
}

/***
*void BubbleSort(int Arr[], int n) 
* - Sorts an array using bubble approach
*
*Entry:
*       int Arr[]                 - an input array to sort
*       int n                     - the length of array
*       
*******************************************************************************/
void BubbleSort(int Arr[], int n)
{
	int i;

	/* Loop from the end to the start and do bubble */
	for (i = n - 1; i > 0; i--)
	{
		BubbleMaxElement(Arr, i, n);
	}
}

/*  Lets take for example an array of 16 elements
 *
 *  [A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16]
 *  1. First of all we sort those pairs of elements:
 *  (A1, A9) (A2, A10) (A3, A11) (A4, A12) (A5, A13) (A6, A14) (A7, A15) (A8, A16)
 *
 *  2. Then we sort those groups of elements by 4 in one group
 *  (A1, A5, A9, A13) (A2, A6, A10, A14) (A3 A7 A11 A15) (A4 A8 A12 A16)
 *     Lets sort the first group (A1, A5, A9, A13) using insertion sort
 *     - try to insert A5 into appropriate place - compare A1 comp A5
 *     - try to insert A9 into appropriate place - compare A9 with A1 or A5
 *     - try to insert A13 into appropriate place - compare A13 with A1 and A9 
 *
 *  3. Then we sort those groups of elements by 8 in one group
 *  (A1, A2, A5, A6, A9, A10 A13, A14) (A3 A7 A11 A15 ...)
 *  Conditions: A1 <= A5, A2 <= A6, A5 <= A9, A6 <= A10, A9 <= A13, A10 <= A14
 *  A2 into appr place - comp A2 with A1 - 1
 *  A5 into appr place - comp A5 with A2 - 1
 *  A6 into appr place - comp A6 with A5 and A1 - 2
 *  A9 into appr place - comp A9 with A6 and A2 - 2
 *  A10 into appr palce - comp A10 with A9 
 */
void ShellSort(int Arr[], int n)
{
	int step;

	/* Launch InsertionSort with different steps */
	for (step = n / 2; step > 0; step /= 2)
	{
		InsertinSort(Arr, n, step);
	}
}

/* Swap twoo elements in an array */
void Swap(int* Arr, int i, int j)
{
	int temp;

	temp = Arr[j];
	Arr[j] = Arr[i];
	Arr[i] = temp;
}

/***
*void QuickSort(int* Arr, int n) 
* - Sorts an array using Hoar's fastest approach
*
*Entry:
*       int* Arr                 - an input array to sort
*       int left                 - the left index
*       int right                - the right index
*       
*******************************************************************************/
void QuickSort(int* Arr, int left, int right)
{
    int mid;
	int midValue;
	int i;
    int j;

	/* Determine the median point */
    mid = (left + right) / 2;

	/* The median value */
	midValue = Arr[mid];

	/* Move mid element into the left position of the array */
	Swap(Arr, left, mid);

	/* Divide the whole array into two parts: 
	 * all items from the first part of array are less or equal than Arr[mid]
	 * all items from the second part of the array are more than Arr[mid]
	 * For example:
	 * Arr = [5, 6, 9, 1, 2, 6, 0, -1, 10, 12]
	 *       [5, 1, 9, 6, 2, 6, 0, -1, 10, 12]
     *       [5, 1, 2, 6, 9, 6, 0, -1, 10, 12]
	 *       [5, 1, 2, 0, 9, 6, 6, -1, 10, 12]
	 *       [5, 1, 2, 0, -1, 6, 6, 9, 10, 12]
	 *
	 * j = 6 i = 6, 9, 1 swap (6, 1) j = 9
	 * Arr = [5, 1, 9, 6, 2, 6, 0, -1, 10, 12]      
	 * j - points to the delimiter the lower part and the upper part.
	 * From start, i and j points to the start of array
	 */
	j = left + 1;
	for (i = left + 1; i <= right; i++)
	{
        if (Arr[i] < midValue)
		{
			Swap(Arr, i, j);
			j++;
		}
	}

	Swap(Arr, left, j - 1);
    QuickSort(Arr, left, mid - 1);
    QuickSort(Arr, mid + 1, right);
}


