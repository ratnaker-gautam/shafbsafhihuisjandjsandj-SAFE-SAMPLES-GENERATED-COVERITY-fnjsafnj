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
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_start_state(StateMachine *sm, Event event, char data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event, char data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1 && is_valid_input_char(data)) {
                sm->buffer[sm->buffer_len++] = data;
                return STATE_READING;
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            if (sm->buffer_len > 0) {
                sm->buffer[sm->buffer_len] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event, char data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            sm->data_count++;
            printf("Processed: %s (count: %d)\n", sm->buffer, sm->data_count);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event, char data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event, char data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State state_machine_transition(StateMachine *sm, Event event, char data) {
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

Event get_next_event(char input_char, int *end_of_input) {
    if (end_of_input == NULL) return EVENT_ERROR;
    
    if (input_char == EOF) {
        *end_of_input = 1;
        return EVENT_COMPLETE;
    }
    
    if (input_char == '!') {
        return EVENT_ERROR;
    }
    
    if (input_char == '#') {
        return EVENT_RESET;
    }
    
    return EVENT_DATA;
}

void print_state_info(State state) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Enter text (max %d chars). Use '!' for error, '#' for reset, Ctrl+D to finish.\n", MAX_INPUT_LEN - 1);
    
    int end_of_input = 0;
    int input_char;
    
    while (!end_of_input && sm.current_state != STATE_ERROR) {
        print_state_info(sm.current_state);
        
        if (sm.current_state == STATE_START) {
            sm.current_state = state_machine_transition(&sm, EVENT_START, 0);
            continue;
        }
        
        if (sm.current_state