#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

typedef struct {
    char title[32];
    } Tasks;

Tasks get_task(int count) {        
    Tasks task[count];
    for (int i = 0; i < count; i++) {
	if (fgets(task[0].title, sizeof(Tasks), stdin) == NULL) {
	    fprintf(stderr, "fgets failed. \n");
	    exit(EXIT_FAILURE);
	    }	    
	printf("Enter task %d \n", i + 1);	
	char *newline = strchr(task[0].title, '\n');
	if (newline != NULL) {
	    *newline = '\0';	
	    }
   	else{
	    int c; 
	    while ((c = getchar()) != '\n' && c != EOF);
	    }
	}	
    return *task;
    }	
	
int main(int argc, char **argv) {
    
    return 0;
    }
