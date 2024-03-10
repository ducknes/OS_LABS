#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MSG_KEY 1234

struct msgbuf {
    long mtype;
    char mtext[2048];
};

int main() {
    int msqid;
    struct msgbuf msg;
    key_t key = MSG_KEY;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msqid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Получено сообщение от клиента: \n %s \n", msg.mtext);

    system(msg.mtext);

    char result[2048];
    FILE *fp;

    fp = popen(msg.mtext, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(result, buffer);
    }
    pclose(fp);

    strcpy(msg.mtext, result);
    msg.mtype = 2;
    
    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}