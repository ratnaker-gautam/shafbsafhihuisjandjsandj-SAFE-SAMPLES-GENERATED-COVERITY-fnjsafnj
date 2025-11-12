//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

State handle_idle_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, strlen(data))) {
                size_t data_len = strlen(data);
                if (data_len < MAX_INPUT_LEN) {
                    strncpy(sm->buffer, data, MAX_INPUT_LEN - 1);
                    sm->buffer[MAX_INPUT_LEN - 1] = '\0';
                    sm->buffer_len = data_len;
                    sm->data_count++;
                    printf("Data received: %s\n", sm->buffer);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed data: %s\n", sm->buffer);
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_WRITE:
            printf("Writing completed. Total data processed: %d\n", sm->data_count);
            memset(sm->buffer, 0, sizeof(sm->buffer));
            sm->buffer_len = 0;
            return STATE_READING;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_WRITING;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            printf("Error state. Use reset to continue.\n");
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event, const char *data) {
    return STATE_DONE;
}

Event get_user_input(char *buffer, size_t buf_size) {
    if (buffer == NULL || buf_size == 0) return EVENT_ERROR;
    
    printf("Enter command (start, data, process, write, reset, exit): ");
    if (fgets(buffer, buf_size, stdin) == NULL) return EVENT_ERROR;
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (strcmp(buffer, "start") == 0) return EVENT_START;
    if (strcmp(buffer, "data") == 0) return EVENT_DATA;
    if (strcmp(buffer, "process") == 0) return EVENT_PROCESS;
    if (strcmp(buffer, "write") == 0) return EVENT_WRITE;
    if (strcmp(buffer, "reset") == 0) return EVENT_RESET;
    if (strcmp(buffer, "exit") == 0) return EVENT_EXIT;
    
    return EVENT_ERROR;
}

void process_state_machine(StateMachine