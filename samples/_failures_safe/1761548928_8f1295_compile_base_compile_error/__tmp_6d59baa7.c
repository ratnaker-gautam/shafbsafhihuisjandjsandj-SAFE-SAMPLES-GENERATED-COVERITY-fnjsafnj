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
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} StateMachine;

State handle_event(StateMachine* sm, Event event, const char* data);
void initialize_machine(StateMachine* sm);
int validate_input(const char* input, size_t len);
void process_data(StateMachine* sm);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->processed_count = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    sm->processed_count++;
}

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && validate_input(data, strlen(data))) {
                        size_t data_len = strlen(data);
                        if (data_len < sizeof(sm->buffer)) {
                            strncpy(sm->buffer, data, sizeof(sm->buffer) - 1);
                            sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                            sm->buffer_len = data_len;
                            return STATE_PROCESSING;
                        }
                    }
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    process_data(sm);
                    return STATE_FINISHED;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_COMPLETE:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf("State Machine Demo - Text Processing\n");
    printf("Enter text to process (or 'quit' to exit):\n");
    
    while (1) {
        if (sm.current_state == STATE_START) {
            sm.current_state = handle_event(&sm, EVENT_START, NULL);
            if (sm.current_state == STATE_ERROR) {
                printf("Error: Failed to start state machine\n");
                return 1;
            }
        }
        
        if (sm.current_state == STATE_READING) {
            printf("Enter text: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Error reading input\n");
                return 1;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }
            
            if (strcmp(input, "quit") == 0) {
                break;
            }
            
            sm.current_state = handle_event(&sm, EVENT_DATA, input);
            if (sm.current_state == STATE_ERROR) {
                printf("Error: Invalid input\n");
                initialize_machine(&sm);
                continue;
            }
        }
        
        if (sm.current_state == STATE_PROCESSING) {
            sm.current_state = handle_event(&sm, EVENT_PROCESS, NULL);
            if (sm.current_state == STATE_ERROR) {
                printf("Error: Processing failed\n");
                initialize_machine(&sm);
                continue;
            }
        }
        
        if (sm.current_state == STATE_FINISHED) {
            printf("Processed text: %s\n", sm.buffer);
            printf("Total processed: %d\n", sm.processed_count);
            sm.current_state = handle_event(&sm, EVENT_COMPLETE, NULL);
        }
        
        if (sm.current_state == STATE_ERROR) {
            printf("Error: State machine error occurred\n