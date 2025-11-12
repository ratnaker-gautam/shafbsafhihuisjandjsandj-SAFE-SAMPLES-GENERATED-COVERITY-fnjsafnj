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
void initialize_machine(StateMachine* sm);
int validate_input(const char* input, size_t len);
Event get_next_event(void);

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    printf("State Machine Demo - Enter commands: start, data, process, complete, error, reset, quit\n");
    
    while (sm.current_state != STATE_FINISHED) {
        char input[MAX_INPUT_LEN];
        
        printf("Current state: %d > ", sm.current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "data") == 0) {
            event = EVENT_DATA;
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
        } else if (strcmp(input, "complete") == 0) {
            event = EVENT_COMPLETE;
        } else if (strcmp(input, "error") == 0) {
            event = EVENT_ERROR;
        } else if (strcmp(input, "reset") == 0) {
            event = EVENT_RESET;
        } else {
            printf("Invalid command\n");
            continue;
        }
        
        State new_state = handle_event(sm.current_state, event, &sm);
        sm.current_state = new_state;
        
        if (sm.current_state == STATE_ERROR) {
            printf("State machine entered error state\n");
        } else if (sm.current_state == STATE_FINISHED) {
            printf("State machine completed successfully. Processed %d data items\n", sm.data_count);
        }
    }
    
    return 0;
}

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State handle_event(State current, Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("Transitioning to READING state\n");
                    return STATE_READING;
                case EVENT_RESET:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (sm->data_count < MAX_STATES) {
                        sm->data_count++;
                        printf("Data received, count: %d\n", sm->data_count);
                        return STATE_READING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_PROCESS:
                    if (sm->data_count > 0) {
                        printf("Transitioning to PROCESSING state\n");
                        return STATE_PROCESSING;
                    } else {
                        return STATE_ERROR;
                    }
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
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
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

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    
    return 1;
}

Event get_next_event(void) {
    return EVENT_START;
}