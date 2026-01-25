#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define USER_ERROR -2
#define SYSTEM_ERROR -1
#define CREATE_SUCCESS 1
#define READ_SUCCESS 2
#define ADD_SUCCESS 3
#define DELETE_SUCCESS 4
#define DELETE_FAIL 5

typedef struct {
    char chore_name[64];
    } Chores;

Chores *chore_arr = NULL;
const char *filename = "data.bin";

void create_list(char *command, uint32_t chore_count) {
    FILE *f;
        
    if (strcmp(command, "create") == 0) {
	f = fopen(filename, "wb");
	}
    else if (strcmp(command, "add") == 0) {
	f = fopen(filename, "ab");
	}
    if (!f) {
	perror("Failed to create file");
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
    int i = 1;
    
    if (!f) {
	perror("Failed to open file");
	return;	
	}
    uint32_t len;
    char *buffer;
    while(fread(&len, sizeof(uint32_t), 1, f)) {
	buffer = malloc(len + 1);
	if(!buffer) {
	    perror("Memory allocation failed");
	    return;
	    }
	fread(buffer, sizeof(char), len, f);
	buffer[len] = '\0';
	printf("Item %d: \t%s \n", i, buffer);
	i++;
	}
    fclose(f);
    }

void delete_item(int item) {        
    FILE *f, *f_copy;
    uint32_t len;
    int i = 0;
    Chores chore;
    
    f = fopen(filename, "rb");
    f_copy = fopen("temp.bin", "wb");
    if (!f || !f_copy) {
	perror("Failed to open file");
	return;
	}
    while (fread(&len, sizeof(uint32_t), 1, f)) {
	fread(chore.chore_name, sizeof(char), len, f);
	i++;
	if (i == item) {
	    chore.chore_name[len] = '\0';
	    printf("Item %d(%s) successfully deleted. \n", i, chore.chore_name);
	    continue;
	    }
	fwrite(&len, sizeof(uint32_t), 1, f_copy);
	fwrite(chore.chore_name, sizeof(char), len, f_copy);
	}
    if (item > i) printf("Item %d does not exist. \n", item);
    remove(filename);
    rename("temp.bin", filename);
    fclose(f_copy);
    fclose(f);
    return;
    }

int main(int argc, char **argv) {
    if (argc < 2) {
	printf("Program usage: %s <command> <arguments> \n", argv[0]);
	return USER_ERROR;
	} 
    uint32_t chore_count;
    int create, add, read, delete;
    chore_count = (uint32_t)argc - 2;
    chore_arr = malloc(sizeof(Chores) * chore_count);
    if (!chore_arr) {
	perror("Memory allocation failed: ");
	return SYSTEM_ERROR;
	}
    
    for (int i = 0; i < chore_count; i++) {
	strcpy(chore_arr[i].chore_name, argv[i + 2]);
	}
    
    create = strcmp(argv[1], "create");
    add = strcmp(argv[1], "add");
    read = strcmp(argv[1], "read");
    delete = strcmp(argv[1], "delete");
    
    if ((create == 0) || (add == 0)) {
	create_list(argv[1], chore_count);
	if (create == 0) {
	    printf("New list created successfully. \n");
	    return CREATE_SUCCESS;
	    }
	if (add == 0) {
	    printf("List updated successfully. \n");
	    return ADD_SUCCESS;
	    }
	}
    else if (read == 0) {
	read_list();
	return READ_SUCCESS;
	}
    else if (delete == 0) {
	if (argc == 2) {
	    remove(filename);
	    printf("List deleted successfully. \n");
	    return DELETE_SUCCESS;
	    }
	char *end_ptr;
	int item, success;
	success = 0;
	errno = 0;
	
	item = (int)strtol(argv[2], &end_ptr, 10);
	if (ERANGE != errno) {
	    if (end_ptr != argv[2]) {
		if (*end_ptr == '\0') {
		    if (item > 0) {
			success = 1;
			delete_item(item);
			return DELETE_SUCCESS;
			}
		    }
		}
	    }
	if (!success) printf("Invalid argument. Must be a non-negative decimal number greater than zero. \n");
	return DELETE_FAIL;
	}
    return 0;
    }
