//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(State current, Event event, StateMachine* sm);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (!(input[i] >= 'a' && input[i] <= 'z') && 
            !(input[i] >= 'A' && input[i] <= 'Z') &&
            !(input[i] >= '0' && input[i] <= '9') &&
            input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

Event get_event_from_input(const char* input, size_t len) {
    if (input == NULL || len == 0) return EVENT_ERROR;
    
    if (len == 5 && strncmp(input, "start", 5) == 0) return EVENT_START;
    if (len == 4 && strncmp(input, "data", 4) == 0) return EVENT_DATA;
    if (len == 7 && strncmp(input, "process", 7) == 0) return EVENT_PROCESS;
    if (len == 8 && strncmp(input, "complete", 8) == 0) return EVENT_COMPLETE;
    if (len == 5 && strncmp(input, "reset", 5) == 0) return EVENT_RESET;
    if (len == 4 && strncmp(input, "quit", 4) == 0) return EVENT_ERROR;
    
    return EVENT_ERROR;
}

State handle_event(State current, Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("Machine started\n");
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (sm->data_count < 3) {
                        sm->data_count++;
                        printf("Data received (%d/3)\n", sm->data_count);
                        return STATE_READING;
                    } else {
                        printf("Maximum data reached\n");
                        return STATE_PROCESSING;
                    }
                case EVENT_PROCESS:
                    if (sm->data_count > 0) {
                        printf("Processing data\n");
                        return STATE_PROCESSING;
                    }
                    return STATE_ERROR;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_COMPLETE:
                    printf("Processing complete\n");
                    return STATE_FINISHED;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

void print_state(State state) {
    switch (state) {
        case STATE_START: printf("Current state: START\n"); break;
        case STATE_READING: printf("Current state: READING\n"); break;
        case STATE_PROCESSING: printf("Current state: PROCESSING\n"); break;
        case STATE_FINISHED: printf("Current state: FINISHED\n"); break;
        case STATE_ERROR: printf("Current state: ERROR\n"); break;
    }
}

int main() {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    size_t input_len;
    
    initialize_machine(&sm);
    
    printf("State Machine Controller\n");
    printf("Available commands: start, data, process, complete, reset, quit\n");
    
    while (1) {
        print_state(sm.current_state);
        printf("Enter command: ");
        
        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
            printf("Input error\n");
            break;
        }
        
        input_len = strlen(input);
        if (input_len >