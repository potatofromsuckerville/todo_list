#include <stdio.h>
#include <time.h>

int main() {
    time_t t = time(NULL);
    
    struct tm *currenttime = localtime(&t);
    printf("%s \n", asctime(currenttime));
               
    return 0;
    }
