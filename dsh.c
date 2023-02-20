/*
 * dsh.c
 *
 *  Created on: Feb 19 23
 *      Author: Zale Fadiman
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#define MAXBUF 256

void method1(){
    //setup;
    pid_t pid=1;
    char *args[]={NULL,NULL};
    char *line = (char*) malloc(256);
    char *path = (char*) malloc(256);
    printf("welcome to zale's shell please enter a command\n");
    //main running loop
    while(pid!=0){
        strcpy(line, "");
        fgets(line, 256, stdin); 

        //trim
        int i=0;
        strcpy(path, "");
        while(strcmp(&line[i],"\0")!=0){
            if(line[i]!=' '){
                if(line[i]!='\n'){
                strncat(path, &line[i], 1);
                }
           }
            i++;
        }

        //check if it is a predefined command
        if(strcmp(path, "exit")==0){
            break;
        }

        else if(strcmp(path, "pwd")==0){
            printf("%s\n", getcwd(path, MAXBUF));
        }

        else if(strncmp(line, "cd", 2)==0){
            /*this command has to use the raw line because it needs the space
            but it also can't have the new line character so this first bit of code
            removes the trailing new line so that the string is properly formatted
            */
             char * p=strchr(line,'\n');
             line[p-line]='\0';   
             char * token = strtok(line, " ");
             int t=0;
             while(token !=NULL){
                if (t==1){
                if(chdir(token)!=0){
                    chdir(getenv("HOME"));
                    printf("oops directory not found returning to home\n");
                }
                }
                token = strtok(NULL, " ");
                t++;
             }
        }

        //check if direct path exists
        else if(path[0]=='/') {
            int b=0;
            if(path[strlen(path)-1]=='&'){
                    path[strlen(path)-1]='\0';
                    b=1;
                }
            if(access(path, F_OK | X_OK) == 0){
                pid= fork();
                if(pid!=0){
                    if(b==0){
                        wait(NULL);
                    }
                }
                else{
                    execv(path, args);
                }
            }
            else{
                printf("no file found with name\n");
            }
        }
        //check if file exists
        else {
           char *temp = (char*) malloc(257); 
           strcpy(temp,"/");
           strcat(temp,path);
           strcat(getcwd(line, MAXBUF),temp);

           if(access(line, F_OK | X_OK) == 0){
                pid= fork();
                if(pid!=0){
                    wait(NULL);
                }
                else{
                    execv(line, args);
    
                }
            }

            else{
                int r=0;
                char * token = strtok(getenv("PATH"), ":");
                while(token !=NULL){
                    strcpy(path,token);
                    strcat(path, temp);
                    if(access(path, F_OK | X_OK) == 0){
                        pid= fork();
                        if(pid!=0){
                            wait(NULL);
                            r=1;
                            break;
                        }
                        else{
                            execv(path, args);
                        }
                    }  
                    token = strtok(NULL, ":");
                }
                if(r==0){
                    printf("no file or command found\n");
                }
            }
        }
    }
}
