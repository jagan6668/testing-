#ifndef HEADER
#define HEADER

#define CLIENT_MSG_KEY 9707
#include<pthread.h>
typedef struct employee {
    char first_name[20];
    char last_name[20];
    char contact[20];
    char project[20];
    char skills[20];
    int emp_id;
    int experience;
    struct employee *next;
} Emp;

extern Emp *ptr;

typedef struct packet {
    Emp data;
    int d;
    char c;
} Packet;

 // extern pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

typedef struct msg {
    long type;
    struct packet pac;
} m;

extern m msg_buffer;
extern m response;
extern int msgid;
extern int msgid2;
extern int fd;

void add_employee(Emp *record);
void search();
void records_with_skills(char *skills);
void records_with_experience(int experience);
void *serve_client(void *p);
void traverse();
//response.pac.c=0;

#endif    
