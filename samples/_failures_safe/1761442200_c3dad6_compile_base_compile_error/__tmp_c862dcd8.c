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
    EVENT_ERROR,
    EVENT_RESET
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

State transition_state(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start(sm, event);
        case STATE_READING:
            return handle_reading(sm, event);
        case STATE_PROCESSING:
            return handle_processing(sm, event);
        case STATE_FINISHED:
            return handle_finished(sm, event);
        case STATE_ERROR:
            return handle_error(sm, event);
        default:
            return STATE_ERROR;
    }
}

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
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
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < MAX_STATES) {
                sm->data_count++;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_PROCESS:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

void print_state_info(State state) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int get_user_input(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter event (0=START, 1=DATA, 2=PROCESS, 3=COMPLETE, 4=ERROR, 5=RESET, 6=QUIT): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input) {
        return -1;
    }
    
    if (*endptr != '\n' && *endptr != '\0') {
        return -1;
    }
    
    if (value < 0 || value > 6) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.buffer_len = 0;
    sm.data_count = 0;
    memset(sm.buffer, 0, sizeof(sm.buffer));
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        print_state_info(sm.current_state);
        printf("Data count: %d\n", sm.data_count);
        
        int input = get_user_input();
        if (input == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (input == 6) {
            break;
        }
        
        Event event = (Event)input;
        State new_state = transition_state(&sm, event);
        
        if (new_state != sm.current_state) {
            printf("State