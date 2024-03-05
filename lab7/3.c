#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void flagHandler(int signum) {
  if (signum == SIGUSR1) {
    flag = 1;
  }
}

int main() {
  signal(SIGUSR1, flagHandler);

  pid_t child_pid;
  child_pid = fork();

  if (child_pid == -1) {
    perror("ошибка при создании дочернего процесса\n");
    exit(EXIT_FAILURE);
  } else if (child_pid == 0) {
    printf("Дочерний процесс ожидает начала работы\n");
    while (!flag) {
    }
    printf("Дочерний процесс синхронизирован\n");
  } else {
    printf("Родительский процесс начинает работу\n");
    sleep(5);
    kill(child_pid, SIGUSR1);
    printf("Родительский процесс завершил работу\n");
    wait(NULL);
  }

  return EXIT_SUCCESS;
}
