#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void outputToFile(const char *redirectFileName) {
  freopen(redirectFileName, "a", stdout);
}

int main() {
  pid_t child_pid;

  child_pid = fork();

  if (child_pid == -1) {
    printf("ошибка при создании дочернего процесса");
    exit(EXIT_FAILURE);
  } else if (child_pid == 0) {
    printf("Дочерний процесс:\nPID: %d\nPPID: %d\n\n", getpid(), getppid());
    outputToFile("file-1.txt");
    system("ps -l");
  } else {
    sleep(1);
    printf("Родительский процесс:\nPID: %d\nPPID: %d\n\n", getpid(), getppid());
    outputToFile("file-2.txt");
    system("ps -l");
    wait(NULL);
  }
  return EXIT_SUCCESS;
}
