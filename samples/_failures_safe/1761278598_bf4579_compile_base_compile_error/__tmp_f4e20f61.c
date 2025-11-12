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
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, process, complete, error, reset, quit\n");
    
    char input[MAX_INPUT_LEN];
    while (sm.current_state != STATE_FINISHED) {
        printf("Current state: %d > ", sm.current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strncmp(input, "data ", 5) == 0) {
            const char* data = input + 5;
            if (validate_input(data, strlen(data))) {
                size_t copy_len = strlen(data);
                if (copy_len >= MAX_INPUT_LEN - sm.buffer_len) {
                    copy_len = MAX_INPUT_LEN - sm.buffer_len - 1;
                }
                if (copy_len > 0) {
                    memcpy(sm.buffer + sm.buffer_len, data, copy_len);
                    sm.buffer_len += copy_len;
                    sm.buffer[sm.buffer_len] = '\0';
                    event = EVENT_DATA;
                } else {
                    event = EVENT_ERROR;
                }
            } else {
                event = EVENT_ERROR;
            }
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
        } else if (strcmp(input, "complete") == 0) {
            event = EVENT_COMPLETE;
        } else if (strcmp(input, "error") == 0) {
            event = EVENT_ERROR;
        } else if (strcmp(input, "reset") == 0) {
            event = EVENT_RESET;
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else {
            printf("Unknown command\n");
            continue;
        }
        
        State new_state = handle_event(sm.current_state, event, &sm);
        sm.current_state = new_state;
        
        if (sm.current_state == STATE_PROCESSING) {
            printf("Processing %zu bytes of data\n", sm.buffer_len);
            sm.data_count++;
        } else if (sm.current_state == STATE_FINISHED) {
            printf("Finished processing %d data items\n", sm.data_count);
        } else if (sm.current_state == STATE_ERROR) {
            printf("Error state reached\n");
        }
    }
    
    return 0;
}

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    sm->data_count = 0;
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

State handle_event(State current, Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("Machine started\n");
                    return STATE_READING;
                case EVENT_RESET:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    printf("Data received: %s\n", sm->buffer);
                    return STATE_READING;
                case EVENT_PROCESS:
                    if (sm->buffer_len > 0) {
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
                    sm->buffer[0] = '\0';
                    sm->buffer_len = 0;
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FIN