//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
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
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_index;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    machine->buffer_index = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

Event get_next_event(void) {
    char input[MAX_INPUT_LENGTH];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

State handle_start_state(Event event, StateMachine *machine) {
    switch (event) {
        case EVENT_START:
            printf("Machine started\n");
            return STATE_READING;
        default:
            printf("Invalid event for START state\n");
            return STATE_ERROR;
    }
}

State handle_reading_state(Event event, StateMachine *machine) {
    switch (event) {
        case EVENT_DATA:
            printf("Enter data: ");
            char data[MAX_INPUT_LENGTH];
            if (fgets(data, sizeof(data), stdin) != NULL) {
                size_t len = strlen(data);
                if (len > 0 && data[len-1] == '\n') {
                    data[len-1] = '\0';
                }
                
                if (machine->buffer_index + len < MAX_INPUT_LENGTH) {
                    for (size_t i = 0; i < len; i++) {
                        if (is_valid_input(data[i])) {
                            machine->buffer[machine->buffer_index++] = data[i];
                        }
                    }
                    machine->data_count++;
                    printf("Data stored. Total items: %d\n", machine->data_count);
                } else {
                    printf("Buffer full\n");
                    return STATE_ERROR;
                }
            }
            return STATE_READING;
        case EVENT_PROCESS:
            if (machine->data_count > 0) {
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READING;
            }
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

State handle_processing_state(Event event, StateMachine *machine) {
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Data: %s\n", machine->buffer);
            return STATE_FINISHED;
        case EVENT_ERROR:
            printf("Processing error\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

State handle_finished_state(Event event, StateMachine *machine) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            printf("Invalid event for FINISHED state\n");
            return STATE_ERROR;
    }
}

State handle_error_state(Event event, StateMachine *machine) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

State process_event(Event event, StateMachine *machine) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            return handle_start_state(event, machine);
        case STATE_READING:
            return handle_reading_state(event, machine);
        case STATE_PROCESSING:
            return handle_processing_state(event, machine);
        case STATE_FINISHED:
            return handle_finished_state(event, machine);
        case STATE_ERROR:
            return handle_error_state(event, machine);
        default:
            return STATE_ERROR;
    }
}

void print_instructions(void) {
    printf("Available commands: start,