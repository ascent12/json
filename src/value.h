#ifndef VALUE_H
#define VALUE_H

#include "all.h"

void value_destroy(struct value *v);
bool value_is_valid(struct value *v);

#define value_set(_value, _data) _Generic((_data),	\
	int:		value_set_int,			\
	double: 	value_set_double,		\
	bool:		value_set_bool,			\
	char *:		value_set_string,		\
	object *:	value_set_object,		\
	array *:	value_set_array,		\
	default:	value_set_ptr)(_value, _data)

int value_set_int(struct value *v, int value);
int value_set_double(struct value *v, double value);
int value_set_bool(struct value *v, bool value);
int value_set_string(struct value *v, char *value);
int value_set_object(struct value *v, object *value);
int value_set_array(struct value *v, array *value);
int value_set_ptr(struct value *v, void* value);

#define value_get(_value, _destination) _Generic(_destination, \
	int *:		value_get_int, \
	double *:	value_get_double, \
	bool *:		value_get_bool, \
	char **:	value_get_string, \
	object **:	value_get_object, \
	array **:	value_get_array, \
	default:	value_get_ptr)(_value, _destination)

int value_get_int(struct value *v, int *dest);
int value_get_double(struct value *v, double *dest);
int value_get_bool(struct value *v, bool *dest);
int value_get_string(struct value *v, char **dest);
int value_get_object(struct value *v, object **dest);
int value_get_array(struct value *v, array **dest);
int value_get_ptr(struct value *v, void **dest);

void tab_indent(FILE *fp, int indent);
void value_print(FILE *fp, struct value *v, int indent);

#endif
