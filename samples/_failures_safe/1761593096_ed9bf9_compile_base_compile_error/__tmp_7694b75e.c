//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
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
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_length;
    int data_count;
} StateMachine;

State handle_event(StateMachine* sm, Event event, const char* data);
void initialize_machine(StateMachine* sm);
int validate_input(const char* input);
void process_data(StateMachine* sm);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_length = 0;
    sm->data_count = 0;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LENGTH) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_length == 0) return;
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_length; i++) {
        sum += (int)sm->buffer[i];
    }
    printf("Processed data: %s (sum: %d)\n", sm->buffer, sum);
    sm->data_count++;
}

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("State machine started\n");
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && validate_input(data)) {
                        size_t len = strlen(data);
                        if (len < sizeof(sm->buffer)) {
                            strncpy(sm->buffer, data, sizeof(sm->buffer) - 1);
                            sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                            sm->buffer_length = len;
                            printf("Data received: %s\n", sm->buffer);
                            return STATE_PROCESSING;
                        }
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    process_data(sm);
                    return STATE_FINISHED;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_COMPLETE:
                    printf("Processing complete. Total data processed: %d\n", sm->data_count);
                    return STATE_FINISHED;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                case EVENT_ERROR:
                    return STATE_ERROR;
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

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input[MAX_INPUT_LENGTH];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, process, complete, reset, quit\n");
    
    while (running) {
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_FINISHED: printf("FINISHED"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
        } else if (strcmp(input, "start") == 0) {
            sm.current_state = handle_event(&sm, EVENT_START, NULL);
        } else if (strncmp(input, "data ", 5) == 0) {
            const char* data =