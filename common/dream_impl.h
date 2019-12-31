/* 
   reference implementation of the dream source code package
   Copyright (C) <2018>  <Grif Hughes>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 */
#ifndef DREAM_IMPL_H
#define DREAM_IMPL_H

#include <string.h>
#include <stdint.h>

static inline void store32(uint8_t *d, uint32_t w)
{
           d[0] = (uint8_t)(w >> 24);
           d[1] = (uint8_t)(w >> 16);
           d[2] = (uint8_t)(w >> 8);
           d[3] = (uint8_t)(w >> 0);
}

static inline uint32_t load32(const uint8_t *s)
{
           return ((uint32_t )(s[0]) << 24) |
           	  ((uint32_t )(s[1]) << 16) |
            	  ((uint32_t )(s[2]) << 8)  |
           	  ((uint32_t )(s[3]) << 0); 
}

static inline uint32_t ror32(uint32_t a, size_t n) 
{
        return (a >> n) | (a << (32 - n));
}

static inline void store64(uint8_t *d, uint64_t w)
{
        d[0] = (uint8_t)(w >> 56);
        d[1] = (uint8_t)(w >> 48);
        d[2] = (uint8_t)(w >> 40);
        d[3] = (uint8_t)(w >> 32);
        d[4] = (uint8_t)(w >> 24);
        d[5] = (uint8_t)(w >> 16);
        d[6] = (uint8_t)(w >> 8);
        d[7] = (uint8_t)(w >> 0);
}

static inline uint64_t load64(const uint8_t *s)
{
        return ((uint64_t )(s[0]) << 56) |
               ((uint64_t )(s[1]) << 48) |
               ((uint64_t )(s[2]) << 40) |
               ((uint64_t )(s[3]) << 32) |
               ((uint64_t )(s[4]) << 24) |
               ((uint64_t )(s[5]) << 16) |
               ((uint64_t )(s[6]) << 8)  |
               ((uint64_t )(s[7]) << 0);
}

static inline uint64_t ror64(const uint64_t v, const unsigned n)
{
        return (v >> n) | (v << (64 - n));
}

static inline int cnst_memcmp(const void *a, const void *b, const unsigned n)
{
        const uint8_t *_a = (const uint8_t *)a;
        const uint8_t *_b = (const uint8_t *)b;
        uint8_t r = 0;

        for(unsigned i = 0; i < n; i++) {
                r |= _a[i] ^ _b[i];
        }

        return r;
}

#endif
