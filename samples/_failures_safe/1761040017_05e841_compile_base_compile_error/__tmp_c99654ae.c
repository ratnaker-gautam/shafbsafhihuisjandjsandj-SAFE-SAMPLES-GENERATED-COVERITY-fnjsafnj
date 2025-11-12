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
void handle_state_action(StateMachine* sm, Event event);
int is_valid_input(const char* input);
void initialize_state_machine(StateMachine* sm);

State get_next_state(State current, Event event) {
    switch(current) {
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
        case STATE_ERROR:
            break;
    }
    return STATE_ERROR;
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
    if (sm == NULL) return;
    
    switch(sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                printf("State machine started.\n");
            }
            break;
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_count < MAX_STATES) {
                    sm->data_count++;
                    printf("Data processed. Count: %d\n", sm->data_count);
                }
            } else if (event == EVENT_END) {
                printf("Processing finished. Total data: %d\n", sm->data_count);
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                printf("Returning to reading state.\n");
            } else if (event == EVENT_END) {
                printf("Processing finished. Total data: %d\n", sm->data_count);
            }
            break;
        case STATE_FINISHED:
            printf("State machine completed successfully.\n");
            break;
        case STATE_ERROR:
            printf("State machine encountered an error.\n");
            break;
    }
}

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    
    return 1;
}

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main() {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_state_machine(&sm);
    
    printf("State Machine Controller\n");
    printf("Valid commands: START, DATA, END\n");
    printf("Enter 'quit' to exit.\n\n");
    
    while (1) {
        printf("Current state: ");
        switch(sm.current_state) {
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_FINISHED: printf("FINISHED"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        
        if (!is_valid_input(input)) {
            printf("Invalid input format.\n");
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        Event event = get_event_from_input(input);
        if (event == EVENT_INVALID) {
            printf("Invalid command. Use: START, DATA, END\n");
            continue;
        }
        
        State next_state = get_next_state(sm.current_state, event);