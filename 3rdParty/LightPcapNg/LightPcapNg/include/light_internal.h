// light_internal.h
// Created on: Jul 23, 2016

// Copyright (c) 2016 Radu Velea

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef INCLUDE_LIGHT_INTERNAL_H_
#define INCLUDE_LIGHT_INTERNAL_H_

#include "light_types.h"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#if defined(USE_Z_STD)
#include <zstd.h>      // presumes zstd library is installed

//An ethernet packet should only ever be up to 1500 bytes + some header crap
//We also expect some ovehead for the pcapng blocks which contain the ethernet packets
//so allocate 1700 bytes as the max input size we expect in a single shot
#define COMPRESSION_BUFFER_IN_MAX_SIZE 1700

//This is the z-std compression type I would call it z-std type and realias 
//2x but complier won't let me do that across bounds it seems
//So I gave it a generic "light" name....
struct light_compression_t
{
	uint32_t* buffer_in;
	uint32_t* buffer_out;
	size_t buffer_in_max_size;
	size_t buffer_out_max_size;
	int compression_level;
	ZSTD_CCtx* cctx;
};

struct light_decompression_t
{
	uint32_t* buffer_in;
	uint32_t* buffer_out;
	size_t buffer_in_max_size;
	size_t buffer_out_max_size;
	ZSTD_DCtx* dctx;
	int outputReady;
	ZSTD_outBuffer output;
	ZSTD_inBuffer input;
};

//I really would like to use _compression_t everywhere and realias that to light_compression * in lightpcapng.h
//but the complier does not seem to allow me to use 2x alias across files
typedef struct light_compression_t _compression_t;
typedef struct light_decompression_t _decompression_t;

//Setup some other compression
#elif defined(USE_THIS_COMPRESSION_INSTEAD)

//No compression
#else

typedef void _compression_t;

#endif

void light_free_compression_context(_compression_t* context);
_compression_t * light_get_compression_context(int compression_level);

void light_free_decompression_context(_decompression_t* context);
_decompression_t * light_get_decompression_context();

struct _light_pcapng {
	uint32_t block_type;
	uint32_t block_total_length;
	uint32_t *block_body;
	struct _light_option *options;
	struct _light_pcapng *next_block;
};

struct _light_option {
	uint16_t custom_option_code;
	uint16_t option_length;
	// uint32_t PEN;
	uint32_t *data;
	struct _light_option *next_option;
};

struct _light_pcapng_mem {
	uint32_t *mem;
	uint32_t **mem_blocks;
	size_t mem_size;
	size_t block_count;
	int owner;
};

// Private Functions
struct _light_pcapng *__copy_block(const struct _light_pcapng *pcapng, const light_boolean recursive);
struct _light_option *__copy_option(const struct _light_option *option);
size_t __get_option_total_size(const struct _light_option *option);
uint32_t *__get_option_size(const struct _light_option *option, size_t *size);
light_boolean __is_section_header(const struct _light_pcapng *section);
int __validate_section(struct _light_pcapng *section);

#endif /* INCLUDE_LIGHT_INTERNAL_H_ */
