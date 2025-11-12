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
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_len = 0;
    machine->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_idle_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_reading_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_DATA:
            if (!validate_input(data, data_len)) {
                printf("Invalid input data\n");
                return STATE_ERROR;
            }
            if (machine->buffer_len + data_len < MAX_INPUT_LEN) {
                memcpy(machine->buffer + machine->buffer_len, data, data_len);
                machine->buffer_len += data_len;
                machine->data_count++;
                printf("Data received: %.*s\n", (int)data_len, data);
                return STATE_READING;
            } else {
                printf("Buffer overflow\n");
                return STATE_ERROR;
            }
        case EVENT_PROCESS:
            if (machine->data_count > 0) {
                printf("Processing %d data items\n", machine->data_count);
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READING;
            }
        case EVENT_RESET:
            printf("Reset to idle\n");
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Processing completed successfully\n");
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            printf("Processing failed\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset to idle\n");
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_complete_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_RESET:
            printf("Reset to idle\n");
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

State handle_error_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_RESET:
            printf("Reset to idle\n");
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

void process_event(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return;
    
    State new_state = machine->current_state;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(machine, event, data, data_len);
            break;
        case STATE_READING:
            new_state = handle_reading_state(machine, event, data, data_len);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(machine, event, data, data_len);
            break;
        case STATE_COMPLETE:
            new_state = handle_complete_state(machine, event, data, data_len);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(machine, event, data, data_len);
            break;
        default:
            printf("Unknown state\n");
            return;
    }
    
    machine->current_state = new_state;
}

Event get_event_from_input(const char *input) {
    if (input == NULL) return EVENT_FAILURE;
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process")