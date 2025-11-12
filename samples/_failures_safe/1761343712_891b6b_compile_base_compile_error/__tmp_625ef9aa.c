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
    EVENT_RESET,
    EVENT_EXIT
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_idle_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            printf("Invalid event for IDLE state.\n");
            return STATE_IDLE;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, strlen(data))) {
                size_t data_len = strlen(data);
                if (data_len < sizeof(sm->buffer) - sm->buffer_len) {
                    memcpy(sm->buffer + sm->buffer_len, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    printf("Data received: %s\n", data);
                    return STATE_READING;
                } else {
                    printf("Buffer full.\n");
                    return STATE_ERROR;
                }
            } else {
                printf("Invalid data.\n");
                return STATE_ERROR;
            }
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("Processing %d data items.\n", sm->data_count);
                return STATE_PROCESSING;
            } else {
                printf("No data to process.\n");
                return STATE_READING;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state.\n");
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_WRITE:
            printf("Writing processed data: %s\n", sm->buffer);
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state.\n");
            return STATE_PROCESSING;
    }
}

State handle_writing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            printf("Data written successfully.\n");
            initialize_machine(sm);
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state.\n");
            return STATE_WRITING;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            initialize_machine(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            printf("Invalid event for ERROR state.\n");
            return STATE_ERROR;
    }
}

Event get_user_event(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter event (start, data, process, write, error, reset, exit): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_EXIT;
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "write") == 0) return EVENT_WRITE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "exit") == 0) return EVENT_EXIT;
    
    printf("Unknown event.\n");