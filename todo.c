#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *f;
    int new, add, read;
    
    if (argc < 2) {
	printf("Program usage: %s \"task1\" \"task2\" ...\n", argv[0]);
	printf("Note: seperate tasks with spaces \n");
	return 1;
	}	   
    new = strcmp(argv[1], "new");
    if (new == 0) {
	f = fopen("data.bin", "wb");
	if (!f) {
	    fprintf(stderr, "Could not open file. \n");
	    goto clean_up;
	    }
	for (int i = 2; i < argc; i++) {
	    size_t len = strlen(argv[i]);
	    fwrite(&len, sizeof(size_t), 1, f);
	    fwrite(argv[i], sizeof(char), len, f);
	    }
	goto clean_up;
	}
    add = strcmp(argv[1], "add");
    if (add == 0) {
	f = fopen("data.bin", "ab");
	if (!f) {
	    fprintf(stderr, "Could not open file. \n");
	    goto clean_up;
	    }
	for (int i = 2; i < argc; i++) {
	    size_t len = strlen(argv[i]);
	    fwrite(&len, sizeof(size_t), 1, f);
	    fwrite(argv[i], sizeof(char), len, f);
	    }
	goto clean_up;
	}
    
    char *buff;
    size_t len;
    read = strcmp(argv[1], "read");
    if (read == 0) {
	f = fopen("data.bin", "rb");
	if (!f) {
	    fprintf(stderr, "Could not open file. \n");
	    goto clean_up;
	    }
	rewind(f);
	while (fread(&len, sizeof(size_t), 1, f)) {
	    buff = malloc(len + 1);
	    fread(buff, sizeof(char), len, f);
	    buff[len] = '\0';
	    printf("%s \n", buff);
	    }
	goto clean_up;
	}    
    printf("Operation not specified. \n");
    printf("Use arguments new/add/read before tasks. \n");
    clean_up:
	if (buff) free(buff);
	if (f) fclose(f);
    return 0;
    }
