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

int is_valid_input(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_start_state(StateMachine *machine, Event event, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (machine->buffer_pos < MAX_INPUT_LEN - 1 && is_valid_input(input)) {
                machine->buffer[machine->buffer_pos++] = input;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            if (machine->buffer_pos > 0) {
                machine->buffer[machine->buffer_pos] = '\0';
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *machine, Event event, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            machine->data_count++;
            printf("Processed: %s (count: %d)\n", machine->buffer, machine->data_count);
            initialize_machine(machine);
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *machine, Event event, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *machine, Event event, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *machine, Event event, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            return handle_start_state(machine, event, input);
        case STATE_READING:
            return handle_reading_state(machine, event, input);
        case STATE_PROCESSING:
            return handle_processing_state(machine, event, input);
        case STATE_FINISHED:
            return handle_finished_state(machine, event, input);
        case STATE_ERROR:
            return handle_error_state(machine, event, input);
        default:
            return STATE_ERROR;
    }
}

Event get_next_event(char input) {
    if (input == 'S') return EVENT_START;
    if (input == 'D') return EVENT_DATA;
    if (input == 'C') return EVENT_COMPLETE;
    if (input == 'E') return EVENT_ERROR;
    if (input == 'R') return EVENT_RESET;
    return EVENT_ERROR;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, D=Data, C=Complete, E=Error, R=Reset, Q=Quit\n");
    printf("For DATA events, enter character after 'D' command\n");
    
    char input_line[MAX_INPUT_LEN];
    
    while (1) {
        printf("Current state: %d | Enter command: ", machine.current_state);
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        size_t input_len = strlen(input_line);
        if (input_len == 0 || input_line[0] == '\n') {
            continue;
        }
        
        char command = input_line[0];
        char data_char = '\0';
        
        if (command == 'D' && input_len >= 2) {
            data_char = input