#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define typename(x) _Generic((x), \
    unsigned long: "unsigned long", \
    int: "int", \
    default: "other")

void signal_handler(int sig, siginfo_t *siginfo, void *context){
	printf("the number recieved is:%lu\n", siginfo->si_value.sival_int);
}

int main(int argc, char *argv[]){
    int pid_s1 = fork();
    if(pid_s1<0){
        printf("failed to create the child process\n");
        return -1;
    }else if(pid_s1==0){
        // s1 executes here
        struct sigaction *action;
        action = malloc(sizeof(struct sigaction));
        action->sa_sigaction = &signal_handler;
        action->sa_flags = SA_SIGINFO;
        int result = sigaction(SIGTERM, action, NULL);
        if(result<0){
        	printf("failed to register\n");
        	exit(1);
        }
        while (1)
        {
            usleep(1000);
        }
        return 0;
    }else{
        //main function executes here
        int pid_sr = fork();
        if(pid_sr<0){
            printf("failed to create the child process\n");
            return -1;
        }else if (pid_sr == 0){
            // sr executes here
            char text[10];
            sprintf(text, "%d", pid_s1);
            char *args[] = {"./E1", text, NULL};
            execvp("./E1", args);
            return 0;
        }else{
           // main executes here
            int pid_st = fork();
            if(pid_st<0){
                printf("failed to create the child process\n");
                return -1;
            }else if (pid_st == 0){
                // st executes here
                char text2[10];
                sprintf(text2, "%d", pid_s1);
                char *args2[] = {"./E2", text2, NULL};
                execvp("./E2", args2);
                return 0;
            }else{ 
                while (1)
                {
            	    usleep(1000);
    	        }
            }
        }
    }
}
