
#include "definitions.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/msg.h>
static Emp *ptr=NULL;
static Emp emp_array[50];


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
while (temp !=NULL)
{
strcpy(response.pac.data.first_name,temp->first_name);

strcpy(response.pac.data.first_name,temp->first_name);
strcpy(response.pac.data.contact,temp->contact);
strcpy(response.pac.data.skills,temp->skills);
strcpy(response.pac.data.project,temp->project);
response.pac.data.emp_id=temp->emp_id;
response.pac.data.experience=temp->experience;
strcpy(response.pac.data.last_name,temp->last_name);

printf("%s %s %s %s %d %d \n ",temp->first_name,temp->last_name,temp->skills,temp->project,temp->experience,temp->emp_id);
temp=temp->next;
}

}



void search() {
Emp *temp=ptr;

if (msg_buffer.pac.d==21)
{
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
     response.pac.c='\0';
     if (msgsnd(msgid2,&response,sizeof(Packet),0)!=0)
       perror("error in sneding \n ");
     
   }
temp=temp->next;
}
strcpy(response.pac.data.contact,"exit");
 if (msgsnd(msgid2,&response,sizeof(Packet),0)!=0)
    perror("error in receieving \n ");
 else
   printf("-> Succesfully sent aackonowleedged as end \n ");
  
}
else if(msg_buffer.pac.d==22)
{
Emp *ptr=temp;
while (temp!=NULL)
{
 if(strcmp(temp->last_name,msg_buffer.pac.data.last_name)==0)
 {
  printf("Clien :  %d \n ",msg_buffer.type );
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
  if (msgsnd(msgid2,&response ,sizeof(Packet),0)!=0)
    perror("error in sending back \n ");
  else
   printf("sent back to client succesfully \n :");
  break;
 }
temp=temp->next;

}
}
}

void records_with_skills(char* skills)
{
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
 
 if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
  {

	printf("-> send sucessfully to client \n :");
  }
   else
       perror("error in sending \n ");
   
}
 temp=temp->next;
}
 strcpy(response.pac.data.last_name,"exit");
 if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
  {
    printf("-> succesfully acknowledged to client as end of data \n ");
  }
else
  {
  perror("error in sending acknowledge \n ");
  }

}

void records_with_experience(int experience) 
{
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
   response.type=msg_buffer.type;
  // printf(" CHECH ***  %s %s \n ",response.pac.data.first_name,response.pac.data.last_name);
   
   if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
   printf(" -> succesfully sent to client \n ");
  else
   perror("error in sending \n ");      
  }

 temp=temp->next;
}

strcpy(response.pac.data.last_name,"exit");
if (msgsnd(msgid2,&response,sizeof(Packet),0)==0)
 printf("-> succesfully Acknowledged as end of data \n ");
else
 perror("error in sending acknowledgement \n ");

}

int compare_emp_id(const void  *a,const  void *b)
{
const Emp *s1=(const Emp *)a;
const Emp *s2=(const Emp *)b;
return s1->emp_id - s2->emp_id;
}

void sorting_with_emp_id()
{
Emp *temp=ptr;
int i=0;
int j=0;
while(temp!=NULL)
{
 strcpy(emp_array[i].first_name,temp->first_name);
 strcpy(emp_array[i].last_name,temp->last_name);
 strcpy(emp_array[i].contact,temp->contact);
 strcpy(emp_array[i].skills,temp->skills);
 strcpy(emp_array[i].project,temp->project);
 emp_array[i].emp_id=temp->emp_id;
 emp_array[i].experience=temp->experience;
 temp=temp->next;
 i++;
}
qsort(emp_array,i,sizeof(Emp),compare_emp_id);

while (j<i)
{
 strcpy(response.pac.data.first_name,emp_array[j].first_name);
 strcpy(response.pac.data.last_name,emp_array[j].last_name);
 strcpy(response.pac.data.skills,emp_array[j].skills);
 strcpy(response.pac.data.project,emp_array[j].project);
 strcpy(response.pac.data.contact,emp_array[j].contact);
 response.pac.data.experience=emp_array[j].experience;
 response.pac.data.emp_id=emp_array[j].emp_id;
 response.pac.c='\0';
 response.pac.d=0;
 response.type=msg_buffer.type;
 if (msgsnd(msgid2,&response,sizeof(Packet),0)!=-1)
  printf("sent correctly \n :");
 else
  perror("eror in sendung \n ");

j++;
}
strcpy(response.pac.data.last_name,"exit");

if (msgsnd(msgid2,&response,sizeof(Packet),0)!=-1)
  printf("sent correxrly as end \n ");
else
perror("error in sending ass end of data \n ");

}

int compare_first_name(const void *a,const void *b)
{
const Emp  *s1=(const Emp *)a;
const Emp  *s2=(const Emp *)b;
return (strcmp(s1->first_name,s2->first_name));
}

void sorting_with_first_name()
{
Emp *temp=ptr;
int i=0;
int j=0;
while(temp!=NULL)
{
 strcpy(emp_array[i].first_name,temp->first_name);
 strcpy(emp_array[i].last_name,temp->last_name);
 strcpy(emp_array[i].contact,temp->contact);
 strcpy(emp_array[i].skills,temp->skills);
 strcpy(emp_array[i].project,temp->project);
 emp_array[i].emp_id=temp->emp_id;
 emp_array[i].experience=temp->experience;
 temp=temp->next;
 i++;
}
qsort(emp_array,i,sizeof(Emp),compare_first_name);

while (j<i)
{
 strcpy(response.pac.data.first_name,emp_array[j].first_name);
 strcpy(response.pac.data.last_name,emp_array[j].last_name);
 strcpy(response.pac.data.skills,emp_array[j].skills);
 strcpy(response.pac.data.project,emp_array[j].project);
 strcpy(response.pac.data.contact,emp_array[j].contact);
 response.pac.data.experience=emp_array[j].experience;
 response.pac.data.emp_id=emp_array[j].emp_id;
 response.pac.c='\0';
 response.pac.d=0;
 response.type=msg_buffer.type;
 if (msgsnd(msgid2,&response,sizeof(Packet),0)!=-1)
  printf("sent correctly \n :");
 else
  perror("eror in sendung \n ");

j++;
}
strcpy(response.pac.data.last_name,"exit");

if (msgsnd(msgid2,&response,sizeof(Packet),0)!=-1)
  printf("sent correxrly as end \n ");
else
perror("error in sending as end of data \n ");

}

