/* 
   reference implementation of the dream source code package - 256 bit security 
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
#include "../common/dream_impl.h"
#include "../common/dream.h"

static const uint64_t rc[10] = { 
	0x243F6A8885A308D3,
	0x13198A2E03707344,
	0xA4093822299F31D0,
	0x082EFA98EC4E6C89,
	0x452821E638D01377,
	0xBE5466CF34E90C6C,
	0xC0AC29B7C97C50DD,
	0x3F84D5B5B5470917,
	0x9216D5D98979FB1B,
	0xD1310BA698DFB5AC
};

#define G(a,b,c,d) 					\
	do { 						\
		c ^= ror64(a,32), d ^= ror64(b,17);	\
                a ^= (b & c) ^ d, b ^= (a & d) ^ c;	\
	} while(0)
#define Q(a,b,c,d) 					\
	do { 						\
		c ^= ror64(a,23), d ^= ror64(b,48);	\
                a ^= (b & c) ^ d, b ^= (a & d) ^ c;	\
	} while(0)
#define ROUND(n)                        		\
	do {                            		\
		s[0] ^= rc[n];				\
		G(s[0],s[4],s[8],s[12]);		\
		G(s[1],s[5],s[9],s[13]); 		\
		G(s[2],s[6],s[10],s[14]); 		\
		G(s[3],s[7],s[11],s[15]); 		\
							\
		s[0] ^= rc[n+1];	    		\
		Q(s[0],s[5],s[10],s[15]); 		\
		Q(s[1],s[6],s[11],s[12]); 		\
		Q(s[2],s[7],s[8],s[13]);  		\
		Q(s[3],s[4],s[9],s[14]);  		\
	} while(0)

static void f(uint8_t state[static 128])
{
	uint64_t s[16];
	for(size_t i = 0; i < 16; i++)
		s[i] = load64(state + (i * sizeof *s));

	ROUND(0);
	ROUND(2);
	ROUND(4);
	ROUND(6);
	ROUND(8);

	for(size_t i = 0; i < 16; i++)
		store64(state + (i * sizeof *s), s[i]);
}

#undef ROUND
#undef G

#define R 64 

static void absorb(uint8_t state[static 128], const uint8_t *in, size_t in_l, uint8_t pad)
{
	for(; in_l >= R; in += R, in_l -= R) {
		for(size_t i = 0; i < R; i++) 
			state[i] ^= in[i];        
		f(state);
	}
	for(size_t i = 0; i < in_l; i++)
		state[i] ^= in[i];
	state[in_l] ^= pad;
	state[R - 1] ^= 0x01;

	f(state);
}

static void squeeze(uint8_t state[static 128], uint8_t *out, size_t out_l)
{
	for(; out_l > R; out += R, out_l -= R) {
		for(size_t i = 0; i < R; i++)
			out[i] = state[i];
		f(state);
	}
	for(size_t i = 0; i < out_l; i++)
		out[i] = state[i];
}

static void key_init(uint8_t state[static 128], const uint8_t *key, size_t kl, const uint8_t *nonce, size_t nl)
{
	absorb(state, key, kl, PAD_KEYSTREAM);
	absorb(state, nonce, nl, PAD_KEYSTREAM);
}

void dream256_hash(const uint8_t *buf, size_t bl, uint8_t digest[static 64])
{
	uint8_t state[128] = {0};
	absorb(state, buf, bl, PAD_HASH);
	squeeze(state, digest, dream256_DIGEST); 
}

void dream256_wrap(const uint8_t key[static 32],
		const uint8_t *header, size_t hl, 
		const uint8_t *body, size_t bl, 
		uint8_t *cgram, 
		uint8_t *tag)
{
	if(hl >= R)
		return;

	uint8_t state[128] = {0};
	key_init(state, key, dream256_KEY, header, hl);

	size_t P = R - 1;
	for(; bl > P; body += P, cgram += P, bl -= P) {
		for(size_t i = 0; i < P; i++) 
			cgram[i] = body[i] ^ state[i];
		absorb(state, body, P, PAD_KEYSTREAM); 
	}
	for(size_t i = 0; i < bl; i++)
		cgram[i] = body[i] ^ state[i];
	absorb(state, body, bl, PAD_TAG);
	squeeze(state, tag, dream256_DIGEST);
}

int dream256_unwrap(const uint8_t key[static 32],
		const uint8_t *header, size_t hl,
		const uint8_t *cgram, size_t cl, 
		uint8_t *body, 
		const uint8_t *tag)
{
	if(hl >= R)
		return 0;

	uint8_t state[128] = {0};
	key_init(state, key, dream256_KEY, header, hl);

	size_t P = R - 1;
	for(; cl > P; body += P, cgram += P, cl -= P) {
		for(size_t i = 0; i < P; i++) 
			body[i] = cgram[i] ^ state[i];
		absorb(state, body, P, PAD_KEYSTREAM); 
	}
	for(size_t i = 0; i < cl; i++)
		body[i] = cgram[i] ^ state[i];
	absorb(state, body, cl, PAD_TAG);

	if(cnst_memcmp(state, tag, dream256_DIGEST) == 0) {
		return 1;
	} else {
		for(size_t i = 0; i < cl; i++)
			body[i] = 0;
		return -1;
	}
}
