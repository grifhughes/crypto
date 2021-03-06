#include "common/dream.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TESTS 500000

void rand_buf(uint8_t *buf, size_t l)
{
	for(size_t i = 0 ; i < l; i++)
		buf[i] = rand();
}

uint8_t count_ones(uint8_t byte)
{
	static const uint8_t NIBBLE_LOOKUP [16] =
	{
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4
	};

	return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}

int main(void)
{
	srand(time(NULL));

	uint8_t toggle, m[64];
	uint8_t digest1[dream256_DIGEST], digest2[dream256_DIGEST];

	int set = 0, ctr = 0;
	const double digest_bits = dream256_DIGEST * 8;
	double *tksac = malloc(512 * sizeof(*tksac));

	for(size_t i = 0; i < TESTS; i++) {
		rand_buf(m, sizeof m);
		dream256_hash(m, sizeof m, digest1);

		for(size_t j = 0; j < sizeof m; j++) {
			for(size_t b = 0; b < 8; b++) {
				toggle = 1UL << b;
				m[j] ^= toggle;
				dream256_hash(m, sizeof m, digest2);

				for(size_t h = 0; h < dream256_DIGEST; h++)
					set += count_ones(digest1[h] ^ digest2[h]);
				tksac[ctr] += ((double)set/digest_bits);
				ctr++;
				set = 0;
				m[j] ^= toggle;
			}
		}
		ctr = 0;
	}

	for(size_t i = 0; i < 512; i++)
		printf("tksac bit %zu: %lf\n", i, tksac[i] / TESTS);
	free(tksac);
}
