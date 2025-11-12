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
    int error_code;
} StateMachine;

State handle_idle_state(StateMachine *sm, Event event, const char *data);
State handle_reading_state(StateMachine *sm, Event event, const char *data);
State handle_processing_state(StateMachine *sm, Event event, const char *data);
State handle_writing_state(StateMachine *sm, Event event, const char *data);
State handle_error_state(StateMachine *sm, Event event, const char *data);
State handle_done_state(StateMachine *sm, Event event, const char *data);

void initialize_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->error_code = 0;
}

State handle_idle_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
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
            if (data != NULL) {
                size_t data_len = strlen(data);
                if (data_len < MAX_INPUT_LEN) {
                    strncpy(sm->buffer, data, MAX_INPUT_LEN - 1);
                    sm->buffer[MAX_INPUT_LEN - 1] = '\0';
                    sm->buffer_len = data_len;
                    printf("Read %zu characters\n", data_len);
                    return STATE_PROCESSING;
                } else {
                    sm->error_code = 1;
                    return STATE_ERROR;
                }
            }
            return STATE_READING;
        case EVENT_ERROR:
            sm->error_code = 2;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
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
                printf("Processed: %s\n", sm->buffer);
                return STATE_WRITING;
            }
            return STATE_PROCESSING;
        case EVENT_ERROR:
            sm->error_code = 3;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_WRITE:
            printf("Output: %s\n", sm->buffer);
            return STATE_DONE;
        case EVENT_ERROR:
            sm->error_code = 4;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            printf("Error code: %d\n", sm->error_code);
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event, const char *data) {
    return STATE_DONE;
}

Event get_user_input(char *input_buffer, size_t buffer_size) {
    printf("Enter command (start, data <text>, process, write, error, reset, exit): ");
    
    if (fgets(input_buffer, buffer_size, stdin) == NULL) {
        return EVENT_EXIT;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (strncmp(input_buffer, "start", 5) == 0) {
        return EVENT_START;
    } else if (strncmp(input_buffer, "data ", 5) == 0) {
        return EVENT_DATA;
    } else if (strncmp(input_buffer, "process", 7) == 0) {
        return EVENT_PROCESS;
    } else if (strncmp(input_buffer, "write", 5) == 0) {
        return EVENT_WRITE;
    } else if (strncmp(input_buffer, "error", 5