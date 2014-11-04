#ifndef OBJECT_H
#define OBJECT_H

#include "all.h"
#include "value.h"

object *object_create(size_t size);

void object_destroy(object **o);

value *object_add_general(object *o, char *name);

#define object_add(_object, _name, _data) \
	value_set(object_add_general(_object, _name), _data)

void object_remove(object *o, char *name);

bool object_exists(object *o, char *name);

object_pair *object_get_pair(object *o, char *name);

#define object_get(_object, _name, _destination) \
	value_get(&object_get_pair(_object, _name)->value, _destination)

void object_print(FILE *fp, object *o, int indent);

#endif
