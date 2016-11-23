#include<stdio.h>
#include<sys/socket.h> 
 
int main(int argc , char *argv[])
{
    int socket_desc;
    socket_desc = socket(AF_INET , SOCK_STREAM , 0); //connection oriented TCP protocol
     
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    return 0;
}
