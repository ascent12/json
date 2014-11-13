#ifndef JSON11_H
#define JSON11_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* GCC visibility */
#if defined(__GNUC__) && __GNUC__ >= 4
#define JSON11_EXPORT __attribute__ ((visibility("default")))
#else
#define JSON11_EXPORT
#endif

/*
 * Key-value pair data type.
 * Represents a 'JSON object'.
 */
typedef struct object object;

/*
 * Dynamic array data type that can hold different data types.
 * Represents a 'JSON array'.
 */
typedef struct array array;

#define JSON11_ERR_OK		0
#define JSON11_ERR_ALLOC	-1
#define JSON11_ERR_TOOBIG	-2
#define JSON11_ERR_UNDEF	-3
#define JSON11_ERR_EXISTS	-4

object *object_create	(size_t size);
void    object_destroy	(object **o);

#if (__STDC_VERSION__ >= 201112L)
#define object_add(_object, _key, _value) _Generic(_value,	\
	int:		object_add_int,				\
	double:		object_add_double,			\
	bool:		object_add_bool,			\
	char *:		object_add_string,			\
	object *:	object_add_object,			\
	array *:	object_add_array,			\
	default:	object_add_ptr)(_object, _key, _value)
#endif

int object_add_int	(object *o, char *key, int value);
int object_add_double	(object *o, char *key, double value);
int object_add_bool	(object *o, char *key, bool value);
int object_add_string	(object *o, char *key, char *value);
int object_add_object	(object *o, char *key, object *value);
int object_add_array	(object *o, char *key, array *value);
int object_add_ptr	(object *o, char *key, void *value);

#if (__STDC_VERSION__ >= 201112L)
#define object_get(_object, _key, _dest) _Generic(_dest,	\
	int *:		object_get_int,				\
	double *:	object_get_double,			\
	bool *:		object_get_bool,			\
	char **:	object_get_string,			\
	object **:	object_get_object,			\
	array **:	object_get_array,			\
	default:	object_get_ptr)(_object, _key, _dest)
#endif

int object_get_int	(object *o, char *key, int *dest);
int object_get_double	(object *o, char *key, double *dest);
int object_get_bool	(object *o, char *key, bool *dest);
int object_get_string	(object *o, char *key, char **dest);
int object_get_object	(object *o, char *key, object **dest);
int object_get_array	(object *o, char *key, array **dest);
int object_get_ptr	(object *o, char *key, void **dest);

void object_remove	(object *o, char *key);
bool object_exists	(object *o, char *key);

array *array_create	(size_t size);
void array_destroy	(array **a);

#if (__STDC_VERSION__ >= 201112L)
#define array_set(_array, _index, _value) _Generic(_value,	\
	int:		array_set_int,				\
	double:		array_set_double,			\
	bool:		array_set_bool,				\
	char *:		array_set_string,			\
	object *:	array_set_object,			\
	array *:	array_set_array,			\
	default:	array_set_ptr)(_array, _index, _value)
#endif

int array_set_int	(array *a, uintmax_t index, int value);
int array_set_double	(array *a, uintmax_t index, double value);
int array_set_bool	(array *a, uintmax_t index, bool value);
int array_set_string	(array *a, uintmax_t index, char *value);
int array_set_object	(array *a, uintmax_t index, object *value);
int array_set_array	(array *a, uintmax_t index, array *value);
int array_set_ptr	(array *a, uintmax_t index, void *value);

#if (__STDC_VERSION__ >= 201112L)
#define array_get(_array, _index, _value) _Generic(_value,	\
	int *:		array_get_int,				\
	double *:	array_get_double,			\
	bool *:		array_get_bool,				\
	char **:	array_get_string,			\
	object **:	array_get_object,			\
	array **:	array_get_array,			\
	default:	array_get_ptr)(_array, _index, _value)
#endif

int array_get_int	(array *a, uintmax_t index, int *dest);
int array_get_double	(array *a, uintmax_t index, double *dest);
int array_get_bool	(array *a, uintmax_t index, bool *dest);
int array_get_string	(array *a, uintmax_t index, char **dest);
int array_get_object	(array *a, uintmax_t index, object **dest);
int array_get_array	(array *a, uintmax_t index, array **dest);
int array_get_ptr	(array *a, uintmax_t index, void **dest);

void array_remove	(array *a, uintmax_t index);
bool array_valid	(array *a, uintmax_t index);
uintmax_t array_next_free (array *a);

object *json_parse(FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* JSON11_H */
