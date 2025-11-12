//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 8

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
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, MAX_INPUT_LEN)) {
                size_t data_len = strlen(data);
                if (sm->buffer_len + data_len < MAX_INPUT_LEN) {
                    strncpy(sm->buffer + sm->buffer_len, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    printf("Data received: %s (total: %zu)\n", data, sm->buffer_len);
                    return STATE_READING;
                } else {
                    printf("Buffer full\n");
                    return STATE_ERROR;
                }
            } else {
                printf("Invalid data\n");
                return STATE_ERROR;
            }
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("State: READING -> PROCESSING\n");
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE:
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Processing error occurred\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Writing completed. Output: %s\n", sm->buffer);
            printf("State: WRITING -> DONE\n");
            return STATE_DONE;
        case EVENT_ERROR:
            printf("Writing error occurred\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from ERROR state\n");
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Must reset from ERROR state\n");
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from D