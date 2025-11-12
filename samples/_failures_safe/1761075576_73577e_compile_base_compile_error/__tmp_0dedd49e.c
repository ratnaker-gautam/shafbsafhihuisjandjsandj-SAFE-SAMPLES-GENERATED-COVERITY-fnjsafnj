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

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State handle_start_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                size_t data_len = strlen(data);
                if (data_len > 0 && data_len < MAX_INPUT_LEN - sm->buffer_len) {
                    strncat(sm->buffer, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            if (sm->data_count > 0) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State state_machine_transition(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event, data);
        case STATE_READING:
            return handle_reading_state(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event, data);
        case STATE_FINISHED:
            return handle_finished_state(sm, event, data);
        case STATE_ERROR:
            return handle_error_state(sm, event, data);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_START: printf("START"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_FINISHED: printf("FINISHED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf(", Data count: %d", sm->data_count);
    if (sm->buffer_len > 0) {
        printf(", Buffer: %s", sm->buffer);
    }
    printf("\n");
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo - Input Processing\n");
    printf("Available commands: start, data <text>, complete, error, reset, quit\n");
    
    char input[MAX_INPUT_LEN];
    char command[MAX_INPUT_LEN];
    char data[MAX_INPUT_LEN];
    
    while (1) {
        printf("\n> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) {
            continue;
        }
        
        memset(command, 0, sizeof(command));
        memset(data, 0, sizeof(data));
        
        if (sscanf(input, "%99s %99[^\n]", command, data) < 1) {
            printf("Invalid command\n");
            continue;
        }
        
        Event event;
        if (