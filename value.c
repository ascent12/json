#include "value.h"

void value_set_int(value *v, int value)
{
	value_set_double(v, (double)value);
}

void value_set_double(value *v, double value)
{
	v->type = type_double;
	v->d = value;
}

void value_set_bool(value *v, bool value)
{
	v->type = type_bool;
	v->b = value;
}

void value_set_string(value *v, char *value)
{
	if (v->type == type_string && v->s)
		free(v->s);

	v->type = type_string;
	if (!(v->s = calloc(strlen(value) + 1, 1)))
		exit(1);
	strcpy(v->s, value);
}

void value_set_object(value *v, object *value)
{
	v->type = type_object;
	v->o = value;
}

void value_set_array(value *v, array *value)
{
	v->type = type_array;
	v->a = value;
}

void value_set_ptr(value *v, void *value)
{
	v->type = type_ptr;
	v->p = value;
}

int value_get_int(value *v, int *dest)
{
	if (dest)
		*dest = (int)v->d;
	return (int)v->d;
}

double value_get_double(value *v, double *dest)
{
	if (dest)
		*dest = v->d;
	return v->d;
}

bool value_get_bool(value *v, bool *dest)
{
	if (dest)
		*dest = v->b;
	return v->b;
}

char *value_get_string(value *v, char **dest)
{
	if (dest)
		*dest = v->s;
	return v->s;
}

object *value_get_object(value *v, object **dest)
{
	if (dest)
		*dest = v->o;
	return v->o;
}

array *value_get_array(value *v, array **dest)
{
	if (dest)
		*dest = v->a;
	return v->a;
}

void *value_get_ptr(value *v, void **dest)
{
	if (dest)
		*dest = v->p;
	return v->p;
}

void value_destroy(value *v)
{
	switch (v->type) {
		case type_string:
			free(v->s);
			break;
		case type_object:
			object_destroy(&v->o);
			break;
		case type_array:
			array_destroy(&v->a);
			break;
		default:
			break;
	}
}

inline void tab_indent(FILE *fp, int indent)
{
	for (int i = 0; i < indent; ++i)
		fprintf(fp, "    ");
}

void value_print(FILE *fp, value *v, int indent)
{
	switch (v->type) {
	case type_double:
		fprintf(fp, "%g", v->d);
	       	break;
	case type_bool:
		fprintf(fp, "%s", v->b ? "true" : "false");
		break;
	case type_string:
		fprintf(fp, "\"%s\"", v->s);
		break;
	case type_object:
		object_print(fp, v->o, indent);
		break;
	case type_array:
		array_print(fp, v->a, indent + 1);
		break;
	case type_ptr:
		v->p ? fprintf(fp, "%p", v->p) : fprintf(fp, "null");
		break;
	default:
		break;
	}
}
