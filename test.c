#include "object.h"
#include "array.h"

int main(void)
{

	object *o1 = object_create(64);

	object_add(o1, "Int", 12345);	/* Lies. It's really stored as a double */
	object_add(o1, "Double", -1.2345e-10);
	object_add(o1, "String", "Hello World!");
	object_add(o1, "True", (bool)true);
	object_add(o1, "False", (bool)false);
	object_add(o1, "null", NULL);
	object_add(o1, "Empty Object", object_create(1));
	object_add(o1, "Empty Array", array_create(1));
	object_add(o1, "Object", object_create(32));
	object_add(o1, "Array", array_create(32));

	object *o2;
	array *a1;

	object_get(o1, "Object", &o2);
	object_get(o1, "Array", &a1);

	object_add(o2, "Indentation", NULL);
	object_add(o2, "Sub-Array", array_create(32));

	array *a2;

	object_get(o2, "Sub-Array", &a2);

	array_push(a2, "Install");
	array_push(a2, "Gentoo");

	array_push(a1, 12345);
	array_push(a1, array_create(1));
	array_push(a1, object_create(1));
	array_push(a1, "Hello");
	array_push(a1, NULL);

	object *o3;

	array_get_index(a1, 2, &o3);

	object_add(o3, "Sub-Object", NULL);

	object_print(stdout, o1, 0);

	/* Sub-objects, sub-arrays, and strings are automatically freed */
	object_destroy(&o1);

	return 0;
}
