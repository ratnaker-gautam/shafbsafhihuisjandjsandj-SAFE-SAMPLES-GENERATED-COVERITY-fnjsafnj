//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_WRITE,
    EVENT_ERROR,
    EVENT_COMPLETE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_processed;
} StateMachine;

State handle_idle(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_processed = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

State handle_idle(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DATA:
            return STATE_PROCESSING;
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
            sm->data_processed = 1;
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_WRITE:
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle(sm, event);
        case STATE_READING:
            return handle_reading(sm, event);
        case STATE_PROCESSING:
            return handle_processing(sm, event);
        case STATE_WRITING:
            return handle_writing(sm, event);
        case STATE_ERROR:
            return handle_error(sm, event);
        case STATE_DONE:
            return handle_done(sm, event);
        default:
            return STATE_ERROR;
    }
}

void print_state(State state) {
    switch (state) {
        case STATE_IDLE:
            printf("IDLE");
            break;
        case STATE_READING:
            printf("READING");
            break;
        case STATE_PROCESSING:
            printf("PROCESSING");
            break;
        case STATE_WRITING:
            printf("WRITING");
            break;
        case STATE_ERROR:
            printf("ERROR");
            break;
        case STATE_DONE:
            printf("DONE");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, write, error, complete, reset, quit\n");
    
    while (running) {
        printf("Current state: ");
        print_state(sm.current_state);
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp