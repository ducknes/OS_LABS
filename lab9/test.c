#include <stdio.h>
#include <stdlib.h>

int main() {
    char result[256];
    FILE *fp;

    fp = popen("ps -la", "r"); // Replace "ls -l" with your system command
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    while (fgets(result, sizeof(result), fp) != NULL) {
        printf("%s", result);
    }
    pclose(fp);
    return 0;
}