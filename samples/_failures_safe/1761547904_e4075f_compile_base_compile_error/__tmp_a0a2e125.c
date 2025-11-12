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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State get_next_state(State current, Event event);
void handle_state_action(StateMachine *sm, Event event);
Event get_event_from_input(const char *input);
int validate_input(const char *input);
void initialize_state_machine(StateMachine *sm);
void cleanup_state_machine(StateMachine *sm);

State get_next_state(State current, Event event) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_START) return STATE_READING;
            break;
        case STATE_READING:
            if (event == EVENT_DATA) return STATE_PROCESSING;
            if (event == EVENT_END) return STATE_FINISHED;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_DATA) return STATE_READING;
            if (event == EVENT_END) return STATE_FINISHED;
            break;
        case STATE_FINISHED:
            return STATE_FINISHED;
        case STATE_ERROR:
            return STATE_ERROR;
        default:
            break;
    }
    return STATE_ERROR;
}

void handle_state_action(StateMachine *sm, Event event) {
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                printf("State machine started.\n");
                sm->buffer_len = 0;
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        case STATE_READING:
            if (event == EVENT_DATA) {
                printf("Processing data: %s\n", sm->buffer);
                sm->data_count++;
                sm->buffer_len = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            } else if (event == EVENT_END) {
                printf("Finished with %d data items processed.\n", sm->data_count);
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                printf("Data processed, ready for next.\n");
            } else if (event == EVENT_END) {
                printf("Finished with %d data items processed.\n", sm->data_count);
            }
            break;
        case STATE_FINISHED:
            printf("State machine finished.\n");
            break;
        case STATE_ERROR:
            printf("State machine error occurred.\n");
            break;
        default:
            break;
    }
}

Event get_event_from_input(const char *input) {
    if (input == NULL) return EVENT_INVALID;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "end") == 0) return EVENT_END;
    
    return EVENT_INVALID;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    
    return 1;
}

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    sm->buffer_len = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void cleanup_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    sm->buffer_len = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_state_machine(&sm);
    
    printf("State Machine Controller\n");
    printf("Available commands: start, data, end\n");
    printf("Enter 'quit' to exit.\n");
    
    while (1) {
        printf("Current state: %d > ", sm.current_state);
        
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
        
        if (!validate_input(input)) {
            printf("Invalid input format.\n");
            continue;
        }
        
        Event event = get_event_from_input(input);
        if (event == EVENT_INVALID) {
            printf("Unknown command.\n");
            continue;
        }
        
        if (event == EVENT