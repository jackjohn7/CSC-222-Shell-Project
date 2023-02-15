#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include<sys/wait.h>
#include <errno.h>
#include <stdlib.h>
static const int size=256;

// Major blocks are understanding the file thing, fork and repeating, displaying error message

int main(){
    // int flag = 1;
    char input[256];
    pid_t pid = fork();
    int status;
    int n = 20;

    for (int id=0; id<n; id++) { 
        if ((pid = fork()) == 0) {
            char inpt[256]; 
            fgets(input, size, stdin);       

            // handles files
            
            // FILE* infile = fopen(my_input_file, "r");
            // dup2(fileno(infile), 0);
            // fclose(infile);

            // FILE* outfile = fopen(my_output_file, "w");
            // dup2(fileno(outfile), 1);
            // fclose(outfile);

            char str2[10] = "exit\n";
            if ((strcmp(inpt,str2)) == 0) {
                pid++;
                break;
            }
            else {

            input[strlen(input)-1] = '\0';

            char *tokens = strtok(input, " \n");
            char *inpt_tokens[size];

            int i = 0;
            while (tokens) {
                inpt_tokens[i++] = tokens;
                tokens = strtok(NULL, " \n");
            }

            inpt_tokens[i] = NULL;
            execvp(inpt_tokens[0], inpt_tokens);
            exit(errno);
            }
        }
    }
    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if(status)
            printf("%s\n", strerror(errno));
    }
    return 0;
}