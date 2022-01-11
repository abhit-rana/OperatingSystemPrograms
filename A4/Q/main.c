#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#define LEN 8

#define syscall_writer 449

#define syscall_reader 450


int main(int argc, char *argv[]){
    pid_t pprocess = fork();
    if(pprocess<0){
        printf("fork failed for pprocess!");
        exit(-1);
    }else if(pprocess==0){
        char bytesSend[LEN];
	    int fd;
        while(1){
            if((fd = open("/dev/urandom", O_RDONLY))<0){
                printf("failed to open the file\n");
                exit(-1);
            }
            int nOfBytes = 0;
            while(nOfBytes<LEN){
                if(read(fd, &bytesSend[nOfBytes], 1)<0){
                    printf("error in reading the %dth byte\n", nOfBytes);
                    exit(-1);
                }
                nOfBytes += 1;
            }
            printf("the bytes enqueued are ->%s<-\n", bytesSend);
            long res = syscall(syscall_writer, bytesSend);
            if(res<0){
                printf("the number recieved from the system call is %d\n", res);
                exit(-1);
            }
        }
    }else{
        pid_t cprocess = fork();
        if(cprocess<0){
            printf("fork failed for cprocess!");
            exit(-1);
        }else if(cprocess==0){
            char bytesRec[8];
            while(1){
                long res1 = syscall(syscall_reader, bytesRec);
                if(res1<0){
                    printf("the number recieved from the system call %d\n", res1);
                    exit(-1);
                }
                printf( "the bytes dequeued are ->%s<-\n", bytesRec);
            }
        }else{
            if(waitpid(pprocess, NULL, 0)<0){
                printf("error!");
                exit(-1);
            }
            if(waitpid(cprocess, NULL ,0)<0){
                printf("error!");
                exit(-1);
            }
        }
    }
    return 0;
}
