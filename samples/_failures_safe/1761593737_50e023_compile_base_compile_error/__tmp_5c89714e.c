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
    int data_count;
} StateMachine;

State handle_idle(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

State handle_idle(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_START:
            printf("Starting data processing\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Already in idle state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for idle state\n");
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                sm->buffer_len = strlen(sm->buffer);
                if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                    sm->buffer[--sm->buffer_len] = '\0';
                }
                if (validate_input(sm->buffer, sm->buffer_len)) {
                    sm->data_count++;
                    return STATE_PROCESSING;
                } else {
                    printf("Invalid input data\n");
                    return STATE_ERROR;
                }
            } else {
                printf("Failed to read input\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("Resetting from reading state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for reading state\n");
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data: ");
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
                putchar(sm->buffer[i]);
            }
            printf("\n");
            return STATE_WRITING;
        case EVENT_RESET:
            printf("Resetting from processing state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for processing state\n");
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_WRITE:
            printf("Data processed successfully. Count: %d\n", sm->data_count);
            if (sm->data_count >= 3) {
                return STATE_DONE;
            } else {
                return STATE_READING;
            }
        case EVENT_RESET:
            printf("Resetting from writing state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for writing state\n");
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_IDLE;
        default:
            printf("Must reset from error state\n");
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            return STATE_IDLE;
        default:
            printf("Processing complete. Use reset to start over\n");
            return STATE_DONE;
    }
}

State transition_state(StateMachine *sm, Event event) {
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle(sm, event);
        case STATE_READING:
            return handle_reading(sm, event);
        case STATE_PROCESSING:
            return handle_processing(sm, event);
        case STATE_WRITING:
            return handle_writing(sm, event);
        case STATE