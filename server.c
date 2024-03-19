#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

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

Emp *ptr;
static int fd;
static int msgid;

typedef struct packet {
    Emp data;
    int d;
    char c;
} Packet;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

typedef struct msg {
    long type;
    struct packet pac;
} m;

m msg_buffer;

void add_employee();
void search();
void records_with_skills();
void records_with_experience();
void *serve_client(void *p);
void traverse();

int main() {
    pthread_t tid;
    int key = 9706;
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    fd = open("Employee.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("open");
       
    }
     printf("%d id  \n :",msgid);
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
 printf("%d\n",msg_buffer.type); 
  
  switch (msg_buffer.pac.d) {
        case 1:
	    pthread_mutex_lock(&mutex);
            add_employee(&msg_buffer.pac.data);
	    pthread_mutex_unlock(&mutex);
            printf("adding employee type %d \n ",msg_buffer.type);
            break;
        case 2:
	    pthread_mutex_lock(&mutex);
            search();
	    printf("Searching \n ");
	    pthread_mutex_unlock(&mutex);
            break;
        case 3:
	    pthread_mutex_lock(&mutex);
            records_with_skills(msg_buffer.pac.data.skills);
	    printf("Records with skills \n ");
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

/*

if ((msgrcv(msgid,&msg_buffer,sizeof(struct packet),1,0)!=-1) && msg_buffer.pac.d==1 && msg_buffer.type==1)
 {
printf("Hello");
   add_employee();
   printf("adding employuee \n ");   
   printf(" \n %d",msg_buffer.type);
 }


if ((msgrcv(msgid,&msg_buffer,sizeof(struct packet),2,0)!=-1) && msg_buffer.pac.d==2 && msg_buffer.type==2)
 {
  search();
  printf("searched \n :");
  printf("%d \n ",msg_buffer.type);
}


if ((msgrcv(msgid,&msg_buffer,sizeof(struct packet),3,0)!=-1) && msg_buffer.pac.d==3 && msg_buffer.type==3)
 {
records_with_skills();
printf("records with skills are recordedd \n :");
printf("%d \n ",msg_buffer.type);
}


if ((msgrcv(msgid,&msg_buffer,sizeof(struct packet),4,0)!=-1) && msg_buffer.pac.d==4 && msg_buffer.type==4)
 {
records_with_experience();
printf("  rerc \n ");
printf("%d \n ",msg_buffer.type);
}
*/
pthread_exit(NULL);

}

void add_employee(Emp* record) {
Emp *node=(Emp *)malloc(sizeof(Emp));
strcpy(node->first_name,record->first_name);
strcpy(node->last_name,record->last_name);
strcpy(node->contact,record->contact);
strcpy(node->project,record->project);
strcpy(node->skills,record->skills);
node->emp_id=record->emp_id;
node->experience=record->experience;
node->next=NULL;
write(fd,node,sizeof(Emp));
printf("inside employee adding \n ");

if (ptr==NULL)
{
ptr=node;
}
else
{
Emp *temp=ptr;
while(temp->next!=NULL){
temp=temp->next;

}
temp->next=node;
}
printf("%p",ptr);

//printf("%s %s %s %s %d %d \n ",ptr->first_name,ptr->last_name,ptr->skills,ptr->project,ptr->experience,ptr->emp_id);
// printf(" temp ->next %p \n ",temp->next);
traverse();
}

void traverse(){
Emp *temp=ptr;
printf(" \n %p",temp);
while (temp !=NULL)
{
printf("%s %s %s %s %d %d \n ",temp->first_name,temp->last_name,temp->skills,temp->project,temp->experience,temp->emp_id);
temp=temp->next;
}
}

void search() {
Emp *temp=ptr;

printf("inside seardch \n :");
while(temp!=NULL)
{
 if (strcmp(temp->first_name,msg_buffer.pac.data.first_name)==0)
 {
 printf("found \n ");
// if (msgsnd(msgid,&msg_buffer,sizeof(struct packet),2)!=0)
  // perror("error in sneding \n ");
 break;
 }
 if(strcmp(temp->last_name,msg_buffer.pac.data.last_name)==0)
 {
 printf("found \n ");
  //if (msgsnd(msgid,&msg_buffer,sizeof(struct packet),2)!=0)
    //perror("error in sending back \n ");
 break;
 }
temp=temp->next;

}

}

void records_with_skills(char* skills) {

printf("inside th records_with_skills \n ");
Emp *temp=ptr;
while(temp!=NULL)
{
if (strcmp(temp->skills,skills)==0)
  printf("%s %s   -  %d ",temp->first_name,temp->last_name,temp->emp_id);
temp=temp->next;
}
}

void records_with_experience(int experience) {
    // Implementation of records_with_experience function
printf("inisde tht records_with_experience \n :");
Emp *temp=ptr;
while(temp!=NULL)
{
   if (temp->experience==experience)
     {
        printf("%s %s   -  %d " ,temp->last_name,temp->first_name,temp->emp_id);
     }
   temp=temp->next;
}

}

