/*
 * Copyright (c) 2014, Scott Anderson <ascent@cock.li>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef ALL_H
#define ALL_H

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct array array;
typedef struct object object;

struct value {
	enum {
		type_undefined,
		type_int,
		type_double,
		type_bool,
		type_string,
		type_object,
		type_array,
		type_ptr
	} type;

	union {
		int i;
		double d;
		bool b;
		char* s;
		object *o;
		array *a;
		void *p;
	};
};

struct object_pair {
	struct object_pair *next;
	struct object_pair *order;

	struct value value;
	char key[];
};

struct object {
	struct object_pair *head;
	struct object_pair *tail;

	size_t size;
	struct object_pair *buckets[];
};

struct array {
	size_t size;
	struct value *data;
};

//array_print(FILE *fp, array *a, int index);

#endif
