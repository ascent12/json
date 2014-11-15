#include "../src/json11.h"

#include <assert.h>

int main(void)
{
	object *obj = object_create(64);
	assert(obj != NULL);

	/* Testing adding all of the types */
	assert(object_add(obj, "integer", 12345) == JSON11_ERR_OK);
	assert(object_add(obj, "double", -1.2345e4) == JSON11_ERR_OK);
	assert(object_add(obj, "string", "abcdef") == JSON11_ERR_OK);
	assert(object_add(obj, "true", (bool)true) == JSON11_ERR_OK);
	assert(object_add(obj, "false", (bool)false) == JSON11_ERR_OK);
	assert(object_add(obj, "null", NULL) == JSON11_ERR_OK);

	object *inner_object = object_create(32);
	array *inner_array = array_create(32);

	assert(inner_object != NULL);
	assert(inner_array != NULL);

	assert(object_add(obj, "object", inner_object) == JSON11_ERR_OK);
	assert(object_add(obj, "array", inner_array) == JSON11_ERR_OK);

//	object_destroy(&obj);
//	assert(obj == NULL);
}
