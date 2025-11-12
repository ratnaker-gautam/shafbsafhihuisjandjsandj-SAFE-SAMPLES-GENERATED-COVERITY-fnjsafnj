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
                    sm->data_count = 0;
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && validate_input(data)) {
                        size_t len = strlen(data);
                        if (len < MAX_INPUT_LEN) {
                            strncpy(sm->buffer, data, MAX_INPUT_LEN - 1);
                            sm->buffer[MAX_INPUT_LEN - 1] = '\0';
                            sm->buffer_len = len;
                            sm->data_count++;
                            return STATE_PROCESSING;
                        }
                    }
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    process_data(sm);
                    if (sm->data_count >= 3) {
                        return STATE_FINISHED;
                    }
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_RESET:
                    return STATE_START;
                default:
                    return STATE_FINISHED;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
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
    sm->buffer[0] = '\0';
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
    
    printf("Processing: ");
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            printf("%c", sm->buffer[i] - 32);
        } else {
            printf("%c", sm->buffer[i]);
        }
    }
    printf(" (count: %d)\n", sm->data_count);
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf("State Machine Demo - Enter text (or 'quit' to exit, 'reset' to restart):\n");
    
    while (1) {
        if (sm.current_state == STATE_START) {
            sm.current_state = handle_event(&sm, EVENT_START, NULL);
            printf("State: START -> READING\n");
        }
        
        if (sm.current_state == STATE_READING) {
            printf("Enter data: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                sm.current_state = handle_event(&sm, EVENT_ERROR, NULL);
                continue;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }
            
            if (strcmp(input, "quit") == 0) {
                break;
            } else if (strcmp(input, "reset") == 0) {
                sm.current_state = handle_event(&sm, EVENT_RESET, NULL);
                printf("State: RESET -> START\n");
                continue;
            }
            
            State new_state = handle_event(&sm, EVENT_DATA, input);
            printf("State: READING -> %s\n", 
                   new_state == STATE_PROCESSING ? "PROCESSING" : 
                   new_state == STATE_ERROR ? "ERROR" : "UNKNOWN");
            sm.current_state = new_state;
        }
        
        if (sm.current