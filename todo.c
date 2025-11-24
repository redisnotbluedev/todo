#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct task {
	char name[1024];
	bool done;
};

int main(int argc, char *argv[])
{
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <add|list|done|delete>\n", argv[0]);
		return 1;
	}
	
	char mode[20];
	strncpy(mode, argv[1], sizeof(mode) - 1);
	mode[sizeof(mode) - 1] = '\0';

	FILE *f;
	f = fopen("todo.dat", "r");

	struct task tasks[128];
	int tasks_length = 0;

	if (f != NULL) {
		char name[1024];
		int done;
		while (fscanf(f, " '%1023[^']'; %d;", name, &done) == 2) {
			tasks[tasks_length].done = done;
			strcpy(tasks[tasks_length].name, name);
			tasks_length++;
		}
		fclose(f);
	}

	if (!strcmp(mode, "add")) {
		f = fopen("todo.dat", "a");
		if (argc <= 2) {
			fprintf(stderr, "Usage: %s add <name>", argv[0]);
			return 1;
		}

		char name[1024] = "";
		for (int i = 2; i < argc; i++) {
			strcat(name, " ");
			strcat(name, argv[i]);
		}

		fprintf(f, "'%s'; 0;\n", name);
		printf("Successfully added task '%s' to tasklist.\n", name);
	}
	else if (!strcmp(mode, "list")) {
		if (tasks_length < 1) {
			printf("No tasks yet!\n");
		}
		else {
			for (int i = 0; i < tasks_length; i++) {
				struct task task = tasks[i];
				printf("%s %s\033[0m\n", task.done ? "\033[9m✓" : "✕", task.name);
			}
		}
	}
	else if (!strcmp(mode, "done")) {
		printf("Marking as done\n");
	}
	else if (!strcmp(mode, "delete")) {
		printf("Removing");
	}
	else {
		fprintf(stderr, "Usage: %s <add|list|done|delete>\n", argv[0]);
		return 1;
	}
}