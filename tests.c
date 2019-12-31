#include "common/dream.h"
#include <stdio.h>

static uint8_t vec1[] = "Yet not so many, not so fair, as when Thangorodrim was broken";
static uint8_t vec2[] = "The quick brown fox jumps over the lazy brown dog.";
static uint8_t vec3[] = "Hello, world ";

void print_hex(const uint8_t *buf, size_t bl)
{
        for(size_t i = 0; i < bl; i++)
                printf("%02x", buf[i]);
        putchar('\n');
}

int main(void)
{
        /* hash tests */
        uint8_t digest128[dream128_DIGEST];
        uint8_t digest256[dream256_DIGEST];

        dream128_hash(vec1, sizeof vec1, digest128);
        print_hex(digest128, sizeof digest128);

        dream128_hash(vec2, sizeof vec2, digest128);
        print_hex(digest128, sizeof digest128);

        dream128_hash(vec3, sizeof vec3, digest128);
        print_hex(digest128, sizeof digest128);

        dream256_hash(vec1, sizeof vec1, digest256);
        print_hex(digest256, sizeof digest256);

        dream256_hash(vec2, sizeof vec2, digest256);
        print_hex(digest256, sizeof digest256);

        dream256_hash(vec3, sizeof vec3, digest256);
        print_hex(digest256, sizeof digest256);
}
