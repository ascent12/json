#include "object.h"

object *object_create(size_t size)
{
	object *o;

	if (!(o = calloc(1, sizeof *o)))
		exit(1);

	o->size = size;

	if (!(o->buckets = calloc(size, sizeof *o->buckets)))
		exit(1);

	return o;
}

static void free_pair(object_pair *pair)
{
	free(pair->name);
	value_destroy(&pair->value);
	free(pair);
}

void object_destroy(object **o)
{
	object_pair *node = (*o)->head;

	while (node) {
		object_pair *tmp = node->order;
		free_pair(node);
		node = tmp;
	}

	free((*o)->buckets);
	free(*o);
	*o = NULL;
}

static int str_hash(char *name, size_t max)
{
	size_t len = strlen(name);
	uint64_t sum = 0;
	int mult;

	for (size_t i = 0; i < len; ++i) {
		if (i % 4 == 0)
			mult = 1;

		sum += name[i] * mult;

		mult *= 256;
	}

	return sum % max;
}

value *object_add_general(object *o, char *name)
{
	int hash = str_hash(name, o->size);

	object_pair *pair;

	if (!(pair = calloc(1, sizeof *pair)) ||
			!(pair->name = calloc(strlen(name) + 1, 1)))
		exit(1);

	strcpy(pair->name, name);

	/* Adding to hashmap bucket */
	if (o->buckets[hash])
		pair->next = o->buckets[hash];
	o->buckets[hash] = pair;

	/* Adding to list of order added */
	if (!o->head) {
		o->head = pair;
		o->tail = pair;
	} else {
		o->tail->order = pair;
		o->tail = pair;
	}
	
	return &pair->value;
}

void object_remove(object *o, char *name)
{
	int hash = str_hash(name, o->size);

	/* Removing entry from hashmap bucket */
	for (object_pair **curr = o->buckets + hash; *curr; ) {
		object_pair *entry = *curr;

		if (strcmp(entry->name, name) == 0) {
			*curr = entry->next;
		} else {
			curr = &entry->next;
		}
	}

	/* Removing entry from list of order added */
	for (object_pair **curr = &o->head; *curr; ) {
		object_pair *entry = *curr;

		if (strcmp(entry->name, name) == 0) {
			*curr = entry->next;
			free_pair(entry);
		} else {
			curr = &entry->next;
		}
	}

	if (!o->head)
		o->tail = NULL;
}

object_pair *object_get_pair(object *o, char *name)
{
	object_pair *pair = o->buckets[str_hash(name, o->size)];

	while (!pair || strcmp(pair->name, name) != 0)
		pair = pair->next;

	return pair;
}

static void print_pair(FILE *fp, object_pair *pair, int indent)
{
	tab_indent(fp, indent);

	fprintf(fp, "\"%s\" : ", pair->name);

	value_print(fp, &pair->value, indent);
}

void object_print(FILE *fp, object *o, int indent)
{
	if (!o->head) {
		fprintf(fp, "{ }");
		return;
	}

	fputs("{\n", fp);

	object_pair *node = o->head;

	while (node) {
		print_pair(fp, node, indent + 1);

		if (node == o->tail)
			putc('\n', fp);
		else
			fputs(",\n", fp);

		node = node->order;
	}

	tab_indent(fp, indent);

	putc('}', fp);

	if (indent == 0)
		putc('\n', fp);
}

