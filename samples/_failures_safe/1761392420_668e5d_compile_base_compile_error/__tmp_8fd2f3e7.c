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
    EVENT_DONE,
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

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start(StateMachine *sm, Event event) {
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

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
                if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                    size_t len = strlen(sm->buffer);
                    if (len > 0 && sm->buffer[len - 1] == '\n') {
                        sm->buffer[len - 1] = '\0';
                        len--;
                    }
                    if (validate_input(sm->buffer, len)) {
                        sm->buffer_len = len;
                        sm->data_count++;
                        return STATE_PROCESSING;
                    }
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing: '");
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    putchar(sm->buffer[i] - 32);
                } else {
                    putchar(sm->buffer[i]);
                }
            }
            printf("' (count: %d)\n", sm->data_count);
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DONE:
            printf("Processing complete. Total items: %d\n", sm->data_count);
            return STATE_START;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            printf("Error state - reset required\n");
            return STATE_ERROR;
    }
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
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

Event get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("1 - START, 2 - DATA, 3 - PROCESS, 4 - DONE, 5 - RESET, 0 - EXIT\n");
    printf("Choose event: ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) ==