#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define CLIENT_MSG_KEY 9707

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
static int msgid2;

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

m response;



//response.type=
void add_employee(Emp* record);
void search();
void records_with_skills(char *skills);
void records_with_experience(int experience);
void *serve_client(void *p);
void traverse();
//response.pac.c=0;
int main() {
    response.pac.d=0;
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
 printf("%d\n",msg_buffer.type); 
 printf(" msg .d %d \n ",msg_buffer.pac.d); 
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
response.type=CLIENT_MSG_KEY;
printf("inside seardch \n :");
while(temp!=NULL)
{
 if (strcmp(temp->first_name,msg_buffer.pac.data.first_name)==0)
 {
     printf("found \n ");
    strcpy(response.pac.data.first_name,temp->first_name);
     strcpy(response.pac.data.last_name,temp->last_name);
     strcpy(response.pac.data.skills,temp->skills);
     strcpy(response.pac.data.project,temp->project);
     strcpy(response.pac.data.experience,temp->experience);
     strcpy(response.pac.data.contact,temp->contact);
     strcpy(response.pac.c,'\0');
    if (msgsnd(msgid2,&response,sizeof(struct packet),0)!=0)
       perror("error in sneding \n ");

 break;
 }
 if(strcmp(temp->last_name,msg_buffer.pac.data.last_name)==0)
 {
 printf("found \n ");
  strcpy(response.pac.data.first_name,temp->first_name);
  strcpy(response.pac.data.last_name,temp->last_name);
  strcpy(response.pac.data.skills,temp->skills);
  strcpy(response.pac.data.project,temp->project);
  strcpy(response.pac.data.contact,temp->contact);
  response.pac.data.experience=temp->experience;
  response.pac.data.emp_id=temp->emp_id;
  
  printf("TRAIL %s \n " ,response.pac.data.first_name);
  printf("TRAIL %s \n ",response.pac.data.last_name);
  if (msgsnd(msgid2,&response ,sizeof(struct packet),0)!=0)
    perror("error in sending back \n ");
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

