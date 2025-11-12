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
    machine->buffer_pos = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_start_state(StateMachine *machine, Event event, char input) {
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, char input) {
    switch (event) {
        case EVENT_DATA:
            if (machine->buffer_pos < MAX_INPUT_LEN - 1 && is_valid_input_char(input)) {
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
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *machine, Event event, char input) {
    switch (event) {
        case EVENT_COMPLETE:
            machine->data_count++;
            printf("Processed: %s (count: %d)\n", machine->buffer, machine->data_count);
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *machine, Event event, char input) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *machine, Event event, char input) {
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

Event get_event_from_input(char input) {
    if (input == '\n') return EVENT_COMPLETE;
    if (input == 'q') return EVENT_RESET;
    if (is_valid_input_char(input)) return EVENT_DATA;
    return EVENT_ERROR;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo - Enter text (press Enter to process, 'q' to reset, Ctrl+D to exit)\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == EOF) break;
        
        Event event = get_event_from_input((char)input_char);
        State new_state = process_event(&machine, event, (char)input_char);
        
        if (new_state == STATE_ERROR && machine.current_state != STATE_ERROR) {
            printf("Error: Invalid transition\n");
        }
        
        machine.current_state = new_state;
        
        if (machine.current_state == STATE_FINISHED) {
            printf("Operation completed successfully. Press 'q' to reset or continue.\n");
        }
    }
    
    printf("\nFinal state: ");
    switch (machine.current_state) {
        case STATE_START: printf("START"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_FINISHED: printf("FINISHED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\nTotal processed items: %d\n", machine.data_count);
    
    return 0;
}