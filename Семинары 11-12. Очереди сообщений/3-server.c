// Напишите 2 программы для предложенной схемы мультиплексирования сообщений: клиент и сервер.
// Клиент вводит с клавиатуры число с плавающей точкой и отправляет серверу. Дожидается от сервера
// ответа на запрос – это будет число с плавающей точкой – и печатает его на экране. 
// Сервер принимает от клиентов запросы. Полученное от клиента число возводится в квадрат, и результат
// отправляется клиенту.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main() {
    int msqid;
    char pathname[] = "12-server.c";
    key_t key;
    int len, maxlen;

    // структура запроса клиента 
    struct mymsgbuf1 {
        long mtype; // тип сообщения (1)
        struct {
            pid_t client_pid; // идентификатор клиента
	    double client_request; // число, которое нужно возвести в квадрат
        } request;
    } request_buf;

    // структура ответа сервера
    struct mymsgbuf2 {
        long mtype; // тип сообщения (pid)
        struct {
	    double server_response; // число, возведенное в квадрат
        } response;
    } response_buf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("[error 12-server.c] can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("[error 12-server.c] can't get msqid\n");
        exit(-1);
    }
 
    while (1) {
        maxlen = sizeof(request_buf.request);
        if (len = msgrcv(msqid, (struct msgbuf *) &request_buf, maxlen, 1, 0) < 0) {
            printf("[error 12-server.c] can't receive message from queue\n");
            exit(-1);
        }

        response_buf.mtype = request_buf.request.client_pid;
        response_buf.response.server_response = request_buf.request.client_request * request_buf.request.client_request;
        len = sizeof(response_buf.response);
 
        if (msgsnd(msqid, (struct msgbuf *) &response_buf, len, 0) < 0) {
            printf("[error 12-server.c] can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    // msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

    return 0;
}
