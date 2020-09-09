#include "shared.h"

int client_msqid;
int msqid;
int client_id;
pid_t child_pid;

void on_quit() {
    if(child_pid != 0) {
        msgctl(client_msqid, IPC_RMID, NULL);
        send_data(msqid, STOP, client_id, 0);
        kill(child_pid, SIGKILL);   
    }
}

void int_handler(int signum) {
    printf("\nInterrupt signal\n");
    exit(0);
}

void sigusr_handler(int signum) {
    printf("Server stopped\n");
    exit(0);
}

void error_exit() {
    perror("Error");
    exit(errno);
}

void send_echo(char *string, size_t size) {
    memcpy(text, string + 5, size);
    send_data(msqid, ECHO, client_id, size);
}

void send_list() {
    send_data(msqid, LIST, 0, 0);
}

void send_to_all(char *string, size_t size) {
    memcpy(text, string + 5, size);
    send_data(msqid, TO_ALL, client_id, size);
}

void send_to_one(char *string, int id, size_t size) {
    memcpy(text, string, size);
    send_data(msqid, TO_ONE, id, size);
}

void send_friends(char *string, size_t size) {
    memcpy(text, string + 8, size);
    send_data(msqid, FRIENDS, client_id, size);
}

void send_add_friends(char *string, size_t size) {
    memcpy(text, string + 4, size);
    send_data(msqid, ADD_FRIENDS, client_id, size);
}

void send_del_friends(char *string, size_t size) {
    memcpy(text, string + 4, size);
    send_data(msqid, DEL_FRIENDS, client_id, size);
}

void send_to_friends(char *string, size_t size) {
    memcpy(text, string + 9, size);
    send_data(msqid, TO_FRIENDS, client_id, size);
}

void handle_input(char *comm, size_t size) {
    if(strncmp(comm, "echo", 4) == 0) {
        send_echo(comm, size - 5);
    }
    else if(strcmp(comm, "list\n") == 0) {
        send_list();
    }
    else if(strncmp(comm, "2all", 4) == 0) {
        send_to_all(comm, size - 5);
    }
    else if(strncmp(comm, "2one", 4) == 0) {
        strtok(comm, " ");
        char *saveptr;
        char *str = strtok_r(comm + 5, " ", &saveptr);
        if(str == NULL) {
            fprintf(stderr, "Wrong number of arguments\n"); 
            return;
        }
        int id = atoi(str);
        str = strtok_r(NULL, "", &saveptr);
        size_t size = 0;
        if(str != NULL) size = strlen(str);
        send_to_one(str, id, size); 
    }
    else if(strncmp(comm, "friends", 7) == 0) {
        send_friends(comm, size - 8);
    }
    else if(strncmp(comm, "add", 3) == 0) {
        send_add_friends(comm, size - 4);
    }
    else if(strncmp(comm, "del", 3) == 0) {
        send_del_friends(comm, size - 4);
    }
    else if(strncmp(comm, "2friends", 8) == 0) {
        send_to_friends(comm, size - 9);
    }
    else if(strcmp(comm, "stop\n") == 0) {
        exit(0);
    }
    else{
        printf("Wrong command\n");
    }
}

int main(int argc, char **argv) {
    text = malloc(MAX_MESSAGE_LEN);
    key_t key = ftok(getenv("HOME"), 0);
    if((msqid = msgget(key, 0)) == -1) error_exit();
    key_t client_key = ftok(getenv("HOME"), getpid());
    if((client_msqid = msgget(client_key, IPC_EXCL | IPC_CREAT | 0666)) == -1) error_exit();
    atexit(on_quit);
    send_data(msqid, INIT, client_key, 0);
    receive_data(client_msqid, NULL, &client_id, NULL);
    printf("My id: %d\n", client_id);
    size_t max_comm_len = MAX_MESSAGE_LEN + 6;
    char *comm = malloc(MAX_MESSAGE_LEN + 6);
    if((child_pid = fork()) == -1) error_exit();
    if(child_pid == 0) {
        int type, int_val, text_len;
        while(1) {
            receive_data(client_msqid, &type, &int_val, &text_len);
            if(type == ECHO) {
                printf("%.*s\n", text_len, text);
            }
            if(type == STOP) {
                kill(getppid(), SIGUSR1);
                exit(0);
            }
        }
    }
    else {
        signal(SIGINT, int_handler);
        signal(SIGUSR1, sigusr_handler);
        if(argc > 1) {
            FILE *fd;
            if((fd = fopen(argv[1], "r")) != NULL) {
                char input[10000]; 
                fread(input, sizeof(char), 10000, fd);
                char *saveptr;
                char *comm = strtok_r(input, "\n", &saveptr);
                while(comm != NULL) {
                    char tmp[strlen(comm) + 1];
                    int len = sprintf(tmp, "%s\n", comm);
                    handle_input(tmp, len);
                    comm = strtok_r(NULL, "\n", &saveptr);
                }
            }
        }
        while(1) {
            size_t size = getline(&comm, &max_comm_len, stdin);
            handle_input(comm, size);
         }
    }
}
