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

State handle_idle(StateMachine *sm, Event event, const char *data);
State handle_reading(StateMachine *sm, Event event, const char *data);
State handle_processing(StateMachine *sm, Event event, const char *data);
State handle_writing(StateMachine *sm, Event event, const char *data);
State handle_error(StateMachine *sm, Event event, const char *data);
State handle_done(StateMachine *sm, Event event, const char *data);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_idle(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_START:
            initialize_machine(sm);
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DATA:
            if (data == NULL || !validate_input(data, MAX_INPUT_LEN)) {
                return STATE_ERROR;
            }
            if (sm->buffer_len + strlen(data) >= MAX_INPUT_LEN) {
                return STATE_ERROR;
            }
            strcpy(sm->buffer + sm->buffer_len, data);
            sm->buffer_len += strlen(data);
            sm->data_count++;
            printf("Data received: %s (total: %zu)\n", data, sm->buffer_len);
            if (sm->data_count >= 3) {
                printf("State: READING -> PROCESSING\n");
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len == 0) return STATE_ERROR;
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            printf("Processing complete. Result: %s\n", sm->buffer);
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_WRITE:
            printf("Final output: %s\n", sm->buffer);
            printf("State: WRITING -> DONE\n");
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            printf("State: ERROR -> IDLE (reset)\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            printf("State: DONE -> IDLE (reset)\n");
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

State process_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    State next_state = STATE_ERROR;
    switch (sm->current_state) {
        case STATE_IDLE:
            next_state = handle_idle