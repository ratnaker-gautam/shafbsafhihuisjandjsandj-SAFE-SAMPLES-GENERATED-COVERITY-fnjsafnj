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
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_finished_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

State transition_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_FINISHED:
            return handle_finished_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            sm->buffer_len = 0;
            sm->data_count = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                char input[MAX_INPUT_LEN];
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len-1] == '\n') {
                        input[len-1] = '\0';
                        len--;
                    }
                    if (sm->buffer_len + len < MAX_INPUT_LEN) {
                        strncpy(sm->buffer + sm->buffer_len, input, len);
                        sm->buffer[sm->buffer_len + len] = '\0';
                        sm->buffer_len += len;
                        sm->data_count++;
                        printf("Data received: %s (Total: %zu chars, %d items)\n", 
                               input, sm->buffer_len, sm->data_count);
                    }
                }
            }
            if (sm->data_count >= 3) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data...\n");
            printf("Total characters: %zu\n", sm->buffer_len);
            printf("Total data items: %d\n", sm->data_count);
            printf("Combined data: %s\n", sm->buffer);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete.\n");
            return STATE_FINISHED;
        case EVENT_START:
            return handle_start_state(sm, EVENT_START);
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_START:
            return handle_start_state(sm, EVENT_START);
        default:
            printf("Error state. Use START to reset.\n");
            return STATE_ERROR;
    }
}

void initialize_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        sm->current_state = STATE_START;
        sm->buffer_len = 0;
        sm->data_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
    }
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, complete, error, quit\n");
    
    char command[MAX_INPUT_LEN];
    
    while (1) {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(command);
        if (len > 0 && command