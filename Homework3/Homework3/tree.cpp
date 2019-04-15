// Return the number of ways that all n2 elements of a2 appear
	  // in the n1 element array a1 in the same order (though not
	  // necessarily consecutively).  The empty sequence appears in a
	  // sequence of length n1 in 1 way, even if n1 is 0.
	  // For example, if a1 is the 7 element array
	  //	10 50 40 20 50 40 30
	  // then for this value of a2     the function must return
	  //	10 20 40			1
	  //	10 40 30			2
	  //	20 10 40			0
	  //	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0) return 1;		// only 1 way for empty arr 
	if (n1 <= 0) return 0;		// 0 ways for an empty arr to hold nonzero elements
	if (n1 < n2) return 0;		// 0 ways for an array to hold more elements than its size
	if (a1[0] == a2[0]) {
		return countIncludes(a1 + 1, n1 - 1, a2, n2) + countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	}
	else {
		return countIncludes(a1 + 1, n1 - 1, a2, n2);
	}
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider, int& firstNotGreater, int& firstLess)
{
	if (n < 0) n = 0;			
	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < divider)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > divider)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 1) return;
	int firstNotGreater = 0;
	int firstLess = n;
	int half = n / 2;
	divide(a, n, a[half], firstNotGreater, firstLess);
	order(a, firstNotGreater);
	order(a + firstLess, n - firstLess);
}