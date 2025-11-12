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
    EVENT_END,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    int data_count;
    char buffer[MAX_INPUT_LEN];
} StateMachine;

State get_next_state(State current, Event event);
Event get_event_from_input(const char* input);
void process_state_machine(StateMachine* sm, const char* input);
void initialize_state_machine(StateMachine* sm);
void handle_state_action(StateMachine* sm, Event event);

State get_next_state(State current, Event event) {
    switch(current) {
        case STATE_START:
            if (event == EVENT_START) return STATE_READING;
            if (event == EVENT_INVALID) return STATE_ERROR;
            break;
        case STATE_READING:
            if (event == EVENT_DATA) return STATE_PROCESSING;
            if (event == EVENT_END) return STATE_FINISHED;
            if (event == EVENT_INVALID) return STATE_ERROR;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_DATA) return STATE_READING;
            if (event == EVENT_END) return STATE_FINISHED;
            if (event == EVENT_INVALID) return STATE_ERROR;
            break;
        case STATE_FINISHED:
        case STATE_ERROR:
            break;
    }
    return current;
}

Event get_event_from_input(const char* input) {
    if (input == NULL) return EVENT_INVALID;
    
    size_t len = strlen(input);
    if (len == 0) return EVENT_INVALID;
    
    if (strcmp(input, "START") == 0) return EVENT_START;
    if (strcmp(input, "END") == 0) return EVENT_END;
    if (strcmp(input, "DATA") == 0) return EVENT_DATA;
    
    return EVENT_INVALID;
}

void handle_state_action(StateMachine* sm, Event event) {
    switch(sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                printf("State machine started.\n");
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        case STATE_READING:
            if (event == EVENT_DATA) {
                printf("Reading data...\n");
                sm->data_count++;
            } else if (event == EVENT_END) {
                printf("Finished reading. Total data items: %d\n", sm->data_count);
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                printf("Processing data...\n");
            } else if (event == EVENT_END) {
                printf("Finished processing. Total data items: %d\n", sm->data_count);
            }
            break;
        case STATE_FINISHED:
            printf("State machine finished successfully.\n");
            break;
        case STATE_ERROR:
            printf("State machine encountered an error.\n");
            break;
    }
}

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void process_state_machine(StateMachine* sm, const char* input) {
    if (sm == NULL || input == NULL) return;
    
    Event event = get_event_from_input(input);
    State next_state = get_next_state(sm->current_state, event);
    
    if (next_state != sm->current_state) {
        sm->current_state = next_state;
        handle_state_action(sm, event);
    }
}

int main() {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_state_machine(&sm);
    
    printf("State Machine Controller\n");
    printf("Available commands: START, DATA, END\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) >= MAX_INPUT_LEN - 1) {
            printf("Input too long. Maximum length is %d characters.\n", MAX_INPUT_LEN - 1);
            continue;
        }
        
        process_state_machine(&sm, input);
        
        if (sm.current_state == STATE_FINISHED) {
            printf("Resetting state machine...\n");
            initialize_state_machine(&sm);
        }
    }
    
    printf("Exiting state machine controller.\n");
    return 0;
}