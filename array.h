#ifndef ARRAY_H
#define ARRAY_H

#include "all.h"
#include "value.h"

array *array_create(size_t size);

void array_destroy(array **a);

value *array_get_value(array *a, int i);

#define array_get_index(_array, _index, _destination) \
	value_get(array_get_value(_array, _index), _destination)

#define array_set_index(_array, _index, _data) \
	value_set(array_get_value(_array, _index), _data)

void array_resize(array *a);

#define array_push(_array, _data) \
	array_set_index(_array, _array->end++, _data); \
	array_resize(_array);

#define array_pop(_array, _destination) \
	array_get_index(_array, --_array->end, _destination)

void array_print(FILE *fp, array *a, int indent);

#endif
