#include "shared.h"

int msqid;
int last_client_id = 0;
int clients[100];
int friends_num = 0;
int friends[100];


void error_exit() {
    perror("Error");
    exit(errno);
}

void handle_init(int key) {
    if((clients[last_client_id] = msgget(key, 0)) == -1) error_exit();
    send_data(clients[last_client_id], INIT, last_client_id, 0);
    last_client_id++;
}

void handle_echo(int client_id, int text_len) {
    time_t now;
    time(&now);
    size_t len = sprintf(text, "%.*s%s", text_len, text, ctime(&now));
    send_data(clients[client_id], ECHO, 0, len);
}

void handle_to_one(int client_id, int text_len) {
    if(clients[client_id] != 0) {
        time_t now;
        time(&now);
        size_t len = sprintf(text, "%.*s%sFrom id: %d\n",text_len, text, ctime(&now), client_id);
        send_data(clients[client_id], ECHO, 0, len);
    }   
}

void handle_list() {
    int i;
    printf("Client list:\n");
    for(i = 0; i < last_client_id; i++) {
        if(clients[i] != 0) {
            printf("id: %d, queue: %d\n", i, clients[i]);
        }
    }
}

void handle_to_all(int client_id, int text_len) {
    time_t now;
    time(&now);
    size_t len = sprintf(text, "%.*s%sFrom id: %d\n",text_len, text, ctime(&now), client_id);
    int i;
    for(i = 0; i < last_client_id; i++) {
        if(clients[i] != 0) {
            send_data(clients[i], ECHO, 0, len);
        }
    }
}

void handle_friends(int text_len) {
    char *tmp = malloc(text_len);
    memcpy(tmp, text, text_len);
    char *str = strtok(tmp, " ");
    friends_num = 0;
    while(str != NULL) {
        int exists = 0;
        int num = atoi(str);
        int i;
        for(i = 0; i < friends_num; i++) {
            if(friends[i] == num) {
                exists = 1;
            }
        }
        if(!exists) {
            friends[friends_num++] = num;
        }
        str = strtok(NULL, " ");
    }
}

void handle_add_friends(int text_len) {
    char *tmp = malloc(text_len);
    memcpy(tmp, text, text_len);
    char *str = strtok(tmp, " ");
    while(str != NULL) {
        int exists = 0;
        int num = atoi(str);
        int i;
        for(i = 0; i < friends_num; i++) {
            if(friends[i] == num) {
                exists = 1;
            }
        }
        if(!exists) {
            friends[friends_num++] = num;
        }
        str = strtok(NULL, " ");
    }
}

void handle_del_friends(int text_len) {
    char *tmp = malloc(text_len);
    memcpy(tmp, text, text_len);
    char *str = strtok(tmp, " ");
    while(str != NULL) {
        int num = atoi(str);
        int i;
        for(i = 0; i < friends_num; i++) {
            if(friends[i] == num) {
                int j;
                for(j = i; j < friends_num - 1; j++) {
                    friends[j] = friends[j+1];
                }
                friends_num--;
            }
        }
        str = strtok(NULL, " ");
    }
}

void handle_to_friends(int client_id, int text_len) {
    time_t now;
    time(&now);
    size_t len = sprintf(text, "%.*s%sFrom id: %d\n",text_len, text, ctime(&now), client_id);
    int i;
    for(i = 0; i < friends_num; i++) {
        if(clients[friends[i]] != 0) {
            send_data(clients[friends[i]], ECHO, 0, len);
        }
    }
}

void handle_stop(int client_id) {
    clients[client_id] = 0; 
    int i;
    int active_clients = 0;
    for(i = 0; i < last_client_id; i++) {
        if(clients[i] != 0) active_clients++;
    }
    if(active_clients == 0) {
        printf("No active clients\n");
        exit(0);
    }
}

void handle_received(int type, int int_val, int text_len) {
    switch(type) {
        case INIT:
            handle_init(int_val);
            break;
        case ECHO:
            handle_echo(int_val, text_len);
            break;
        case TO_ONE:
            handle_to_one(int_val, text_len);
            break;
        case LIST:
            handle_list();
            break;
        case TO_ALL:
            handle_to_all(int_val, text_len);
            break;
        case FRIENDS:
            handle_friends(text_len);
            break;
        case ADD_FRIENDS:
            handle_add_friends(text_len);
            break;
        case DEL_FRIENDS:
            handle_del_friends(text_len);
            break;
        case TO_FRIENDS:
            handle_to_friends(int_val, text_len);
            break;
        case STOP:
            handle_stop(int_val);
            break;
    }
}

void int_handler(int signum) {
    printf("\nInterrupt signal\n");
    exit(0);
}

void on_quit() {
    int i;
    for(i = 0; i < last_client_id; i++) {
        if(clients[i] != 0) {
            send_data(clients[i], STOP, 0, 0);
            receive_data(msqid, NULL, NULL, NULL);
        }
    }
    msgctl(msqid, IPC_RMID, NULL);
}

int main() {
    text = malloc(MAX_MESSAGE_LEN);
    signal(SIGINT, int_handler);
    key_t key = ftok(getenv("HOME"), 0);
    if((msqid = msgget(key, IPC_EXCL | IPC_CREAT | 0666)) == -1) error_exit();
    atexit(on_quit);
    int type, int_val, text_len;
    while(1) {
        receive_data(msqid, &type, &int_val, &text_len);
        handle_received(type, int_val, text_len);
    }
}


