#include "all.h"
#include "json11.h"

/*
 * <object> ::= "{" [ [ <pair> "," ]* <pair> ] "}"
 * <pair> ::= <string> ":" <value>
 * <array> ::= "[" [ [ <value> "," ]* <value> ] "]"
 * <value> ::= <object> | <array> | <string> | <number> | "true" | "false" | "null"
 * <string> ::= '"' [ character | <escape> ] '"'
 * <escape> ::= "\" ( '"' | "\" | "/" | "b" | "f" | "n" | "r" | "t" | "u"<hex><hex><hex><hex> )
 * <number> ::= [ "-" ] ( "0" | ( <digit-0> <digit>* ) ) [ "." <digit>+ ] [ ( "e" | "E" ) [ "+" | "-" ] <digit>+ ]
 * <digit-0> ::= "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
 * <digit> :: <digit-0> | "0"
 */

#define BUF_SIZE 1024

/* TODO: Remove globals. Maybe have a JSON parser struct to pass around */
static int row = 1;
static int col = 0;

static int fpeek(FILE *fp)
{
	int c;
	
	c = fgetc(fp);
	ungetc(c, fp);

	return c;
}

static int get_char(FILE *fp)
{
	int c = fgetc(fp);

	++col;
	if (c == '\n') {
		++row;
		col = 0;
	}

	return c;
}

static void unget_char(int c, FILE *fp)
{
	ungetc(c, fp);
	--col;
}

static void skip_whitespace(FILE *fp)
{
	while (isspace(fpeek(fp)))
		get_char(fp);
}

static bool parse_escape(FILE *fp, char **dest)
{
	get_char(fp);

	int c = get_char(fp);
	char *ptr = *dest;

	switch (c) {
	case '"':
		*ptr++ = '"';
		break;
	case '\\':
		*ptr++ = '\\';
		break;
	case '/':
		*ptr++ = '/';
		break;
	case 'b':
		*ptr++ = '\b';
		break;
	case 'f':
		*ptr++ = '\f';
		break;
	case 'n':
		*ptr++ = '\n';
		break;
	case 'r':
		*ptr++ = '\r';
		break;
	case 't':
		*ptr++ = '\t';
		break;
	case 'u':
		/* TODO: Unicode escapes */
		/* Fallthrough, for now */
	default:
		fprintf(stderr, "[%d:%d] Unrecognised escape '\\%c'\n", row, col, c);
		return 0;
	}

	*dest = ptr;
	return 1;
}

static bool parse_string(FILE *fp, char *dest)
{
	skip_whitespace(fp);

	int c;

	if ((c = get_char(fp)) != '"') {
		fprintf(stderr, "[%d:%d] Expected '\"', but got '%c'\n", row, col, c);
		return 0;
	}

	while ((c = get_char(fp)) != '"') {
		if (fpeek(fp) == '\\' && !parse_escape(fp, &dest))
			return 0;

		*dest++ = c;
	}

	*dest++ = '\0';

	return 1;
}

static bool parse_true(FILE *fp)
{
	if (get_char(fp) != 't' ||
	    get_char(fp) != 'r' ||
	    get_char(fp) != 'u' ||
	    get_char(fp) != 'e') {
		fprintf(stderr, "[%d:%d] Unexpected symbol\n", row, col);	/* TODO: Better error message */
		return 0;
	}

	return 1;
}

static bool parse_false(FILE *fp)
{
	if (get_char(fp) != 'f' ||
	    get_char(fp) != 'a' ||
	    get_char(fp) != 'l' ||
	    get_char(fp) != 's' ||
	    get_char(fp) != 'e') {
		fprintf(stderr, "[%d:%d] Unexpected symbol\n", row, col);	/* TODO: Better error message */
		return 0;
	}

	return 1;
}

static bool parse_null(FILE *fp)
{
	if (get_char(fp) != 'n' ||
	    get_char(fp) != 'u' ||
	    get_char(fp) != 'l' ||
	    get_char(fp) != 'l') {
		fprintf(stderr, "[%d:%d] Unexpected symbol\n", row, col);	/* TODO: Better error message */
		return 0;
	}

	return 1;
}

/* Kind of unnecessary, since this just checks for the format strtod expects,
 * but this allows for better error messages, and to be a bit stricter
 * on the allowed inputs
 */
static bool parse_number(FILE *fp, double *dest)
{
	char buffer[BUF_SIZE];
	int n = 0;

	if ((buffer[n] = get_char(fp)) != '0') {
		do
			/* This will read one character past the number */
			buffer[++n] = get_char(fp);
		while (isdigit(buffer[n]));
	} else {
		if ((buffer[++n] = get_char(fp)) != '.' && buffer[n] != 'e' && buffer[n] != 'E' &&
				buffer[n] != ',' && !isspace(buffer[n])) {
			fprintf(stderr, "[%d:%d] Leading zero is disallowed\n", row, col);
			return 0;
		}
	}

	/* Number contains no decimal or exponent */
	if (buffer[n] == ',')
		unget_char(',', fp);
	
	if (buffer[n] == '.') {
		do
			buffer[++n] = get_char(fp);
		while (isdigit(buffer[n]));

		if (!isdigit(buffer[n-1])) {
			fprintf(stderr, "[%d:%d] Must have at least one digit after decimal point\n",
					row, col);
			return 0;
		}

		/* Number contains no exponent */
		if (buffer[n] == ',')
			unget_char(',', fp);
	}

	if (buffer[n] == 'e' || buffer[n] == 'E') {
		if ((buffer[++n] = get_char(fp)) != '+' && buffer[n] != '-' && !isdigit(buffer[n])) {
			fprintf(stderr, "[%d:%d] Invalid exponent '%c'\n", row, col, buffer[n]);
			return 0;
		}

		do
			buffer[++n] = get_char(fp);
		while (isdigit(buffer[n]));

		if (buffer[n] == ',')
			unget_char(',', fp);
	}

	buffer[n] = '\0';

	*dest = strtod(buffer, NULL);

	return 1;
}

static bool parse_object(FILE *fp, object *dest);

static bool parse_array(FILE *fp, array *dest)
{
	skip_whitespace(fp);

	int c;

	if ((c = get_char(fp)) != '[') {
		fprintf(stderr, "[%d:%d] Expected '[', but got '%c'\n", row, col, c);
		return 0;
	}

	skip_whitespace(fp);

	while ((c = get_char(fp)) != ']') {
		unget_char(c, fp);
		skip_whitespace(fp);

		/* TODO: Refactor into a function or something, if it's possible */
		switch (c = fpeek(fp)) {
		case '{':
			;
			object *obj = object_create(64); /* TODO: Replace magic number */
			if (!parse_object(fp, obj)) {
				object_destroy(&obj);
				return 0;
			}
			array_set(dest, array_next_free(dest), obj);
			break;
		case '[':
			;
			array *arr = array_create(10); /* TODO: Replace magic number */
			if (!parse_array(fp, arr)) {
				array_destroy(&arr);
				return 0;
			}
			array_set(dest, array_next_free(dest), arr);
			break;
		case '"':
			;
			char string[BUF_SIZE] = {0};
			if (!parse_string(fp, string))
				return 0;
			array_set(dest, array_next_free(dest), (char*)string);
			break;
		case 't':
			if (!parse_true(fp))
				return 0;
			array_set(dest, array_next_free(dest), (bool)true);
			break;
		case 'f':
			if (!parse_false(fp))
				return 0;
			array_set(dest, array_next_free(dest), (bool)false);
			break;
		case 'n':
			if (!parse_null(fp))
				return 0;
			array_set(dest, array_next_free(dest), NULL);
			break;
		default:
			if (c == '-' || isdigit(c)) {
				double d;
				if (!parse_number(fp, &d))
					return 0;
				array_set(dest, array_next_free(dest), d);
				break;
			}

			fprintf(stderr, "[%d:%d] Unrecognised symbol '%c'\n", row, col, c);
			return 0;
		}

		skip_whitespace(fp);

		if ((c = get_char(fp)) != ',')
			break;

		skip_whitespace(fp);
	}

	if (c != ']') {
		fprintf(stderr, "[%d:%d] Expected ']', but got '%c'\n", row, col, c);
		return 0;
	}

	return 1;
}

static bool parse_object(FILE *fp, object *dest)
{
	skip_whitespace(fp);

	int c;

	if ((c = get_char(fp)) != '{') {
		fprintf(stderr, "[%d:%d] Expected '{', but got '%c'\n", row, col, c);
		return 0;
	}

	skip_whitespace(fp);

	while ((c = get_char(fp)) == '"') {
		unget_char('"', fp);

		char key[BUF_SIZE];
		if (!parse_string(fp, key))
			return 0;

		skip_whitespace(fp);

		if ((c = get_char(fp)) != ':') {
			fprintf(stderr, "[%d:%d] Expected ':', but got '%c'\n", row, col, c);
			return 0;
		}

		skip_whitespace(fp);

		/* TODO: Refactor into a function or something, if it's possible */
		switch (c = fpeek(fp)) {
		case '{':
			;
			object *obj = object_create(64); /* TODO: Replace magic number */
			if (!parse_object(fp, obj)) {
				object_destroy(&obj);
				return 0;
			}
			object_add(dest, key, obj);
			break;
		case '[':
			;
			array *arr = array_create(10); /* TODO: Replace magic number */
			if (!parse_array(fp, arr)) {
				array_destroy(&arr);
				return 0;
			}
			object_add(dest, key, arr);
			break;
		case '"':
			;
			char string[BUF_SIZE];
			if (!parse_string(fp, string))
				return 0;
			object_add(dest, key, (char*)string);
			break;
		case 't':
			if (!parse_true(fp))
				return 0;
			object_add(dest, key, (bool)true);
			break;
		case 'f':
			if (!parse_false(fp))
				return 0;
			object_add(dest, key, (bool)false);
			break;
		case 'n':
			if (!parse_null(fp))
				return 0;
			object_add(dest, key, NULL);
			break;
		default:
			if (c == '-' || isdigit(c)) {
				double d;
				if (!parse_number(fp, &d))
					return 0;
				object_add(dest, key, d);
				break;
			}

			fprintf(stderr, "[%d:%d] Unrecognised symbol '%c'\n", row, col, c);
			return 0;
		}

		skip_whitespace(fp);

		if ((c = get_char(fp)) != ',')
			break;

		skip_whitespace(fp);
	}

	if (c != '}') {
		fprintf(stderr, "[%d:%d] Expected '}', but got '%c'\n", row, col, c);
		return 0;
	}

	return 1;
}

JSON11_EXPORT 
object *json_parse(FILE *fp)
{
	object *obj = object_create(64);

	if (!parse_object(fp, obj)) {
		object_destroy(&obj);
		return NULL;
	}

	return obj;
}
