
#include "definitions.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/msg.h>

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
     response.pac.data.experience=temp->experience;
     strcpy(response.pac.data.contact,temp->contact);
     response.type = msg_buffer.type;
     response.pac.d=0;
     printf("%d\n",response.type);
     response.pac.c='\0';
     if (msgsnd(msgid2,&response,sizeof(Packet),0)!=0)
       perror("error in sneding \n ");

      break;
 }
 if(strcmp(temp->last_name,msg_buffer.pac.data.last_name)==0)
 {
  printf("Clien :  %d found \n ",msg_buffer.type );
  strcpy(response.pac.data.first_name,temp->first_name);
  strcpy(response.pac.data.last_name,temp->last_name);
  strcpy(response.pac.data.skills,temp->skills);
  strcpy(response.pac.data.project,temp->project);
  strcpy(response.pac.data.contact,temp->contact);
  response.pac.data.experience=temp->experience;
  response.pac.data.emp_id=temp->emp_id;
  response.pac.c='\0';
  response.pac.d=0;
  response.type=msg_buffer.type;
  printf("TRAIL %s \n " ,response.pac.data.first_name);
  printf("TRAIL %s \n ",response.pac.data.last_name);
  if (msgsnd(msgid2,&response ,sizeof(Packet),0)!=0)
    perror("error in sending back \n ");
  else
{
printf("sent back to client succesfully \n :");
}

 break;
 }
temp=temp->next;

}

}

void records_with_skills(char* skills) {
response.type=CLIENT_MSG_KEY;

printf("inside th records_with_skills \n ");
Emp *temp=ptr;
while(temp!=NULL)
{
if (strcmp(temp->skills,skills)==0)
{

strcpy(response.pac.data.first_name,temp->first_name);
strcpy(response.pac.data.last_name,temp->last_name);
strcpy(response.pac.data.contact,temp->contact);
strcpy(response.pac.data.skills,temp->skills);
strcpy(response.pac.data.project,temp->project);
response.pac.data.experience=temp->experience;
response.pac.data.emp_id=temp->emp_id;
response.pac.c='\0';
response.type=msg_buffer.type;
response.pac.d='\0';

printf(" CHECK ***  %s %s \n ",response.pac.data.first_name,response.pac.data.last_name);
  if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
  {
   printf("send sucessfully to client \n :");
  }
   else
   {
    perror("error in sending \n ");
    }
}
temp=temp->next;
}
 strcpy(response.pac.data.last_name,"exit");
 if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
  {
    printf(" succesfully acknowledged to client as end of data \n ");
  }
else
  {
  perror("error in sending acknowledge \n ");
  }

}

void records_with_experience(int experience) {
    // Implementation of records_with_experience function
response.type=CLIENT_MSG_KEY;

printf("inisde tht records_with_experience \n :");
Emp *temp=ptr;
while(temp!=NULL)
{
   if (temp->experience==experience)
     {
    strcpy(response.pac.data.first_name,temp->first_name);
    strcpy(response.pac.data.last_name,temp->last_name);
    strcpy(response.pac.data.skills,temp->skills);
   strcpy(response.pac.data.project,temp->project);
   response.pac.data.experience=temp->experience;
   response.pac.data.emp_id=temp->emp_id;
   strcpy(response.pac.data.contact,temp->contact);
   response.pac.c=0;
   response.type=getpid();
   printf(" CHECH ***  %s %s \n ",response.pac.data.first_name,response.pac.data.last_name);
   
   if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
   printf(" succesfully sent to client \n ");
  else
   perror("error in sending \n ");      
  }

 temp=temp->next;
}

strcpy(response.pac.data.last_name,"exit");
if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
 printf("succesfully Acknowledged as end of data \n ");
else
 perror("error in sending acknowledgement \n ");

}

