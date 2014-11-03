#ifndef VALUE_H
#define VALUE_H

#include "all.h"
#include "object.h"
#include "array.h"

#define value_set(_value, _data) _Generic((_data), \
	int:		value_set_int, \
	double: 	value_set_double, \
	bool:		value_set_bool, \
	char *:		value_set_string, \
	object*:	value_set_object, \
	array*:		value_set_array, \
	default:	value_set_ptr)(_value, _data)

void value_set_int(value *v, int value);
void value_set_double(value *v, double value);
void value_set_bool(value *v, bool value);
void value_set_string(value *v, char *value);
void value_set_object(value *v, object *value);
void value_set_array(value *v, array *value);
void value_set_ptr(value *v, void* value);

#define value_get(_value, _destination) _Generic(_destination, \
	int*:		value_get_int, \
	double*:	value_get_double, \
	bool*:		value_get_bool, \
	char**:		value_get_string, \
	object**:	value_get_object, \
	array**:	value_get_array, \
	default:	value_get_ptr)(_value, _destination)

int value_get_int(value *v, int *dest);
double value_get_double(value *v, double *dest);
bool value_get_bool(value *v, bool *dest);
char *value_get_string(value *v, char **dest);
object *value_get_object(value *v, object **dest);
array *value_get_array(value *v, array **dest);
void *value_get_ptr(value *v, void **dest);

void value_destroy(value *v);

void tab_indent(FILE *fp, int indent);
void value_print(FILE *fp, value *v, int indent);

#endif
