#include "all.h"
#include "json11.h"
#include "value.h"

JSON11_EXPORT 
array *array_create(size_t size)
{
	array *a;

	if (size == 0)
		size = 1;
	else if (size > SIZE_MAX / sizeof *a->data)
		size = SIZE_MAX / sizeof *a->data;

	a = calloc(1, sizeof *a);
	if (!a)
		return NULL;

	a->data = calloc(size, sizeof *a->data);
	if (!a->data) {
		free(a);
		return NULL;
	}

	a->size = size;

	return a;
}

JSON11_EXPORT 
void array_destroy(array **a)
{
	for (size_t i = 0; i < (*a)->size; ++i)
		value_destroy((*a)->data + i);

	free((*a)->data);
	free(*a);
	*a = NULL;
}

static int array_resize(array *a, uintmax_t index)
{
	size_t new;
	size_t old;
	struct value *tmp;

	if (index < a->size)
		return JSON11_ERR_OK;

	if (index > SIZE_MAX / sizeof *a->data)
		return JSON11_ERR_TOOBIG;

	old = a->size;

	new = index * 1.5;
	if (new > SIZE_MAX / sizeof *a->data)
		new = SIZE_MAX / sizeof *a->data;

	tmp = realloc(a->data, new * sizeof *a->data);
	if (!tmp) {
		/* We DON'T free the array here.
		 * It is still valid, but setting the variable failed */
		return JSON11_ERR_ALLOC;
	}

	for (size_t i = old; i < new; ++i) {
		tmp[i].type = type_undefined;
		tmp[i].p = NULL;
	}

	a->data = tmp;
	a->size = new;

	return JSON11_ERR_OK;
}

JSON11_EXPORT 
int array_set_int(array *a, uintmax_t index, int value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_set_double(array *a, uintmax_t index, double value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_set_bool(array *a, uintmax_t index, bool value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_set_string(array *a, uintmax_t index, char *value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_set_object(array *a, uintmax_t index, object *value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_set_array(array *a, uintmax_t index, array *value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_set_ptr(array *a, uintmax_t index, void *value) {
	int ret;

	ret = array_resize(a, index);
	if (ret != JSON11_ERR_OK)
		return ret;

	return value_set(a->data + index, value);
}

JSON11_EXPORT 
int array_get_int(array *a, uintmax_t index, int *dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
int array_get_double(array *a, uintmax_t index, double *dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
int array_get_bool(array *a, uintmax_t index, bool *dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
int array_get_string(array *a, uintmax_t index, char **dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
int array_get_object(array *a, uintmax_t index, object **dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
int array_get_array(array *a, uintmax_t index, array **dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
int array_get_ptr(array *a, uintmax_t index, void **dest)
{
	if (index >= a->size)
		return JSON11_ERR_UNDEF;
	return value_get(a->data + index, dest);
}

JSON11_EXPORT 
void array_remove(array *a, uintmax_t index)
{
	value_destroy(a->data + index);
}

JSON11_EXPORT 
bool array_valid(array *a, uintmax_t index)
{
	return value_is_valid(a->data + index);
}

JSON11_EXPORT 
uintmax_t array_next_free(array *a)
{
	for (size_t i = 0; i < a->size; ++i)
		if (!array_valid(a, i))
			return i;
	return a->size;
}

/*void array_print(FILE *fp, array *a, int indent)
{
	if (a->end == 0) {
		fprintf(fp, "[ ]");
		return;
	}

	fputs("[\n", fp);

	for (int i = 0; i < a->end; ++i) {
		tab_indent(fp ,indent);

		value_print(fp, a->data + i, indent);

		if (i != a->end - 1)
			fputs(",\n", fp);
		else
			putc('\n', fp);
	}

	tab_indent(fp, indent - 1);

	putc(']', fp);

	if (indent == 0)
		putc('\n', fp);
}*/
