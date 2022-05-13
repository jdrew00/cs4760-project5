/*
Jacob Drew
4760 Project 5
userProcess.c

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/msg.h>

// structure for message queue
struct mesg_buffer
{
    long mesg_type;
    char mesg_text[500];
} message;

// global data structures
struct resourceDescriptor
{
    int used;
    int available;
    int claim;
    int alloc;
};

int main(int argc, char *argv[])
{

    key_t shmkeyResources;
    shmkeyResources = ftok("oss.c", 'r');
    struct resourceDescriptor *resoruceArray;
    int size_resourceDescriptor = sizeof(struct resourceDescriptor) * 10;
    int shmidresourceDescriptor = shmget(shmkeyResources, size_resourceDescriptor, 0600 | IPC_CREAT);
    resoruceArray = (struct resourceDescriptor *)shmat(shmidresourceDescriptor, 0, 0);

    //modify resoruce array to simulate resoruces being used
    resoruceArray[1].used = resoruceArray[1].used+1;
    resoruceArray[1].available = resoruceArray[1].available-1;
    resoruceArray[1].claim = resoruceArray[1].claim+1;
    resoruceArray[1].alloc = resoruceArray[1].alloc+1;

    char strAvailable[100];
    sprintf(strAvailable, "%d", resoruceArray[1].available);

    char strUsed[100];
    sprintf(strUsed, "%d", resoruceArray[1].used);
    

    int pid = getpid();
    char *mypid = malloc(6); 
    sprintf(mypid, "%d", pid);

    key_t msgkey;
    int msgid;

    // ftok to generate unique key
    msgkey = ftok("oss.c", 'C');

    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(msgkey, 0666 | IPC_CREAT);

    // msgrcv to receive message
    msgrcv(msgid, &message, sizeof(message), 1, 0);

    strcat(message.mesg_text, "Child pid from child process: ");
    strcat(message.mesg_text, mypid);
    strcat(message.mesg_text, ". resouce R1 remaining: ");
    strcat(message.mesg_text, strAvailable);
    strcat(message.mesg_text, ". resouce R1 used: ");
    strcat(message.mesg_text, strUsed);
    strcat(message.mesg_text, "\n");
    // send user process msg from message queue
    msgsnd(msgid, &message, sizeof(message), 0);

    
    return 0;
}