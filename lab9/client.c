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

    printf("Введите команду для исполнения: \n");
    char command[2048];
    fgets(command, sizeof(command), stdin);

    strcpy(msg.mtext, command);
    msg.mtype = 1;

    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1 ) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msqid, &msg, sizeof(msg.mtext), 2, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Ответ от сервера: \n %s \n", msg.mtext);

    return EXIT_SUCCESS;
}
