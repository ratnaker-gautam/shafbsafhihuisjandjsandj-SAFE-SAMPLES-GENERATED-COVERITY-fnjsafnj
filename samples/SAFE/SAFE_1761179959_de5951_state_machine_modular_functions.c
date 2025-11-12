//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char input_char;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->buffer_pos = 0;
    machine->processed_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

State get_next_state(State current, char input) {
    switch(current) {
        case STATE_IDLE:
            if (is_valid_input(input) && input != '\n') {
                return STATE_READING;
            }
            break;
        case STATE_READING:
            if (input == '\n') {
                return STATE_PROCESSING;
            } else if (is_valid_input(input)) {
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
            break;
        case STATE_PROCESSING:
            if (current == STATE_PROCESSING) {
                return STATE_COMPLETE;
            }
            break;
        case STATE_COMPLETE:
            return STATE_IDLE;
        case STATE_ERROR:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

void process_buffer(StateMachine *machine) {
    if (machine->buffer_pos == 0) {
        return;
    }
    
    for (size_t i = 0; i < machine->buffer_pos; i++) {
        if (islower(machine->buffer[i])) {
            machine->buffer[i] = toupper(machine->buffer[i]);
        } else if (isupper(machine->buffer[i])) {
            machine->buffer[i] = tolower(machine->buffer[i]);
        }
    }
    machine->processed_count++;
}

void handle_state_action(StateMachine *machine, char input) {
    switch(machine->current_state) {
        case STATE_IDLE:
            if (is_valid_input(input) && input != '\n') {
                if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                    machine->buffer[machine->buffer_pos++] = input;
                }
            }
            break;
        case STATE_READING:
            if (input == '\n') {
                machine->buffer[machine->buffer_pos] = '\0';
            } else if (is_valid_input(input)) {
                if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                    machine->buffer[machine->buffer_pos++] = input;
                }
            }
            break;
        case STATE_PROCESSING:
            process_buffer(machine);
            break;
        case STATE_COMPLETE:
            printf("Processed: %s (Total: %d)\n", machine->buffer, machine->processed_count);
            machine->buffer_pos = 0;
            memset(machine->buffer, 0, sizeof(machine->buffer));
            break;
        case STATE_ERROR:
            printf("Error: Invalid input detected\n");
            machine->buffer_pos = 0;
            memset(machine->buffer, 0, sizeof(machine->buffer));
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Enter text (empty line to exit):\n");
    
    while (1) {
        int c = getchar();
        if (c == EOF) {
            break;
        }
        
        if (c == '\n' && machine.buffer_pos == 0) {
            break;
        }
        
        State next_state = get_next_state(machine.current_state, (char)c);
        handle_state_action(&machine, (char)c);
        machine.current_state = next_state;
        
        if (machine.current_state == STATE_COMPLETE) {
            machine.current_state = STATE_IDLE;
        }
    }
    
    printf("Total strings processed: %d\n", machine.processed_count);
    return 0;
}