#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_TASKS 10
#define CHAR_LENGTH 100

typedef struct {
    char title[CHAR_LENGTH];
    } Tasks;

void get_input(char *buffer, size_t size);
void get_task_count(int *task_count);
int create_todo(char *todo_name);

int main(int argc, char **argv) {    
    if (argc < 2) {
	printf("Program usage: %s <todo_list_name> \n", argv[0]);
	return 1;
	}
    
    create_todo(argv[1]);
    return 0;
    }

void get_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL){
	printf("Program terminated. \n");
	exit(EXIT_FAILURE);
	}
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
	buffer[len - 1] = '\0';
	}
    else if (getchar() == '\n');
    else {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	fprintf(stderr, "Buffer overflow. \n");
	exit(EXIT_FAILURE);
	}
    return;
    }

void get_task_count(int *task_count) {
    char buffer[3];
    get_input(buffer, sizeof(buffer));
    
    char *end_ptr;
    errno = 0;
    *task_count = (int)strtol(buffer, &end_ptr, 10);
    if (end_ptr == buffer) {
	fprintf(stderr, "Invalid entry. \n");
	exit(EXIT_FAILURE);
	}
    if (errno == ERANGE) {
	perror("Out of range. \n");
	exit(EXIT_FAILURE);
	}
    if (*end_ptr != '\0') {
	fprintf(stderr, "Non-integer character detected in input.");
	exit(EXIT_FAILURE);
	}    
    return;
    }

int create_todo(char *todo_name) {
    printf("======== To-do List App ======== \n");
    printf("Enter the number of tasks (max of 10): ");
    
    int task_count;
    get_task_count(&task_count);    
    Tasks task_list[task_count];
        
    for (int i = 0; i < task_count; i++) {
	printf("Enter task %d \n", i + 1);
	if (fgets(task_list[i].title, CHAR_LENGTH, stdin) == NULL) {
	    fprintf(stderr, "fgets failed. \n");
	    return -1;
	    }
	int c;
	char *newline = strchr(task_list[i].title, '\n');
	if (newline != NULL) {
	    *newline = '\0';
	    }
	else {
	    while ((c = getchar()) != '\n' && c !=  EOF);	    
	    } 
	}

    FILE *f = fopen(todo_name, "wb");
    if (!f) {
	fprintf(stderr, "Unable to create file. \n");
	exit(EXIT_FAILURE);
	}	
    for (int i = 0; i < task_count; i++) {
	fprintf(f, "%s\n", task_list[i].title);
	}	
    printf("List created successfully! \n");

    return 1;
    }
