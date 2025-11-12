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

State handle_start_state(StateMachine* sm, Event event);
State handle_reading_state(StateMachine* sm, Event event);
State handle_processing_state(StateMachine* sm, Event event);
State handle_finished_state(StateMachine* sm, Event event);
State handle_error_state(StateMachine* sm, Event event);

void initialize_machine(StateMachine* sm) {
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

Event get_next_event(const char* input, size_t* pos) {
    if (input == NULL || pos == NULL) return EVENT_ERROR;
    
    if (input[*pos] == '\0') return EVENT_COMPLETE;
    
    if (input[*pos] == '!') {
        (*pos)++;
        return EVENT_ERROR;
    }
    
    if (input[*pos] == '#') {
        (*pos)++;
        return EVENT_RESET;
    }
    
    if (is_valid_input_char(input[*pos])) {
        (*pos)++;
        return EVENT_DATA;
    }
    
    return EVENT_ERROR;
}

State handle_start_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_len++] = 'X';
                sm->data_count++;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
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

void print_state_info(const StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    printf("State: %s, Data Count: %d\n", 
           state_names[sm->current_state], sm->data_count);
}

int main() {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input[MAX_INPUT_LEN * 2];
    printf("Enter input sequence (end with newline): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);