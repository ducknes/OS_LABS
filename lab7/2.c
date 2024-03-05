#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signalEndHandler(int signum) {
  printf("\nПрограмма завершена.");
  exit(EXIT_SUCCESS);
}

int sec = 0;

void textHandler() { printf("Прошло %d секунд\n", sec); }

int main() {
  signal(SIGINT, signalEndHandler);
  signal(SIGALRM, textHandler);

  while (1) {
    sec += 2;
    alarm(2);
    pause();
  }

  return EXIT_SUCCESS;
}
