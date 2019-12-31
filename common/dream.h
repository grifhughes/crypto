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
#ifndef DREAM_H 
#define DREAM_H

#include <stdint.h>
#include <stddef.h>

enum pad_cnst {
	PAD_HASH = 0x60,
	PAD_KEYSTREAM = 0xE0,
	PAD_TAG = 0xA0
};

enum dream128_cnst {
        dream128_DIGEST = 32,
        dream128_KEY = 16
};

enum dream256_cnst {
        dream256_DIGEST = 64,
        dream256_KEY = 32
};

void dream128_hash(const uint8_t *buf, size_t bl, uint8_t digest[static 32]);
void dream128_wrap(const uint8_t key[static 16],
                const uint8_t *header, size_t hl, 
                const uint8_t *body, size_t bl, 
                uint8_t *cgram, 
                uint8_t *tag);
int dream128_unwrap(const uint8_t key[static 16],
                const uint8_t *header, size_t hl,
                const uint8_t *cgram, size_t cl, 
                uint8_t *body, 
                const uint8_t *tag);

void dream256_hash(const uint8_t *buf, size_t bl, uint8_t digest[static 64]);
void dream256_wrap(const uint8_t key[static 32],
                const uint8_t *header, size_t hl, 
                const uint8_t *body, size_t bl, 
                uint8_t *cgram, 
                uint8_t *tag);
int dream256_unwrap(const uint8_t key[static 32],
                const uint8_t *header, size_t hl,
                const uint8_t *cgram, size_t cl, 
                uint8_t *body, 
                const uint8_t *tag);
#endif
