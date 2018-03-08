//
//  main.c
//  maxTweeter
//
//  Created by Tony Xiao on 3/7/18.
//  Copyright © 2018 TonyXiao. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_NUM 20000 // Maxi number of line indicate in prompt
#define MAX_NAME_LEN 50 // Twitter allowed maximum username is 50 character
#define MAX_LINE_SIZE 1024 // MAX size of a line
#define NUM_USER_PRINT 10 // TOP 10 need to print

char* getfield(char* line, int num) { // get the content from Single line
    char* tok;
    
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")) {
        if (!num --)
            return tok;
    }
    return NULL;
}

int findNamePos(char* line) { // find the position of name
    int idx = 0;
    
    for(char *tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"), idx++){
        if(strcmp(tok, "\"name\"") == 0)
            return idx;
    }
    return -1;
}


int main(int argc, const char * argv[]) {
    if(argc != 2) { // invalid input
        printf("Invalid Input Format\n");
        return -1;
    }
    
    printf("File is %s\n", argv[1]);
    
    FILE *fileStream = NULL;
    fileStream = fopen(argv[1], "r"); // open file descriptor
    
    if(fileStream == NULL) { // cannot open file
        printf("Invalid Input Format\n");
        fclose(fileStream); // close file
        return -1;
    }
    
    int* count = (int *)calloc(MAX_LINE_NUM, sizeof(int));
    char** user = (char **)malloc(MAX_LINE_NUM * sizeof(char*));
    
    for(int i = 0; i < MAX_LINE_NUM; i ++) {
        user[i] = (char *)malloc(MAX_NAME_LEN * sizeof(char)); // initiallize the user list
        user[i][0] = '\0';
    }
    
    int lineNUM = 0;
    
    char line[MAX_LINE_SIZE]; // Size depends on the longest line in the csv
    fgets(line, MAX_LINE_SIZE, fileStream); // skip the first line
    char* tmp = strdup(line);
    int namePos = findNamePos(line);
    free(tmp);
    
    if(namePos == -1) { // no name pos found
        printf("Invalid Input Format\n");
        
        free(count);
        
        for(int i = 0; i < MAX_LINE_NUM; i ++)
            free(user[i]);
        
        free(user);
        fclose(fileStream); // close file
        return -1;
    }
    
    while (fgets(line, MAX_LINE_SIZE, fileStream)) { // parsing each line
        char* tmp = strdup(line);
        char* name = getfield(tmp, namePos);
        lineNUM ++;
    
        if(name == NULL || strlen(name) > MAX_NAME_LEN) { // no username or name length > 50
            printf("Invalid Input Format\n");
            free(tmp);
            free(count);
            
            for(int i = 0; i < MAX_LINE_NUM; i ++)
                free(user[i]);
            
            free(user);
            fclose(fileStream); // close file
            return -1;
        }
        
        int idx = 0;
        
        for(int i = 0; i < MAX_LINE_NUM; i++) {
            if(strcmp(user[i], name) == 0 || strlen(user[i]) == 0) { // if found or find an empty spot
                idx = i;
                break;
            }
        }
        
        strcpy(user[idx], name); //insert the name
        count[idx] ++; // increase the count
        free(tmp); // free tmp var
    }
    
    for(int i = 0; i < NUM_USER_PRINT; i++) { // find the top # user
        int max = 0;
        int idx = 0;
        
        for(int j = 0; j < lineNUM; j++) { // find the max # in count
            if(count[j] > max) {
                max = count[j];
                idx = j;
            }
            
            if(count[j] == 0)
                break;
        }
        
        if(count[idx] == -1)
            continue;
        
        printf("%s:%d\n", user[idx], count[idx]);
        count[idx] = -1;
    }
    
//    free all the ptr
    free(count);
    
    for(int i = 0; i < MAX_LINE_NUM; i ++)
        free(user[i]);

    free(user);
    fclose(fileStream); // close file
    return 0;
}



