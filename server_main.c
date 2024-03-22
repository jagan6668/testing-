#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <pthread.h>
#include <fcntl.h>
#include "definitions.h"
#include<stdlib.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
 m msg_buffer;
 m response;
Emp *ptr;
int fd;


int msgid;
int msgid2;

int main()
{
msg_buffer.pac.d=0;
    pthread_t tid;
    int key = 9706;
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
  
   msgid2=msgget(CLIENT_MSG_KEY,IPC_CREAT |0666);   // msgqueue from server to clinet .
   if (msgid2==-1)
      perror("error in creating message queeue from server to clinet ");

    fd = open("Employee.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("open");
       
    }
     printf(" msgid - %d , msgid2 - %d  \n :",msgid,msgid2);
    printf("Server started.\n");

    while (1) {
        pthread_create(&tid, NULL, serve_client, NULL);
        pthread_join(tid, NULL);
    }

    return 0;
}

void *serve_client(void *p) {
   

   if (msgrcv(msgid, &msg_buffer, sizeof(struct packet), 0,0) == -1) {
     printf("%d \n ",msg_buffer.type);
printf("Error in MSG Recieve");
    }
puts(msg_buffer.pac.data.first_name); 
printf( "  CHECK msg_buffer.pac.data.frst_name :  %s\n ",msg_buffer.pac.data.first_name);  
  switch (msg_buffer.pac.d) {
        case 1:
	    pthread_mutex_lock(&mutex);
            add_employee(&msg_buffer.pac.data);
	    pthread_mutex_unlock(&mutex);

            break;
        case 21:
        case 22:
	    pthread_mutex_lock(&mutex);
            search();
	    pthread_mutex_unlock(&mutex);
            break;
        case 3:
	    pthread_mutex_lock(&mutex);
            printf("records with skills \n :");
            records_with_skills(msg_buffer.pac.data.skills);
	    pthread_mutex_unlock(&mutex);
            break;
        case 4:
	    pthread_mutex_lock(&mutex);
            records_with_experience(msg_buffer.pac.data.experience);
	    printf("Records With Experience \n ");
	    pthread_mutex_unlock(&mutex);
            break;
        default:
            printf(" Invalid command \n ");
    }
//     pthread_exit(NULL);
}
