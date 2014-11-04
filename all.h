#ifndef ALL_H
#define ALL_H

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct value value;
typedef struct object_pair object_pair;
typedef struct object object;
typedef struct array array;

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
	char* name;
	value value;

	object_pair *next;
	object_pair *order;
};

struct object {
	size_t size;

	object_pair **buckets;

	object_pair *head;
	object_pair *tail;
};

struct array {
	size_t size;
	size_t min;
	int end;

	value *data;
};

#endif
