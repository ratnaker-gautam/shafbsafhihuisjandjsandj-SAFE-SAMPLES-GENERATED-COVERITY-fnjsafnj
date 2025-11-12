//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(from, to) if(current_state == (from)) { current_state = (to); }
#define VALID_TRANSITION(from, to) ((current_state == (from)) && (next_state == (to)))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *tokens[MAX_TOKENS];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int next_state = STATE_IDLE;
    
    printf("Enter command (start/process/done/quit): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return EXIT_FAILURE;
    }
    
    char *token = strtok(input, " ");
    while (token != NULL && token_count < MAX_TOKENS) {
        tokens[token_count] = token;
        token_count++;
        token = strtok(NULL, " ");
    }
    
    for (int i = 0; i < token_count; i++) {
        char command[20];
        strncpy(command, tokens[i], sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';
        
        for (int j = 0; command[j]; j++) {
            command[j] = tolower((unsigned char)command[j]);
        }
        
        next_state = current_state;
        
        switch (current_state) {
            case STATE_IDLE:
                if (strcmp(command, "start") == 0) {
                    next_state = STATE_READING;
                } else if (strcmp(command, "quit") == 0) {
                    next_state = STATE_DONE;
                } else {
                    next_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (strcmp(command, "process") == 0) {
                    next_state = STATE_PROCESSING;
                } else if (strcmp(command, "quit") == 0) {
                    next_state = STATE_DONE;
                } else {
                    next_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                if (strcmp(command, "done") == 0) {
                    next_state = STATE_DONE;
                } else if (strcmp(command, "quit") == 0) {
                    next_state = STATE_DONE;
                } else {
                    next_state = STATE_ERROR;
                }
                break;
                
            case STATE_DONE:
                break;
                
            case STATE_ERROR:
                break;
        }
        
        if (VALID_TRANSITION(current_state, next_state)) {
            TRANSITION(current_state, next_state);
        } else {
            current_state = STATE_ERROR;
        }
        
        switch (current_state) {
            case STATE_IDLE:
                printf("State: IDLE\n");
                break;
            case STATE_READING:
                printf("State: READING\n");
                break;
            case STATE_PROCESSING:
                printf("State: PROCESSING\n");
                break;
            case STATE_DONE:
                printf("State: DONE\n");
                break;
            case STATE_ERROR:
                printf("State: ERROR\n");
                break;
        }
        
        if (current_state == STATE_DONE || current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Operation completed successfully\n");
    } else if (current_state == STATE_ERROR) {
        printf("Invalid state transition\n");
    }
    
    return EXIT_SUCCESS;
}