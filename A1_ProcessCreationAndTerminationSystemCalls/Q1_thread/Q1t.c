#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include<fcntl.h>

void* CalcAvg(void* arg){
	int n;
	int o;
	int fd;
	fd = open("student_record.csv", O_RDWR);
	char *buff = (char*)malloc(1*sizeof(char));
	char *arr = (char*)malloc(1*sizeof(char));
	int count = 0;
	int ret;
	int size = 1;
	int flag = 0;
	printf("--------------Average of students of Section:%c-------------\n", *(char*)arg);
	char line[100];
	float sumOfmarksOfStudent = 0;
    float sumOfSectionAvgMarks = 0;
	int i = 0;
	int j = 0;
	float *AssignmentMarks = (float*)calloc(6, sizeof(float));
	int flag2;
	int temp;
	while(1){
		ret = read(fd, buff, 1);
		arr[size-1] = *buff;
		if(ret<0){
			arr[size-1] = '\0';
			printf("Error occured, could not read the file\n");
			exit(-1);
		}else if(ret==0){
			break;
		}else if(arr[size-1]!='\n'){
			size += 1;
			arr = (char*)realloc(arr, size*sizeof(char));
			continue;
		}else{
			arr[size-1] = '\0';
			if(flag){
				flag2 = 0;
				char *token;
				char _id[6];
				token = strtok(arr, ",");
				strcpy(_id, token);
				j = 0;
				sumOfmarksOfStudent = 0;
				while(token != NULL){
					if(j==1){
						if(strcmp(token, (char*)arg)==0){
							i += 1;
							flag2 = 1;
						}else{
							break;
						}
					}
					if(flag2 && j>1){
						temp = atoi(token);
						AssignmentMarks[j-2] += temp;
					}
					token = strtok(NULL, ",");
					j += 1;
				}
			}
			free(buff);
			char *arr = (char*)malloc(1*sizeof(char));
			size = 1;
			if(!flag){
				flag = 1;
			}
		}
	}
	int t = 0;
	while (t<6)
	{
		printf("Average Marks of Assignment:%d is %f\n", t+1, AssignmentMarks[t]/i);
		t+=1;
	}
	
}





int main(int argc, char* argv[]){
	pthread_t t1, t2;
	char ch[] = "A";
    	int ret;
    	ret = pthread_create(&t1, NULL, &CalcAvg, &ch);
    	if(ret){
        	printf("Error in creating thread, %d\n", ret);
        	exit(-1);
    	}
	pthread_join(t1, NULL);
	ch[0] = 'B';
    	ret = pthread_create(&t2, NULL, &CalcAvg, &ch);
    	if(ret){
        	printf("Error in creating thread, %d\n", ret);
        	exit(-1);
    	}
	pthread_join(t2, NULL);
	return 0;
}
