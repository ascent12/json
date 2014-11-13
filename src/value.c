#include "all.h"
#include "json11.h"
#include "value.h"

void value_destroy(struct value *v)
{
	switch (v->type) {
		case type_string:
			free(v->s);
			v->s = NULL;
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
	v->type = type_undefined;
}

bool value_is_valid(struct value *v)
{
	return v->type != type_undefined;
}

int value_set_int(struct value *v, int value)
{
	value_destroy(v);

	v->type = type_int;
	v->i = value;

	return JSON11_ERR_OK;
}

int value_set_double(struct value *v, double value)
{
	value_destroy(v);

	v->type = type_double;
	v->d = value;

	return JSON11_ERR_OK;
}

int value_set_bool(struct value *v, bool value)
{
	value_destroy(v);

	v->type = type_bool;
	v->b = value;

	return JSON11_ERR_OK;
}

int value_set_string(struct value *v, char *value)
{
	value_destroy(v);

	v->type = type_string;
	v->s = calloc(strlen(value) + 1, 1);
	if (!v->s)
		return JSON11_ERR_ALLOC;
	strcpy(v->s, value);

	return JSON11_ERR_OK;
}

int value_set_object(struct value *v, object *value)
{
	value_destroy(v);

	v->type = type_object;
	v->o = value;

	return JSON11_ERR_OK;
}

int value_set_array(struct value *v, array *value)
{
	value_destroy(v);

	v->type = type_array;
	v->a = value;

	return JSON11_ERR_OK;
}

int value_set_ptr(struct value *v, void *value)
{
	value_destroy(v);

	v->type = type_ptr;
	v->p = value;

	return JSON11_ERR_OK;
}

int value_get_int(struct value *v, int *dest)
{
	if (!value_is_valid(v))
		return JSON11_ERR_UNDEF;

	*dest = v->i;

	return JSON11_ERR_OK;
}

int value_get_double(struct value *v, double *dest)
{
	if (!value_is_valid(v))
		return JSON11_ERR_UNDEF;

	*dest = v->d;

	return JSON11_ERR_OK;
}

int value_get_bool(struct value *v, bool *dest)
{
	if (!value_is_valid(v))
		return JSON11_ERR_UNDEF;

	*dest = v->b;

	return JSON11_ERR_OK;
}

int value_get_string(struct value *v, char **dest)
{
	if (!value_is_valid(v)) {
		*dest = NULL;
		return JSON11_ERR_UNDEF;
	}

	*dest = v->s;

	return JSON11_ERR_OK;
}

int value_get_object(struct value *v, object **dest)
{
	if (!value_is_valid(v)) {
		*dest = NULL;
		return JSON11_ERR_UNDEF;
	}

	*dest = v->o;

	return JSON11_ERR_OK;
}

int value_get_array(struct value *v, array **dest)
{
	if (!value_is_valid(v)) {
		*dest = NULL;
		return JSON11_ERR_UNDEF;
	}

	*dest = v->a;

	return JSON11_ERR_OK;
}

int value_get_ptr(struct value *v, void **dest)
{
	if (!value_is_valid(v)) {
		*dest = NULL;
		return JSON11_ERR_UNDEF;
	}

	*dest = v->p;

	return JSON11_ERR_OK;
}

/*inline void tab_indent(FILE *fp, int indent)
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
}*/
