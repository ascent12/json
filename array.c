#include "array.h"

array *array_create(size_t size)
{
	array *a;

	if (!(a = calloc(1, sizeof *a)) ||
			!(a->data = calloc(size, sizeof *a->data)))
		exit(1);

	a->size = size;
	a->min = size;

	return a;
}

void array_destroy(array **a)
{
	for (int i = 0; i < (*a)->end; ++i)
		value_destroy((*a)->data + i);

	free((*a)->data);
	free(*a);
	*a = NULL;
}

value *array_get_value(array *a, int i)
{
	return (size_t)i <= a->size ? a->data + i : NULL;
}

void array_resize(array *a)
{
	size_t orig = a->size;

	if ((size_t)a->end == a->size) {
		a->size *= 1.5;
	} else if (a->end < a->size * 0.5) {
		a->size *= 0.66;
		if (a->size < a->min)
			a->size = a->min;
	}

	if (a->size == orig)
		return;

	if (!(a->data = realloc(a->data, sizeof *a->data * a->size)))
		exit(1);

	for (size_t i = orig; i < a->size; ++i) {
		a->data[i].type = type_undefined;
		a->data[i].p = NULL;
	}
}

void array_print(FILE *fp, array *a, int indent)
{
	if (a->end == 0) {
		fprintf(fp, "[ ]");
		return;
	}

	fputs("[\n", fp);

	for (int i = 0; i < a->end; ++i) {
		tab_indent(fp ,indent);

		value_print(fp, array_get_value(a, i), indent);

		if (i != a->end - 1)
			fputs(",\n", fp);
		else
			putc('\n', fp);
	}

	tab_indent(fp, indent - 1);

	putc(']', fp);

	if (indent == 0)
		putc('\n', fp);
}
