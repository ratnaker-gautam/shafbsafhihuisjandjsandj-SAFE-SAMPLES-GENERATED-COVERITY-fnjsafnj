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

State (*state_handlers[])(StateMachine*, Event) = {
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
            printf("State machine started\n");
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
                printf("Processing data chunk %d\n", sm->data_count + 1);
                sm->data_count++;
                if (sm->data_count >= 3) {
                    return STATE_PROCESSING;
                }
                return STATE_READING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing collected data...\n");
            for (int i = 0; i < sm->data_count; i++) {
                printf("Data item %d processed\n", i + 1);
            }
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("State machine completed successfully\n");
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("State machine entered error state\n");
    return STATE_ERROR;
}

void state_machine_step(StateMachine *sm, Event event) {
    if (sm == NULL || sm->current_state >= MAX_STATES) return;
    
    State new_state = state_handlers[sm->current_state](sm, event);
    if (new_state < MAX_STATES) {
        sm->current_state = new_state;
    } else {
        sm->current_state = STATE_ERROR;
    }
}

int get_user_input(void) {
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return -1;
    }
    
    if (strcmp(input, "start") == 0) return 0;
    if (strcmp(input, "data") == 0) return 1;
    if (strcmp(input, "process") == 0) return 2;
    if (strcmp(input, "complete") == 0) return 3;
    if (strcmp(input, "error") == 0) return 4;
    
    return -1;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.buffer_len = 0;
    sm.data_count = 0;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data, process, complete, error\n");
    printf("Enter commands to drive the state machine:\n");
    
    while (sm.current_state != STATE_FINISHED && sm.current_state != STATE_ERROR) {
        printf("Current state: %d > ", sm.current_state);
        
        int input = get_user_input();
        if (input < 0) {
            printf("Invalid input\n");
            continue;
        }
        
        Event event = (Event)input;
        state_machine_step(&sm, event);
    }
    
    if (sm.current_state == STATE_FINISHED) {
        printf("State machine terminated successfully\n");
        return EXIT_SUCCESS;
    } else {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
}