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
    STATE_DONE,
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
    size_t buffer_pos;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_done_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->data_count = 0;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_pos] = 'A';
                sm->buffer_pos++;
                sm->data_count++;
                printf("Data received, count: %d\n", sm->data_count);
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            if (sm->data_count > 0) {
                printf("Reading complete, processing data\n");
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Data: %s\n", sm->buffer);
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_DONE:
            return handle_done_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

Event get_user_event(void) {
    char input[10];
    printf("Enter event (0=START, 1=DATA, 2=COMPLETE, 3=ERROR, 4=RESET, 5=QUIT): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        return EVENT_ERROR;
    }
    
    if (choice < 0 || choice > 5) {
        return EVENT_ERROR;
    }
    
    if (choice == 5) {
        return EVENT_ERROR;
    }
    
    return (Event)choice;
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=COMPLETE, 3=ERROR, 4=RESET, 5=QUIT\n");
    
    while (1) {
        Event event = get_user_event();
        
        if (event == EVENT_ERROR) {
            printf("Exiting state machine\n");
            break;
        }
        
        State new_state = process_event(&sm, event);
        
        if (new_state == STATE_ERROR &&