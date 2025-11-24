#include <stdio.h>
#include <stdlib.h>
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
	bool needs_save = false;

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
		if (argc <= 2) {
			fprintf(stderr, "Usage: %s add <name>", argv[0]);
			return 1;
		}

		char name[1024] = "";
		for (int i = 2; i < argc; i++) {
			if (i > 2) {
				strcat(name, " ");
			}
			strcat(name, argv[i]);
		}

		strcpy(tasks[tasks_length].name, name);
		needs_save = true;
		printf("Added task %d: %s!\n", ++tasks_length, name);
	}
	else if (!strcmp(mode, "list")) {
		if (tasks_length < 1) {
			printf("No tasks yet!\n");
		}

		else {
			for (int i = 0; i < tasks_length; i++) {
				struct task task = tasks[i];
				printf("%d: %s \033[0m %s%s\033[0m\n",
					i + 1,
					task.done ? "\033[32m✓" : "\033[31m✕", // green/red
					task.done ? "\033[9m" : "", // strikethrough
					task.name
				);
			}
		}
	}
	else if (!strcmp(mode, "done")) {
		if (argc != 3) {
			fprintf(stderr, "Usage: %s done <task number>\n", argv[0]);
			return 1;
		}

		int i = atoi(argv[2]) - 1;
		tasks[i].done = 1;
		needs_save = true;
		printf("Marked task '%s' as done!\n", tasks[i].name);
	}
	else if (!strcmp(mode, "delete")) {
		if (argc != 3) {
			fprintf(stderr, "Usage: %s delete <task number>\n", argv[0]);
			return 1;
		}

		int i = atoi(argv[2]) - 1;
		printf("Deleted task '%s' from tasklist!\n", tasks[i].name);
		
		for (int j = i; j < tasks_length; j++) {
			tasks[j] = tasks[j + 1];
		}

		tasks_length--;
		needs_save = true;
	}
	else {
		fprintf(stderr, "Usage: %s <add|list|done|delete>\n", argv[0]);
		return 1;
	}

	if (needs_save) {
		f = fopen("todo.dat", "w");
		if (f == NULL) {
			fprintf(stderr, "Error: could not save data.");
			return 1;
		}
		else {
			for (int i = 0; i < tasks_length; i++) {
				fprintf(f, "'%s'; %d;\n", tasks[i].name, tasks[i].done);
			}
		}
		fclose(f);
	}
}