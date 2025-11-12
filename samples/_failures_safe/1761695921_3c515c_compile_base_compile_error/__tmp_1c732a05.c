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
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            if (sm->buffer_len > 0) {
                sm->buffer[sm->buffer_len] = '\0';
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
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

Event get_next_event(char input_char) {
    if (input_char == '\n') return EVENT_COMPLETE;
    if (input_char == '!') return EVENT_ERROR;
    if (input_char == 'R') return EVENT_RESET;
    if (input_char == 'S') return EVENT_START;
    if (is_valid_input_char(input_char)) return EVENT_DATA;
    return EVENT_ERROR;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, R=Reset, !=Error, Enter=Complete\n");
    printf("Enter text to process (max %d chars):\n", MAX_INPUT_LEN - 1);
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == 'Q' || input_char == 'q') {
            break;
        }
        
        Event event = get_next_event((char)input_char);
        State new_state = state_machine_transition(&sm, event, (char)input_char);
        
        if (new_state != sm.current_state) {
            sm.current_state = new_state;
            
            switch (sm.current_state) {
                case STATE_ERROR:
                    printf("State: ERROR - Type 'R' to reset\n");
                    break;
                case STATE_FINISHED:
                    printf("State: FINISHED - Type 'R' to reset or 'S' to start new\n