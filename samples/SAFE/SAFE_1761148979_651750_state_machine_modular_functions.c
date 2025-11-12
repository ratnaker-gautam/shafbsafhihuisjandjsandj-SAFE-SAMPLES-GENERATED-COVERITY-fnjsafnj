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

State handle_event(StateMachine *sm, Event event, const char *data);
void initialize_machine(StateMachine *sm);
int validate_input(const char *input);
void process_data(StateMachine *sm);

State handle_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                sm->processed_count = 0;
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (data != NULL && validate_input(data)) {
                    size_t len = strlen(data);
                    if (len < MAX_INPUT_LEN) {
                        strncpy(sm->buffer, data, MAX_INPUT_LEN - 1);
                        sm->buffer[MAX_INPUT_LEN - 1] = '\0';
                        sm->buffer_len = len;
                        return STATE_PROCESSING;
                    }
                }
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS) {
                process_data(sm);
                return STATE_FINISHED;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_COMPLETE) {
                return STATE_READING;
            }
            break;
            
        default:
            break;
    }
    
    return STATE_ERROR;
}

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    sm->processed_count = 0;
}

int validate_input(const char *input) {
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

void process_data(StateMachine *sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    int word_count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] == ' ' || sm->buffer[i] == '\t' || sm->buffer[i] == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }
    
    if (word_count > 0) {
        sm->processed_count += word_count;
    }
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Word Counter\n");
    printf("Enter text lines (empty line to exit):\n");
    
    machine.current_state = handle_event(&machine, EVENT_START, NULL);
    
    while (machine.current_state != STATE_ERROR) {
        switch (machine.current_state) {
            case STATE_READING:
                if (fgets(input, MAX_INPUT_LEN, stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    
                    if (strlen(input) == 0) {
                        printf("Total words processed: %d\n", machine.processed_count);
                        return 0;
                    }
                    
                    machine.current_state = handle_event(&machine, EVENT_DATA, input);
                } else {
                    machine.current_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                machine.current_state = handle_event(&machine, EVENT_PROCESS, NULL);
                break;
                
            case STATE_FINISHED:
                printf("Processed: %s (Total: %d words)\n", machine.buffer, machine.processed_count);
                machine.current_state = handle_event(&machine, EVENT_COMPLETE, NULL);
                break;
                
            default:
                machine.current_state = STATE_ERROR;
                break;
        }
    }
    
    printf("Error occurred during processing\n");
    return 1;
}