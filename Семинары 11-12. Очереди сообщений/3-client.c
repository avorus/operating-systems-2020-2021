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

    double input;
    scanf("%lf", &input);

    if ((key = ftok(pathname, 0)) < 0) {
        printf("[error 12-client.c] can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("[error 12-client.c] can't get msqid\n");
        exit(-1);
    }

    request_buf.mtype = 1;
    request_buf.request.client_pid = getpid();
    request_buf.request.client_request = input;
    len = sizeof(request_buf.request);

    if (msgsnd(msqid, (struct msgbuf *) &request_buf, len, 0) < 0) {
        printf("[error 12-client.c] can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    maxlen = sizeof(response_buf.response);
    if (len = msgrcv(msqid, (struct msgbuf *) &response_buf, maxlen, getpid(), 0) < 0) {
        printf("[error 12-client.c] can't receive message from queue\n");
        exit(-1);
    }

    printf("[success 12-client.c] message type = %ld, request = %lf, response = %lf\n", request_buf.request.client_pid, request_buf.request.client_request, response_buf.response.server_response);

    return 0;
}
