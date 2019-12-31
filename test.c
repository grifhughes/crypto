#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static int ipow(int base, int exp)
{
	int result = 1;
	for (;;)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		if (!exp)
			break;
		base *= base;
	}
	return result;
}

void rand_truth(int *t, size_t nvars) 
{
	int ntruth = ipow(2, nvars);
	for(int i = 0; i < ntruth; i++)
		t[i] = rand() & 1;
}

void fwht(int *t, size_t n)
{
	if(n == 1)
		return;

	fwht(t, n/2);
	fwht(t+n/2, n/2);

	for(int i = 0; i < n/2; i++) {
		int tmp = t[i];
		t[i] = tmp + t[n/2+i];
		t[n/2+i] = tmp - t[n/2+i];
	}
}

int udist(int *t, size_t n)
{
	int m = abs(t[1]);
	int curr;
	for(size_t i = 2; i < n; i++) {
		curr = abs(t[i]);
		if(curr > m)
			m = curr;
	}
	return m;
}

void print_hex(uint8_t *buf, size_t l)
{
	for(size_t i = 0; i < l; i++)
		printf("%" PRIx8 "", buf[i]);
	putchar('\n');

}
int main(void)
{
	srand(time(NULL));
	int truth[16] = {
		0,
		1,
		0,
		1,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		0,
		1
	};
	fwht(truth, 16);
	int unex = udist(truth, 16);
	int nlin = 8 - unex;
	for(size_t i = 0; i < 16; i++)
		printf("%d ", truth[i]);
	putchar('\n');
	putchar('\n');
	printf("%d\n", nlin);
}
