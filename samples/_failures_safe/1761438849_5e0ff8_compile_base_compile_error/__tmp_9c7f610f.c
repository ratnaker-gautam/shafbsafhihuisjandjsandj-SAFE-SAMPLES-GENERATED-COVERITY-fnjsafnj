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

State handle_start(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_finished(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine *, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            sm->buffer_len = 0;
            sm->data_count = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < 3) {
                sm->data_count++;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                return STATE_FINISHED;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_FINISHED;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            sm->buffer_len = 0;
            sm->data_count = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    sm->buffer_len = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return -1;
    if (sm->current_state >= MAX_STATES) return -1;
    
    State new_state = state_handlers[sm->current_state](sm, event);
    if (new_state >= MAX_STATES) return -1;
    
    sm->current_state = new_state;
    return 0;
}

void print_state_info(StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    printf("Current state: %s, Data count: %d\n", 
           state_names[sm->current_state], sm->data_count);
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: s=start, d=data, p=process, c=complete, e=error, q=quit\n");
    
    char input[10];
    int running = 1;
    
    while (running) {
        print_state_info(&sm);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = 0;
        }
        
        Event event;
        switch (input[0]) {
            case 's': event = EVENT_START; break;
            case 'd': event = EVENT_DATA; break;
            case 'p': event = EVENT_PROCESS; break;
            case 'c': event = EVENT_COMPLETE; break;
            case 'e': event = EVENT_ERROR; break;
            case 'q': running = 0; continue;
            default:
                printf("Invalid command\n");
                continue;
        }
        
        if (process_event(&sm, event) != 0) {
            printf("Error