#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 

int main() {
  int p1[2]; // C --> S
  int p2[2]; // S --> C

  const char *fileNotFoundErr = "Файл не найден или не может быть прочитан\n";
  const char *filePath = "fil.txt";

  if (pipe(p1) == -1 || pipe(p2) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Server
    close(p1[1]);
    close(p2[0]);

    printf("СЕРВЕР\n"); 

    printf("СЕРВЕР: Ждем получения имени файла\n");
    char filename[256]; 
    if (read(p1[0], filename, sizeof(filename)) == -1) { 
      perror("read");
      exit(EXIT_FAILURE);
    }
    printf("СЕРВЕР: Получено найзвание файла - %s\n", filename);

    printf("СЕРВЕР: Открываем файл - %s\n", filename);
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
      printf("СЕРВЕР: Не смогли открыть файл - %s\n", filename);
      if (write(p2[1], fileNotFoundErr, strlen(fileNotFoundErr)) == -1) { 
        perror("write");
        exit(EXIT_FAILURE);
      }
    } else {
      char buffer[1024];
      size_t bytes_read;
      printf("СЕРВЕР: Отправляем содержимое файла - %s\n", filename);
      while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) { 
        if (write(p2[1], buffer, bytes_read) == -1) { 
          perror("write");
          exit(EXIT_FAILURE);
        }
      }
      fclose(file);
    }

    printf("СЕРВЕР: Закончили отправку\n");
    close(p2[1]);
    close(p1[0]);
  } else {  
    // Client
    printf("КЛИЕНТ\n"); 

    close(p1[0]);
    close(p2[1]);

    printf("КЛИЕНТ: Отправляем имя файла - %s\n", filePath);
    if (write(p1[1], filePath, strlen(filePath)) == -1) { 
      perror("write");
      exit(EXIT_FAILURE);
    }

    wait(NULL);

    char buffer[1024];
    ssize_t bytes_read;
    printf("КЛИЕНТ: Ждем получения содержимого файла - %s\n", filePath);
    while ((bytes_read = read(p2[0], buffer, sizeof(buffer))) > 0) { 
      printf("КЛИЕНТ: Получено содержимое файла - %.*s\n", (int)bytes_read, buffer); 
    }

    printf("КЛИЕНТ: Закончили получение\n");
    close(p1[1]);
    close(p2[0]);
  }

  return EXIT_SUCCESS;
}