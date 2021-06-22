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
    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        struct {
            int iinfo;
            float finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("[error 11-1b.c] can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("[error 11-1b.c] can't get msqid\n");
        exit(-1);
    }

    while (1) {
        maxlen = sizeof(mybuf.info);

        if (len = msgrcv(msqid, &mybuf, maxlen, 0, 0) < 0) {
            printf("[error 11-1b.c] can't receive message from queue\n");
            exit(-1);
        }

        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, NULL);
            exit(0);
        }

        printf("[success 11-1b.c] message type = %ld, iinfo = %i, finfo = %f\n", mybuf.mtype, mybuf.info.iinfo, mybuf.info.finfo);
    }

    return 0;
}