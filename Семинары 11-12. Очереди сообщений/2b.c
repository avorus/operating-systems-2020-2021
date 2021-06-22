// Напишите программы, осуществляющие двустороннюю связь через одну очередь сообщений.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int msqid;
    char pathname[] = "11-2a.c";
    key_t key;
    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        struct {
            int iinfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("[error 11-2b.c] can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("[error 11-2b.c] can't get msqid\n");
        exit(-1);
    }

    for (int i = 1; i <= 5; ++i) {
        maxlen = sizeof(mybuf.info);

        if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0) < 0) {
            printf("[error 11-2a.c] can't receive message from queue\n");
            exit(-1);
        }

        printf("[success 11-2b.c] message type = %ld, iinfo = %i\n", mybuf.mtype, mybuf.info.iinfo);
    }

    for (int i = 1; i <= 5; ++i) {
        mybuf.mtype = 2;
        mybuf.info.iinfo = 321;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("[error 11-2b.c] can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    return 0;
}