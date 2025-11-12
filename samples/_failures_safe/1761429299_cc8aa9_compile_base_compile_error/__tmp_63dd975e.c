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
    memset(machine->buffer, 0, MAX_INPUT_LENGTH);
    machine->buffer_index = 0;
    machine->data_count = 0;
}

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_start_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (machine->buffer_index < MAX_INPUT_LENGTH - 1 && is_valid_input_char(data)) {
                machine->buffer[machine->buffer_index++] = data;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            if (machine->buffer_index > 0) {
                machine->buffer[machine->buffer_index] = '\0';
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            machine->data_count++;
            printf("Processed: %s (count: %d)\n", machine->buffer, machine->data_count);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *machine, Event event, char data) {
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

Event get_event_from_input(char input) {
    switch (input) {
        case 's': return EVENT_START;
        case 'd': return EVENT_DATA;
        case 'c': return EVENT_COMPLETE;
        case 'e': return EVENT_ERROR;
        case 'r': return EVENT_RESET;
        default: return EVENT_ERROR;
    }
}

void print_state_name(State state) {
    switch (state) {
        case STATE_START: printf("START"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_FINISHED: printf("FINISHED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Controller\n");
    printf("Commands: s=start, d=data, c=complete, e=error, r=reset, q=quit\n");
    printf("Enter command followed by data character if needed: ");
    
    char input[10];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (input[0] == 'q') {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        Event event = get_event_from_input(input[0]);