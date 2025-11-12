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
int validate_input(const char* input, size_t len);
void initialize_machine(StateMachine* sm);
void cleanup_machine(StateMachine* sm);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->processed_count = 0;
}

void cleanup_machine(StateMachine* sm) {
    if (sm == NULL) return;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    sm->current_state = STATE_READING;
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && validate_input(data, strlen(data))) {
                        size_t data_len = strlen(data);
                        if (data_len < sizeof(sm->buffer) - sm->buffer_len) {
                            memcpy(sm->buffer + sm->buffer_len, data, data_len);
                            sm->buffer_len += data_len;
                            sm->current_state = STATE_PROCESSING;
                        } else {
                            sm->current_state = STATE_ERROR;
                        }
                    } else {
                        sm->current_state = STATE_ERROR;
                    }
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    if (sm->buffer_len > 0) {
                        sm->processed_count++;
                        sm->buffer_len = 0;
                        memset(sm->buffer, 0, sizeof(sm->buffer));
                        sm->current_state = STATE_FINISHED;
                    } else {
                        sm->current_state = STATE_ERROR;
                    }
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_COMPLETE:
                    sm->current_state = STATE_START;
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_ERROR:
            sm->current_state = STATE_START;
            break;
            
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
    
    return sm->current_state;
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Enter data to process (max %d chars):\n", MAX_INPUT_LEN - 1);
    
    while (sm.current_state != STATE_ERROR && sm.processed_count < 3) {
        switch (sm.current_state) {
            case STATE_START:
                printf("Starting state machine...\n");
                handle_event(&sm, EVENT_START, NULL);
                break;
                
            case STATE_READING:
                printf("Enter data: ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                        len--;
                    }
                    handle_event(&sm, EVENT_DATA, input);
                } else {
                    handle_event(&sm, EVENT_ERROR, NULL);
                }
                break;
                
            case STATE_PROCESSING:
                printf("Processing data: %s\n", sm.buffer);
                handle_event(&sm, EVENT_PROCESS, NULL);
                break;
                
            case STATE_FINISHED:
                printf("Processed %d items. Continue? (y/n): ", sm.processed_count);
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    if (input[0] == 'y' || input[0] == 'Y') {
                        handle_event(&sm, EVENT_COMP