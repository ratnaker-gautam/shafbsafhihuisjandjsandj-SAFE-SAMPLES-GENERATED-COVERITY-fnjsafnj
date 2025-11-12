//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(State current, Event event, StateMachine* sm);
int validate_input(const char* input, size_t len);
void initialize_machine(StateMachine* sm);
void process_data(StateMachine* sm);

State handle_event(State current, Event event, StateMachine* sm) {
    switch(current) {
        case STATE_START:
            if (event == EVENT_START) {
                return STATE_READING;
            }
            break;
        case STATE_READING:
            if (event == EVENT_DATA) {
                return STATE_PROCESSING;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS) {
                process_data(sm);
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
        case STATE_FINISHED:
            if (event == EVENT_COMPLETE) {
                return STATE_START;
            }
            break;
        case STATE_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
    return current;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_count = 0;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    int digit_count = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= '0' && sm->buffer[i] <= '9') {
            digit_count++;
        }
    }
    sm->data_count = digit_count;
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf("State Machine Demo - Enter data lines (empty to quit):\n");
    
    while (1) {
        switch(sm.current_state) {
            case STATE_START:
                printf("> ");
                if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
                    sm.current_state = STATE_ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                    len--;
                }
                
                if (len == 0) {
                    printf("Exiting...\n");
                    return 0;
                }
                
                if (!validate_input(input, len)) {
                    sm.current_state = handle_event(sm.current_state, EVENT_ERROR, &sm);
                    printf("Invalid input detected\n");
                } else {
                    strncpy(sm.buffer, input, MAX_INPUT_LEN-1);
                    sm.buffer[MAX_INPUT_LEN-1] = '\0';
                    sm.buffer_len = len;
                    sm.current_state = handle_event(sm.current_state, EVENT_DATA, &sm);
                }
                break;
                
            case STATE_READING:
                sm.current_state = handle_event(sm.current_state, EVENT_PROCESS, &sm);
                break;
                
            case STATE_PROCESSING:
                printf("Processing data...\n");
                sm.current_state = handle_event(sm.current_state, EVENT_PROCESS, &sm);
                break;
                
            case STATE_FINISHED:
                printf("Processed %d digits in input\n", sm.data_count);
                sm.current_state = handle_event(sm.current_state, EVENT_COMPLETE, &sm);
                break;
                
            case STATE_ERROR:
                printf("Error state reached. Resetting...\n");
                initialize_machine(&sm);
                break;
        }
    }
    
    return 0;
}