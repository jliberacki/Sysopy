#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <mqueue.h>

#define ECHO 1
#define LIST 2
#define FRIENDS 3
#define ADD_FRIENDS 4
#define DEL_FRIENDS 5
#define TO_ALL 6
#define TO_FRIENDS 7
#define TO_ONE 8
#define STOP 9
#define INIT 10

#define MSG_SIZE sizeof(Message)
#define MAX_MESSAGE_LEN 500
#define MAX_MSG_NUM 10
#define SERVER "/server"
char *text;

typedef struct Message {
    long mtype;
    int int_val;
    size_t text_size;
    char text[MAX_MESSAGE_LEN];
} Message;

void error_exit();
void send_data(int id, int type, int int_val, int text_len);
void receive_data(int id, int *type, int *int_val, int *text_len);
