#include "../all.h"
#include "../json11.h"

int main(void)
{
	object *o1 = object_create(64);

	printf("%d\n", object_add(o1, "Int", 12345));	/* Lies. It's really stored as a double */
	printf("%d\n", object_add(o1, "Double", -1.2345e-10));
	printf("%d\n", object_add(o1, "String", "Hello World!"));
	printf("%d\n", object_add(o1, "True", (bool)true));
	printf("%d\n", object_add(o1, "False", (bool)false));
	printf("%d\n", object_add(o1, "null", NULL));
	printf("%d\n", object_add(o1, "Empty Object", object_create(1)));
	printf("%d\n", object_add(o1, "Empty Array", array_create(1)));
	printf("%d\n", object_add(o1, "Object", object_create(32)));
	printf("%d\n", object_add(o1, "Array", array_create(32)));

	object_remove(o1, "null");

	object *o2;
	array *a1;
	int a;

	printf("%d\n", object_get(o1, "Int", &a));
	printf("%d\n", object_get(o1, "Object", &o2));
	printf("%d\n", object_get(o1, "Array", &a1));

	printf("%d\n", object_add(o2, "Indentation", NULL));
	printf("%d\n", object_add(o2, "Sub-Array", array_create(32)));

	array *a2;

	printf("%d\n", object_get(o2, "Sub-Array", &a2));

	printf("%d\n", array_set(a2, 0, "Install"));
	printf("%d\n", array_set(a2, 1, "Gentoo"));

	printf("%d\n", array_set(a1, 0, 12345));
	printf("%d\n", array_set(a1, 1, array_create(1)));
	printf("%d\n", array_set(a1, 2, object_create(1)));
	printf("%d\n", array_set(a1, 3, "Hello"));
	printf("%d\n", array_set(a1, 4, NULL));

	object *o3;

	printf("%d\n", array_get(a1, 2, &o3));

	printf("%d\n", object_add(o3, "Sub-Object", NULL));

	object_remove(o1, "Array");

//	object_print(stdout, o1, 0);

	/* Sub-objects, sub-arrays, and strings are automatically freed */
	object_destroy(&o1);

	return 0;
}
