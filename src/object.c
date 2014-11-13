/*
 * Copyright (c) 2014, Scott Anderson <ascent@cock.li>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "all.h"
#include "json11.h"
#include "value.h"

JSON11_EXPORT 
object *object_create(size_t size)
{
	object *o;

	if (size == 0)
		size = 1;
	else if (size > SIZE_MAX / sizeof *o->buckets)
		size = SIZE_MAX / sizeof *o->buckets;

	o = calloc(1, sizeof *o + sizeof *o->buckets * size);
	if (!o)
		return NULL;

	o->size = size;

	return o;
}

static void free_pair(struct object_pair *pair)
{
	value_destroy(&pair->value);
	free(pair);
}

JSON11_EXPORT 
void object_destroy(object **o)
{
	struct object_pair *node = (*o)->head;

	while (node) {
		struct object_pair *tmp = node->order;
		free_pair(node);
		node = tmp;
	}

	free(*o);
	*o = NULL;
}

static int str_hash(char *name, size_t max)
{
	size_t len = strlen(name);
	uint64_t sum = 0;
	int mult = 1;

	for (size_t i = 0; i < len; ++i) {
		if (i % 4 == 0)
			mult = 1;

		sum += name[i] * mult;

		mult *= 256;
	}

	return sum % max;
}

static struct object_pair *get_object_pair(object *o, char *key)
{
	int hash;

	hash = str_hash(key, o->size);

	for (struct object_pair *pair = o->buckets[hash]; pair; pair = pair->next)
		if (strcmp(pair->key, key) == 0)
			return pair;

	return NULL;
}

static int object_pair_create(object *o, char *key, struct value **ptr)
{
	struct object_pair *pair;
	int hash;

	hash = str_hash(key, o->size);

	if (get_object_pair(o, key) != NULL)
		return JSON11_ERR_EXISTS;

	pair = calloc(1, sizeof *pair + strlen(key) + 1);
	if (!pair) {
		*ptr = NULL;
		return JSON11_ERR_ALLOC;
	}

	strcpy(pair->key, key);

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

	*ptr = &pair->value;

	return JSON11_ERR_OK;
}

JSON11_EXPORT 
int object_add_int(object *o, char *key, int value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_add_double(object *o, char *key, double value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_add_bool(object *o, char *key, bool value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_add_string(object *o, char *key, char *value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_add_object(object *o, char *key, object *value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_add_array(object *o, char *key, array *value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_add_ptr(object *o, char *key, void *value)
{
	int ret;
	struct value *ptr;

	ret = object_pair_create(o, key, &ptr);
	if (ret != JSON11_ERR_OK)
		return ret;
	return value_set(ptr, value);
}

JSON11_EXPORT 
int object_get_int(object *o, char *key, int *dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (!pair)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
int object_get_double(object *o, char *key, double *dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (!pair)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
int object_get_bool(object *o, char *key, bool *dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (!pair)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
int object_get_string(object *o, char *key, char **dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (!pair)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
int object_get_object(object *o, char *key, object **dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (pair == NULL)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
int object_get_array(object *o, char *key, array **dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (!pair)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
int object_get_ptr(object *o, char *key, void **dest)
{
	struct object_pair *pair;
	
	pair = get_object_pair(o, key);
	if (!pair)
		return JSON11_ERR_UNDEF;
	return value_get(&pair->value, dest);
}

JSON11_EXPORT 
void object_remove(object *o, char *key)
{
	int hash = str_hash(key, o->size);

	/* Removing entry from hashmap bucket */
	for (struct object_pair **curr = o->buckets + hash; *curr; ) {
		struct object_pair *entry = *curr;

		if (strcmp(entry->key, key) == 0) {
			*curr = entry->next;
		} else {
			curr = &entry->next;
		}
	}

	/* Removing entry from list of order added */
	for (struct object_pair **curr = &o->head, *prev = NULL; *curr; ) {
		struct object_pair *entry = *curr;

		if (strcmp(entry->key, key) == 0) {
			if (o->tail == entry)
				o->tail = prev;

			prev = *curr;
			*curr = entry->order;

			free_pair(entry);
		} else {
			prev = *curr;
			curr = &entry->order;
		}
	}

	if (!o->head)
		o->tail = NULL;
}

JSON11_EXPORT 
bool object_exists(object *o, char *key)
{
	return get_object_pair(o, key) != NULL;
}
/*
static void print_pair(FILE *fp, struct object_pair *pair, int indent)
{
	tab_indent(fp, indent);

	fprintf(fp, "\"%s\" : ", pair->key);

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
}*/
