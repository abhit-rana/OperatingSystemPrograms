#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

pid_t pid_s1;

void signalSendValue( pid_t id, int signo, unsigned long val ){
	union sigval *sigdata;
	sigdata = malloc ( sizeof( union sigval ) );
	sigdata->sival_int = val;
	sigqueue( id, signo, *sigdata );
	free( sigdata );
}

int rdrand32_step (uint32_t *rand){
	unsigned char ok;
	asm volatile ("rdrand %0; setc %1"
		: "=r" (*rand), "=qm" (ok));
	return (int)ok;
}

void generateRandomNumber(){
    uint32_t rand32;
    if( !rdrand32_step(&rand32)){
        printf("failed to generate random number\n");
    }else{
        signalSendValue(pid_s1, SIGTERM, (unsigned long)rand32);
        sleep(2);
    }
}

void signal_handler_m(int signo, siginfo_t *siginfo, void *context){
	generateRandomNumber();
}

int main(int argc, char *argv[]){
        struct sigaction *action;
        struct itimerval timer;
        action = malloc(sizeof(struct sigaction));
        action->sa_sigaction = &signal_handler_m;
        action->sa_flags = SA_SIGINFO;
        sigaction(SIGALRM, action, NULL);
        pid_s1 = atoi(argv[1]);
        timer.it_value.tv_sec = 1;
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 1;
        timer.it_interval.tv_usec = 0;
        int result = setitimer(ITIMER_REAL, &timer, NULL);
        if(result<0){
            printf("error on setting the time interval\n");
            return -1;
        }
        while (1);
}
