#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
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

typedef struct packet {
    Emp data;
    int d;
    char c;
} Packet;

typedef struct msg {
    long type;
    struct packet pac;
} m;
m msg_buffer;
int msgid;
int msgid2;
m response;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *client_thread(void *arg);
void employee_thread();
void search_thread();
void records_with_skills();
void records_with_experience();
void sorting_with_emp_id();

int main() {
   
    int key = 9706;
    msgid = msgget(key, 0666|IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
msg_buffer.type = getpid();
printf("Client Id - %d\n",getpid());    
pthread_t tid; // Thread ID for each client

    int client_id =1;
    msgid2=msgget(CLIENT_MSG_KEY,IPC_CREAT|0666);     
    while (1) {
       
        if (pthread_create(&tid, NULL, client_thread,(void *)client_id) != 0) {
            perror("error in pthread_create");
            
        }
        printf("Client %d started.\n", client_id);
        pthread_join(tid,NULL);
       
        client_id++;
        sleep(2);
    }

    return 0;
}

void *client_thread(void *arg) {
    int client_id =(int)arg;
    printf("Client %d started.\n", client_id);
    int choice;
    printf("Client %d: Enter your choice:\n",client_id);
    printf("1. Add Employee\n");
    printf("2. Search\n");
    printf("3. Records with Skills\n");
    printf("4. Records with Experience\n");
    printf("5. Sort with emp_id \n ");
    printf("6. Sort with first_name \n ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
	   
            msg_buffer.pac.d = 1; // Add Employee
            employee_thread();
 	   
            break;
        case 2:
	   
            //msg_buffer.pac.d = 2; // Search
            search_thread();
                 
            break;
       case 3:
           
            msg_buffer.pac.d = 3; // Records with Skills
            records_with_skills();
	   
            break;
            
        case 4:
	   
            msg_buffer.pac.d = 4; // Records with Experience
            records_with_experience();
            break;
       case 5:

           msg_buffer.pac.d=5;
           sorting_with_emp_id();
           break;
        
       case 6:
          
            msg_buffer.pac.d=6;
            sorting_with_first_name();
            break;
        default:
            printf("Client %d: Invalid choice\n", client_id);
            pthread_exit(NULL);
    }

    printf("Client %d: Request sent.\n", client_id);
    pthread_exit(NULL);
}


void employee_thread()
{

printf("enter first_name : \n");
getchar();
scanf("%s",msg_buffer.pac.data.first_name);
printf("enter last name : \n");
scanf("%s",msg_buffer.pac.data.last_name);
getchar();
printf("enter contact : \n ");
scanf("%s",msg_buffer.pac.data.contact);
getchar();
printf("enter skills : \n ");
scanf("%s",msg_buffer.pac.data.skills);
getchar();
printf("enter the employeee id : \n ");
scanf("%d",&(msg_buffer.pac.data.emp_id));
getchar();
printf("enter the experience : \n ");
scanf("%d",&msg_buffer.pac.data.experience);
getchar();
printf("enter the project \n ");
scanf("%s",&msg_buffer.pac.data.project);
getchar();

msg_buffer.pac.c='\0';
if (msgsnd(msgid,&msg_buffer,sizeof(struct packet),0)==-1)
{
  perror("error in sending \n ");
 }
else{
printf("-> sent correctly");
}
}


void search_thread()
{
   int c;
printf("%d",msgid);
printf("enter the choiceof searching 1 :first_name  2 :last_name 3: empid \n ");
scanf("%d",&c);
getchar();

switch(c)
{
case 1 :
printf("enter first_name \n ");
//getchar();
scanf("%[^\n]s",msg_buffer.pac.data.first_name);
//getchar();
msg_buffer.pac.d=21;
msg_buffer.pac.c='\0';
if (msgsnd(msgid,&msg_buffer,sizeof(struct packet),0)==-1)
    perror("eror in sending info \n ");
else
 printf("-> sent correctly \n ");

break;

case 2:
printf("enter last name \n ");
scanf("%[^\n]s",msg_buffer.pac.data.last_name);
getchar();
msg_buffer.pac.d=22;
msg_buffer.pac.c='\0';
if (msgsnd(msgid,&msg_buffer,sizeof(Packet),0)==-1)
   perror("error in sending \n ");
else 
printf("-> sent corectly \n : ");
break;

}
while(1)
{
if (msgrcv(msgid2,&response,sizeof(struct packet),getpid(),0)!=-1)
{
  if (strcmp(response.pac.data.contact,"exit")==0)
  break;
   printf(" These are the details \n : ");
   printf(" ***********\n %s %s %s %s %d %d \n*********** \n  ",response.pac.data.first_name,response.pac.data.last_name,response.pac.data.skills,response.pac.data.contact,response.pac.data.emp_id,response.pac.data.experience);
}
else
{
perror(" failed in revceiving from server to client \n ");
}
}
}


void records_with_skills()
{
msg_buffer.pac.c='\0';
printf("enter the skills \n ");
scanf("%s",msg_buffer.pac.data.skills);
if (msgsnd(msgid,&msg_buffer,sizeof(struct packet),0)==-1)
   perror("error in sending \n ");
else 
    printf("-> sent correctly \n :");

while(1)
{
if (msgrcv(msgid2,&response,sizeof(struct packet),getpid(),0)!=-1)
{
  printf(" TYPE %d \n  " ,response.type);
   if (strcmp(response.pac.data.last_name,"exit")==0)
    break;

 printf("*************** \n First name : %s \n Last name : %s \n Skills : %s \n Project : %s \n Emp_id : %d \n ****************** \n ",response.pac.data.first_name,response.pac.data.last_name,response.pac.data.skills,response.pac.data.project,response.pac.data.emp_id);
}
else
perror("error in receiving \n ");

}
}



void records_with_experience()
{

msg_buffer.pac.c='\0';
printf("enter the experience \n");
scanf("%d",&msg_buffer.pac.data.experience);
if (msgsnd(msgid,&msg_buffer,sizeof(Packet),0)==-1)
 perror("error in sending \n ");
else 
printf("sent correctly \n :");


while(1)
{
if (msgrcv(msgid2,&response,sizeof(Packet),getpid(),0)!=-1)
{
 if(strcmp(response.pac.data.last_name,"exit")==0)
  break  ;
 else 
  printf(" *************** \n First name : %s \n Last name : %s \n Skills : %s \n Project : %s \n Emp_id : %d \n ************* \n  ",response.pac.data.first_name,response.pac.data.last_name,response.pac.data.skills,response.pac.data.project,response.pac.data.emp_id);
}

else 
 perror("error in receiving \n : ");

}
}

void sorting_with_emp_id()
{
 msg_buffer.pac.c='\0';
if (msgsnd(msgid,&msg_buffer,sizeof(Packet),0)==-1)
   perror("error in receiving \n ");
else
   printf("sent correctly \n ");
while(1)
{
if (msgrcv(msgid2,&response,sizeof(Packet),getpid(),0)!=-1)
{
 if(strcmp(response.pac.data.last_name,"exit")==0)
    break;
 else
 { 
  printf(" *************** \n First name : %s \n Last name : %s \n Skills : %s \n Project : %s \n Emp_id : %d \n ************* \n  ",response.pac.data.first_name,response.pac.data.last_name,response.pac.data.skills,response.pac.data.project,response.pac.data.emp_id);
 }
}
}
}
void sorting_with_first_name()
{
 msg_buffer.pac.c='\0';
if (msgsnd(msgid,&msg_buffer,sizeof(Packet),0)==-1)
   perror("error in receiving \n ");
else
   printf("sent correctly \n ");
while(1)
{
if (msgrcv(msgid2,&response,sizeof(Packet),getpid(),0)!=-1)
{
 if(strcmp(response.pac.data.last_name,"exit")==0)
    break;
 else
 { 
  printf(" *************** \n First name : %s \n Last name : %s \n Skills : %s \n Project : %s \n Emp_id : %d \n ************* \n  ",response.pac.data.first_name,response.pac.data.last_name,response.pac.data.skills,response.pac.data.project,response.pac.data.emp_id);
 }
}
}
}
