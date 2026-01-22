#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PROGRAM_FAIL -1
#define CREATE_SUCCESS 1
#define READ_SUCCESS 2
#define ADD_SUCCESS 3

typedef struct {
    char chore_name[64];
    } Chores;

Chores *chore_arr = NULL;
const char *filename = "data.bin";
const char *count = "count.bin";

void create_list(char *command, uint32_t chore_count) {
    FILE *f;
        
    if (strcmp(command, "create") == 0) {
	f = fopen(filename, "wb");
	}
    else if (strcmp(command, "add") == 0) {
	f = fopen(filename, "ab");
	}
    if (!f) {
	perror("Failed to create file: ");
	return;
	}    
        
    uint32_t len;
    for (int i = 0; i < chore_count; i++) {
	len = strlen(chore_arr[i].chore_name);
	fwrite(&len, sizeof(uint32_t), 1, f);
	fwrite(chore_arr[i].chore_name, sizeof(char), len, f);
	}	
    fclose(f);
    }

void read_list(void) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
	perror("Failed to open file: ");
	return;	
	}
    uint32_t len;
    char *buffer;
    while(fread(&len, sizeof(uint32_t), 1, f)) {
	buffer = malloc(len + 1);
	if(!buffer) {
	    perror("Memory allocation failed: ");
	    return;
	    }
	fread(buffer, sizeof(char), len, f);
	buffer[len] = '\0';
	printf("%s \n", buffer);
	}
    fclose(f);
    }

int main(int argc, char **argv) {    
    uint32_t chore_count;
    int create, add;
    chore_count = (uint32_t)argc - 2;
    chore_arr = malloc(sizeof(Chores) * chore_count);
    if (!chore_arr) {
	perror("Memory allocation failed: ");
	return PROGRAM_FAIL;
	}
    
    for (int i = 0; i < chore_count; i++) {
	strcpy(chore_arr[i].chore_name, argv[i + 2]);
	}
    
    create = strcmp(argv[1], "create");
    add = strcmp(argv[1], "add");
    if ((create == 0) || (add == 0)) {
	create_list(argv[1], chore_count);
	}
    else if (strcmp(argv[1], "read") == 0) {
	read_list();
	}
    return 0;
    }
