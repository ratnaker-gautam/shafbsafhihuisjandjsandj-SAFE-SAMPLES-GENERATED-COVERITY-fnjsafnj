//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_idle(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("State: READING -> PROCESSING\n");
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            printf("State: READING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                sm->data_processed = 1;
                printf("State: PROCESSING -> WRITING\n");
                return STATE_WRITING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_WRITE:
            if (sm->data_processed) {
                printf("State: WRITING -> DONE\n");
                return STATE_DONE;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            printf("State: WRITING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            printf("State: ERROR -> IDLE\n");
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
            printf("State: DONE -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

void process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return;
    State (*handlers[])(StateMachine*, Event) = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done
    };
    if (sm->current_state < STATE_IDLE || sm->current_state > STATE_DONE) {
        sm->current_state = STATE_ERROR;
        return;
    }
    State new_state = handlers[sm->current_state](sm, event);
    if (new_state >= STATE_IDLE && new_state <= STATE_DONE) {
        sm->current_state = new_state;
    } else {
        sm->current_state = STATE_ERROR;
    }
}

int main(void) {
    StateMachine