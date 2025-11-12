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
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            if (machine->buffer_pos > 0) {
                machine->buffer[machine->buffer_pos] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return machine->current_state;
    }
}

State handle_processing_state(StateMachine *machine, Event event, char input) {
    switch (event) {
        case EVENT_COMPLETE:
            machine->data_count++;
            printf("Processed: %s (count: %d)\n", machine->buffer, machine->data_count);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return machine->current_state;
    }
}

State handle_finished_state(StateMachine *machine, Event event, char input) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return machine->current_state;
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

Event get_next_event(char input, int *end_of_input) {
    if (end_of_input == NULL) return EVENT_ERROR;
    
    if (input == EOF || input == '\0') {
        *end_of_input = 1;
        return EVENT_COMPLETE;
    }
    
    if (input == '!') {
        return EVENT_ERROR;
    }
    
    if (input == '#') {
        return EVENT_RESET;
    }
    
    return EVENT_DATA;
}

void print_state_info(State state) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("State: %s\n", state_names[state]);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Enter text (max %d chars). Use '!' for error, '#' for reset, Ctrl+D to finish:\n", MAX_INPUT_LEN - 1);
    
    int end_of_input = 0;
    int input_char;
    
    while (!end_of_input && machine.current_state != STATE_ERROR) {
        input_char = getchar();
        
        if (input_char == EOF && machine.buffer_pos == 0) {
            break;
        }
        
        Event event = get_next_event(input_char, &end_of_input);
        State new_state = process_event(&machine, event, (char)input_char);
        
        if (new_state != machine.current_state) {
            machine.current_state = new_state;
            print_state_info(machine.current_state);
        }
        
        if (machine.current_state == STATE_FINISHED || machine.current_state == STATE_ERROR) {
            if (machine.current_state == STATE_ERROR) {
                printf("