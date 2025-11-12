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

State handle_event(StateMachine* sm, Event event, const char* data);
void initialize_machine(StateMachine* sm);
int validate_input(const char* input);
void process_data(StateMachine* sm);

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
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
                        if (len < MAX_INPUT_LEN && sm->buffer_len + len < MAX_INPUT_LEN) {
                            strncpy(sm->buffer + sm->buffer_len, data, len);
                            sm->buffer_len += len;
                            sm->data_count++;
                            return STATE_READING;
                        }
                    }
                    return STATE_ERROR;
                case EVENT_PROCESS:
                    if (sm->data_count > 0) {
                        return STATE_PROCESSING;
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_COMPLETE:
                    process_data(sm);
                    return STATE_FINISHED;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_FINISHED;
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

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    printf("Processing %d data entries:\n", sm->data_count);
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] != '\0') {
            putchar(sm->buffer[i]);
        }
    }
    printf("\n");
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: data <text>, process, complete, reset, quit\n");
    
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
        
        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
            sm.current_state = handle_event(&sm, EVENT_ERROR, NULL);
            continue;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
        } else if (strncmp(input, "data ", 5) == 0) {
            const char* data = input + 5;
            sm.current_state = handle_event(&sm, EVENT_DATA, data);
        } else if (strcmp(input, "process") == 0) {
            sm.current_state = handle_event(&sm, EVENT_PROCESS, NULL);
        } else if (strcmp(input, "complete") == 0) {
            sm.current