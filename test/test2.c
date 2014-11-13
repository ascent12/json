#include "../json11.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s input_file.json\n", argv[0]);
		return 1;
	}

	FILE *fp = fopen(argv[1], "r");

	if (!fp) {
		fprintf(stderr, "Unable to open %s\n", argv[1]);
		return 1;
	}

	object *json = json_parse(fp);

	fclose(fp);

	if (json) {
//		object_print(stdout, json, 0);
		object_destroy(&json);
		return 0;
	}

	return 1;
}
