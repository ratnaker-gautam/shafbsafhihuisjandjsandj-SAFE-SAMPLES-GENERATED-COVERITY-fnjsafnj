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
                if (data_len < sizeof(sm->buffer) - sm->buffer_len) {
                    memcpy(sm->buffer + sm->buffer_len, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_WRITE:
            if (sm->buffer_len > 0) {
                return STATE_WRITING;
            }
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            if (sm->data_count > 0) {
                sm->buffer_len = 0;
                sm->data_count = 0;
                return STATE_READING;
            }
            return STATE_IDLE;
        case EVENT_ERROR:
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
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State transition_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, event, data);
        case STATE_READING:
            return handle_reading_state(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event, data);
        case STATE_WRITING:
            return handle_writing_state(sm, event, data);
        case STATE_ERROR:
            return handle_error_state(sm, event, data);
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

void print_state_info(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    if (state >= STATE_IDLE && state <= STATE_DONE) {
        printf("Current state: %s\n", state_names[state]);
    }
}

Event get_user_event() {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_EXIT;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        return EVENT_ERROR;
    }
    
    if (choice >= EVENT_START && choice <= EVENT_EXIT) {
        return (Event)choice;
    }
    return EVENT_ERROR;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");