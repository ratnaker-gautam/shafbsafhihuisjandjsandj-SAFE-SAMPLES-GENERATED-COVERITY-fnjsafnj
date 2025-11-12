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
    STATE_DONE,
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

State handle_event(StateMachine* sm, Event event, const char* data);
void initialize_machine(StateMachine* sm);
int validate_input(const char* input);
void process_data(StateMachine* sm);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        sum += (int)sm->buffer[i];
    }
    sm->data_count++;
    printf("Processed data: %s (sum: %d, count: %d)\n", sm->buffer, sum, sm->data_count);
}

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("State machine started\n");
                    return STATE_READING;
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
                            sm->buffer_len = len;
                            return STATE_PROCESSING;
                        }
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return sm->current_state;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    process_data(sm);
                    return STATE_READING;
                case EVENT_COMPLETE:
                    return STATE_DONE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return sm->current_state;
            }
            
        case STATE_DONE:
            return STATE_DONE;
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

int main() {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    sm.current_state = handle_event(&sm, EVENT_START, NULL);
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        if (sm.current_state == STATE_READING) {
            printf("Enter data (or 'quit' to finish, 'error' to force error): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                sm.current_state = STATE_ERROR;
                break;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }
            
            if (strcmp(input, "quit") == 0) {
                sm.current_state = handle_event(&sm, EVENT_COMPLETE, NULL);
            } else if (strcmp(input, "error") == 0) {
                sm.current_state = handle_event(&sm, EVENT_ERROR, NULL);
            } else {
                sm.current_state = handle_event(&sm, EVENT_DATA, input);
                if (sm.current_state == STATE_PROCESSING) {
                    sm.current_state = handle_event(&sm, EVENT_PROCESS, NULL);
                }
            }
        }
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("State machine completed successfully. Processed %d data items.\n", sm.data_count);
    } else {
        printf("State machine ended in error state.\n");
    }
    
    return sm.current_state == STATE_DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}