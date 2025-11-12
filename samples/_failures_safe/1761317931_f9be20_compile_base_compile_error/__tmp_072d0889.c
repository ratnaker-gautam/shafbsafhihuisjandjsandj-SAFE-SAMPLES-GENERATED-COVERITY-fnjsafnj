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

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ';
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
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (data == NULL) return STATE_ERROR;
            
            size_t data_len = strlen(data);
            if (data_len == 0 || data_len > MAX_INPUT_LEN - sm->buffer_len - 1) {
                return STATE_ERROR;
            }
            
            for (size_t i = 0; i < data_len; i++) {
                if (!is_valid_input_char(data[i])) {
                    return STATE_ERROR;
                }
            }
            
            if (sm->buffer_len + data_len < sizeof(sm->buffer)) {
                strncpy(sm->buffer + sm->buffer_len, data, data_len);
                sm->buffer_len += data_len;
                sm->buffer[sm->buffer_len] = '\0';
                sm->data_count++;
            }
            return STATE_READING;
            
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
            if (sm->buffer_len > 0) {
                printf("Processed %d data items: %s\n", sm->data_count, sm->buffer);
                return STATE_FINISHED;
            }
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
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

void print_state_info(State state) {
    switch (state) {
        case STATE_START: printf("Current state: START\n"); break;
        case STATE_READING: printf("Current state: READING\n"); break;
        case STATE_PROCESSING: printf("Current state: PROCESSING\n"); break;
        case STATE_FINISHED: printf("Current state: FINISHED\n"); break;
        case STATE_ERROR: printf("Current state: ERROR\n"); break;
    }
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("start, data <text>, complete, reset, quit\n");
    
    while (running) {
        print_state_info(sm.current_state);
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\