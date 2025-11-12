//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_EXIT
} State;

typedef State (*StateHandler)(const char*);

State handle_idle(const char* input) {
    if (input[0] == 's') {
        printf("Starting read operation\n");
        return STATE_READING;
    } else if (input[0] == 'q') {
        return STATE_EXIT;
    } else {
        printf("Invalid command. Use 's' to start or 'q' to quit\n");
        return STATE_IDLE;
    }
}

State handle_reading(const char* input) {
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return STATE_READING;
    }
    
    printf("Read data: %s\n", input);
    printf("Data processed successfully\n");
    return STATE_PROCESSING;
}

State handle_processing(const char* input) {
    if (input[0] == 'w') {
        printf("Writing processed data\n");
        return STATE_WRITING;
    } else if (input[0] == 'r') {
        printf("Returning to read mode\n");
        return STATE_READING;
    } else {
        printf("Use 'w' to write or 'r' to read again\n");
        return STATE_PROCESSING;
    }
}

State handle_writing(const char* input) {
    printf("Data written successfully\n");
    printf("Returning to idle state\n");
    return STATE_IDLE;
}

State handle_exit(const char* input) {
    printf("Exiting program\n");
    return STATE_EXIT;
}

int main() {
    State current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_exit
    };
    
    printf("State Machine Program Started\n");
    printf("Commands: s=start, q=quit, w=write, r=read again\n");
    
    while (current_state != STATE_EXIT) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            default: break;
        }
        printf("\nEnter input: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (len >= MAX_INPUT_LEN) {
            printf("Input too long\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (current_state >= 0 && current_state < MAX_STATES) {
            current_state = handlers[current_state](input);
        } else {
            printf("Invalid state detected\n");
            break;
        }
    }
    
    return 0;
}