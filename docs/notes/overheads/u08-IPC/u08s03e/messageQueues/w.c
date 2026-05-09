// C Program for Message Queue (Writer Process) 
#include <stdio.h>
#include <stdlib.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

#define L 100

// structure for message queue 
struct mesg_buffer { 
  long mesg_type; 
  char mesg_text[L]; 
} message; 
  
int main() { 
    key_t key; 
    int msgid; 
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    message.mesg_type = 1; 
  
    printf("Read data: ");
    fgets (message.mesg_text, L, stdin);

    // msgsnd to send message 
    msgsnd(msgid, &message, L*sizeof(char), 0); 
  
    // display the message 
    printf("Data send: %s\n", message.mesg_text); 
  
    return 0; 
}
