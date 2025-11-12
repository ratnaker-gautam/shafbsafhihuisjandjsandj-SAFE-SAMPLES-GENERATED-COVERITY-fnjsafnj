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
    EVENT_COMPLETE,
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

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_len = 0;
    machine->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (!(input[i] >= 'a' && input[i] <= 'z') &&
            !(input[i] >= 'A' && input[i] <= 'Z') &&
            !(input[i] >= '0' && input[i] <= '9') &&
            input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

State handle_start_state(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, strlen(data))) {
                size_t data_len = strlen(data);
                if (machine->buffer_len + data_len < MAX_INPUT_LEN) {
                    strncpy(machine->buffer + machine->buffer_len, data, data_len);
                    machine->buffer_len += data_len;
                    machine->data_count++;
                    printf("Data received: %s (Total: %zu chars)\n", data, machine->buffer_len);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Total data items: %d\n", machine->data_count);
            printf("Final buffer: %s\n", machine->buffer);
            return STATE_FINISHED;
        case EVENT_DATA:
            return handle_reading_state(machine, event, data);
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            printf("State machine reset\n");
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            printf("State machine reset after error\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            return handle_start_state(machine, event, data);
        case STATE_READING:
            return handle_reading_state(machine, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(machine, event, data);
        case STATE_FINISHED:
            return handle_finished_state(machine, event, data);
        case STATE_ERROR:
            return handle_error_state(machine, event, data);
        default:
            return STATE_ERROR;
    }
}

Event get_user_event(void) {
    char input[MAX_INPUT_LEN];
    
    printf("\nAvailable events:\n");
    printf("1 - START\n2 - DATA\n3 - COMPLETE\n4 - ERROR\n5 - RESET\n6 - EXIT\n");
    printf("Choose event (1-6): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || choice < 1 || choice > 6) {
        return EVENT_ERROR;
    }
    
    switch (choice) {
        case 1: return EVENT_START;
        case 2: return