
bool anyTrue(const double a[], int n)
{
	if (n <= 0) return true;
	if (somePredicate(a[n - 1])) return true;
	return anyTrue(a, n - 1);
}

int countTrue(const double a[], int n)
{
	if (n <= 0) return 0;
	if (somePredicate(a[n - 1])) {
		return 1 + countTrue(a, n - 1);
	}
	else {
		return 0 + countTrue(a, n - 1);
	}
}

int firstTrue(const double a[], int n)
{
	if (n <= 0) return -1;
	if (somePredicate(a[0])) {
		return 0;
	}
	if (firstTrue(a + 1, n - 1) == -1) return -1; 
	return 1 + firstTrue(a + 1, n - 1);
}

int positionOfMin(const double a[], int n)
{
	if (n <= 0) return -1;
	int min = 1 + positionOfMin(a + 1, n - 1);
	if (a[0] <= a[min]) {
		return 0;
	}
	else {
		return min;
	}
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 < 0) n1 = 0;
	if (n2 < 0) n2 = 0;
	if (n2 == 0) return true;		// n2 is empty, so true
	if (n1 == 0) return false;		// n1 is empty and n2 has elements, so false
	if (n1 < n2) return false;		// n2 is bigger than n1, so false
	if (a1[0] != a2[0]) {
		return includes(a1 + 1, n1 - 1, a2, n2);
	}
	else {
		return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	}
}
