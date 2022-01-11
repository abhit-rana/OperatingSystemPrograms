#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

pid_t pid_s1;
char *arr;

#define typename(x) _Generic((x), \
    unsigned long: "unsigned long", \
    int: "int", \
    default: "other")


void signalSendMessage( pid_t id, int signo, char *msg ){
	union sigval *sigdata;
    sigdata = (union sigval*)malloc( sizeof( union sigval* ));
	sigdata->sival_ptr = msg;
	int result = sigqueue ( id, signo, *sigdata );
    if(result<0){
        printf("failed to execute:%d\n", result);
        exit(1);
    }
    free(sigdata);
}

static inline unsigned long long getticks(void)
{
	unsigned int lo, hi;
	// RDTSC copies contents of 64-bit TSC into EDX:EAX
	asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
	return (unsigned long long)hi << 32 | lo;
}



void getDateAsString(){
    int day, month, year;
	time_t now;
	time(&now);
    struct tm *local = localtime(&now);
    day = local->tm_mday;
	month = local->tm_mon + 1;
	year = local->tm_year + 1900;
    char sday[10];
    char smonth[10];
    char syear[10];
    sprintf(sday, "%d", day);
    sprintf(smonth, "%d", month);
    sprintf(syear, "%d", year);
    char *arrh = (char*)calloc(50, sizeof(char));
    strcat(arrh, "Date: ");
    strcat(arrh, sday);
    strcat(arrh, "/");
    strcat(arrh, smonth);
    strcat(arrh, "/");
    strcat(arrh, syear);
    strcat(arrh, " ");
    arr = arrh;
}

void generateStringOfDandT(){
    uint32_t rand32;
 	unsigned long long val;
 	val = getticks();
    if(val<0){
        printf("failed to read the time stamp counter\n");
    }else{
        int second = val/2100000000;
 	    int minute = second/60;
 	    int hour = minute/60;
        char ssecond[10];
        char sminute[10];
        char shour[10];
        sprintf(ssecond, "%d", second);
        sprintf(sminute, "%d", minute);
        sprintf(shour, "%d", hour);
        getDateAsString();
        char *msg = (char*)calloc(100, sizeof(char));
        strcat(msg, arr);
        char timeWord[] = "Time: ";
        strcat(msg, timeWord);
        strcat(msg, shour);
        strcat(msg, "H ");
        strcat(msg, sminute);
        strcat(msg, "M ");
        strcat(msg, ssecond);
        strcat(msg, "S ");
        strcat(msg, "\0");
        signalSendMessage(pid_s1, SIGTERM, msg);
        sleep(2);
    }
}

void signal_handler_m(int signo, siginfo_t *siginfo, void *context){
	generateStringOfDandT();
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



// void getDateAsString(){
//     int day, month, year;
// 	time_t now;
// 	time(&now);
//     struct tm *local = localtime(&now);
//     day = local->tm_mday;
// 	month = local->tm_mon + 1;
// 	year = local->tm_year + 1900;
//     char sday[10];
//     char smonth[10];
//     char syear[10];
//     sprintf(sday, "%d", day);
//     sprintf(smonth, "%d", month);
//     sprintf(syear, "%d", year);
//     char arrh[] = "Date: ";
//     strcat(arrh, sday);
//     strcat(arrh, "/");
//     strcat(arrh, smonth);
//     strcat(arrh, "/");
//     strcat(arrh, syear);
//     //strcat(arrh, "\0");
//     printf("arrh:%s\n", arrh);
//     arr = arrh;
//     printf("arr:%s\n", arr);
//     //char arr[] = {dateWord, sday, '/', smonth, '/', syear, ' '};
    
// }

// void generateStringOfDandT(){
//     uint32_t rand32;
//  	unsigned long long val;
//  	val = getticks();
//     if(val<0){
//         printf("failed to read the time stamp counter\n");
//         //return -1;
//     }else{
//         int second = val/2100000000;
//  	    int minute = second/60;
//  	    int hour = minute/60;
//         char ssecond[10];
//         char sminute[10];
//         char shour[10];
//         sprintf(ssecond, "%d", second);
//         sprintf(sminute, "%d", minute);
//         sprintf(shour, "%d", hour);
//         getDateAsString();
//         char *msg = (char*)malloc(100*sizeof(char));
//         strcat(msg, arr);
//         printf("msg1:%s\n", msg);
//         char timeWord[] = "Time: ";
//         strcat(msg, timeWord);
//         printf("msg2:%s\n", msg);
//         strcat(msg, shour);
//         printf("msg3:%s\n", msg);
//         strcat(msg, "H ");
//         printf("msg4:%s\n", msg);
//         strcat(msg, sminute);
//         printf("msg5:%s\n", msg);
//         strcat(msg, "M ");
//         printf("msg6:%s\n", msg);
//         strcat(msg, ssecond);
//         printf("msg7:%s\n", msg);
//         strcat(msg, "S ");
//         printf("msg8:%s\n", msg);
//         strcat(msg, "\0");
//         printf("msg9:%s\n", msg);
//         printf("fms:%s\n", msg);
//         //printf("hello\n");
//         printf("final msg:%s\n", msg);
//         //char timeWord[] = "Time: ";
//         //char arr[] = {date, timeWord, shour, 'H', ' ', sminute, 'M', ' ', ssecond, 'S'};
//         signalSendMessage(pid_s1, SIGALRM, msg);
//     }
// }