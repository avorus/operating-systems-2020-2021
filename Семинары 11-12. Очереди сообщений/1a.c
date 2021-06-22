// Для полной передачи информативной части сообщения в качестве длины нужно указывать не сумму длин полей,
// а полную длину структуры, описывающей информативную часть сообщения. 
// Модифицируйте предыдущие программы ex1a.c и ex1b.c для передачи смешанных по типу сообщений.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main() {
    int msqid;
    char pathname[] = "11-1a.c";
    key_t key;
    int len;

    struct mymsgbuf {
        long mtype;
        struct {
            int iinfo;
            float finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("[error 11-1a.c] can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("[error 11-1a.c] can't get msqid\n");
        exit(-1);
    }

    for (int i = 1; i <= 5; ++i) {
        mybuf.mtype = 1;
        mybuf.info.iinfo = 123;
        mybuf.info.finfo = 1.23;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, &mybuf, len, 0) < 0) {
            printf("[error 11-1a.c] can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    mybuf.mtype = LAST_MESSAGE;
    len = 0;

    if (msgsnd(msqid, &mybuf, len, 0) < 0) {
        printf("[error 11-1a.c] can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    return 0;
}