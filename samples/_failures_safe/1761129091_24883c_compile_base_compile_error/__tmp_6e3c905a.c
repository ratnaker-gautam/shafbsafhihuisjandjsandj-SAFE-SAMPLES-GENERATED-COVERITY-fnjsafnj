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
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_pos = 0;
    machine->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(StateMachine *machine, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, const char *data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, strlen(data))) {
                size_t data_len = strlen(data);
                if (machine->buffer_pos + data_len < MAX_INPUT_LEN - 1) {
                    strncpy(machine->buffer + machine->buffer_pos, data, data_len);
                    machine->buffer_pos += data_len;
                    machine->data_count++;
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
    switch (event) {
        case EVENT_DATA:
            return handle_reading_state(machine, event, data);
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *machine, Event event, const char *data) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *machine, Event event, const char *data) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
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

void print_state_info(const StateMachine *machine) {
    if (machine == NULL) return;
    
    printf("Current state: ");
    switch (machine->current_state) {
        case STATE_START: printf("START"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_FINISHED: printf("FINISHED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\nBuffer: %s\n", machine->buffer);
    printf("Data count: %d\n", machine->data_count);
    printf("---\n");
}

int main() {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data <text>, complete, reset, quit\n");
    
    char input[MAX_INPUT_LEN];
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) continue;
        
        Event event = EVENT_ERROR;
        char *data = NULL;
        
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "complete") == 0) {
            event = EVENT_COMPLETE;
        } else if (strcmp(input, "reset") == 0) {
            event = EVENT_RESET;