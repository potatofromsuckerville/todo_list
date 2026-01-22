#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef struct {
    char task_name[64];    
    }Tasks;

Tasks *task = NULL;
const char *file_name = "data.bin";

void update_fn(uint task_count, const char *mode);
void read_fn(void);
void delete_fn(void);

int main(int argc, char **argv) {
    if (argc < 2) {
	printf("Program usage: %s <command> \"task1\" \"task2\" ... \n", argv[0]);
	return 1;
	}

    uint task_count = (uint)argc - 2;
    int new, read, add, delete;
    task = malloc((task_count) * sizeof(Tasks));    
    for(int i = 0; i < task_count; i++) {
	strcpy(task[i].task_name, argv[i + 2]); 
	}
    
    new = strcmp(argv[1], "new");
    read = strcmp(argv[1], "read");
    add = strcmp(argv[1], "add");
    delete = strcmp(argv[1], "delete");
    
    if (new == 0) {
	update_fn(task_count, "wb");
	}    
    else if (read == 0) {
	read_fn();
	}    
    else if (add == 0) {
	update_fn(task_count, "rb+");
	}    
    else if (delete == 0){
	delete_fn();
	}    
    else {
	printf("No commands detected. Use new/add/read/delete to perform operations. \n");
	}
    free(task);
    return 0;
    }

void update_fn(uint task_count, const char *mode) {
    uint32_t len, date_len;
    FILE *f = fopen(file_name, mode);
    if (!f) {
	perror("Error opening file");
	return;
	}
    time_t date_created = time(NULL);
   
    const char *date = ctime(&date_created);
    date_len = strlen(date);
    printf("%d \n", date_len);
    fwrite(&date_len, sizeof(uint32_t), 1, f);
    fwrite(date, sizeof(char), date_len, f);
    fwrite(&task_count, sizeof(int), 1, f);
    for (int i = 0; i < task_count; i++) {
	len = strlen(task[i].task_name);
	fwrite(&len, sizeof(uint32_t), 1, f);
	fwrite(task[i].task_name, sizeof(char), len, f);
	}
    
    if (strcmp(mode, "wb") == 0) {
	fwrite(&task_count, sizeof(uint), 1, f);
	printf("List successfully created. \n");    
	}
    else if (strcmp(mode, "rb+") == 0) {
	uint curr_count;	
	fseek(f, -sizeof(uint), SEEK_END);
	fread(&curr_count, sizeof(uint), 1, f);
	curr_count += task_count;
	fseek(f, 0, SEEK_SET);
	fwrite(&task_count, sizeof(uint), 1, f);
	printf("Items successfully added. \n");
	}
    else printf("Unknown mode entry. \n");
    fclose(f);
    }

void read_fn(void) {
    FILE *f = fopen(file_name, "r");
    if (!f) {
	perror("Error opening file.");
	return;
	}

    uint32_t len, date_len;    
    fread(&date_len, sizeof(uint32_t), 1, f);
    char *date = malloc((date_len + 1) * sizeof(char));
    fread(date, sizeof(char), date_len, f);
    date[date_len] = '\0';
    for (int i = 0; i < len; i++) {
	fread(&len, sizeof(uint32_t), 1, f);
	fread(task[i].task_name, sizeof(char), len, f);
	}

    free(date);
    fclose(f);
    }

void delete_fn(void) {
    printf("Items successfully deleted. \n");
    }
