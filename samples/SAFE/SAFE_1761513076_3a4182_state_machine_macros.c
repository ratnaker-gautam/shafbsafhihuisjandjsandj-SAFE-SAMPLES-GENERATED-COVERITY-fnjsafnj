//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_OUTPUT_LEN 200

#define TRANSITION(machine, new_state) do { (machine)->state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_pos;
    size_t output_pos;
} state_machine_t;

void init_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->input_pos = 0;
    machine->output_pos = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
    memset(machine->output_buffer, 0, MAX_OUTPUT_LEN);
}

int handle_idle(state_machine_t *machine, char input) {
    if (input == 'S') {
        TRANSITION(machine, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(state_machine_t *machine, char input) {
    if (!CHECK_BOUNDS(machine->input_pos, MAX_INPUT_LEN - 1)) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    if (input == 'E') {
        TRANSITION(machine, STATE_PROCESSING);
        return 1;
    }
    
    if (IS_VALID_CHAR(input)) {
        machine->input_buffer[machine->input_pos++] = input;
        return 1;
    }
    
    TRANSITION(machine, STATE_ERROR);
    return 0;
}

int handle_processing(state_machine_t *machine) {
    if (!CHECK_BOUNDS(machine->input_pos, MAX_INPUT_LEN) || 
        !CHECK_BOUNDS(machine->output_pos, MAX_OUTPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < machine->input_pos; i++) {
        if (!CHECK_BOUNDS(machine->output_pos, MAX_OUTPUT_LEN - 1)) {
            TRANSITION(machine, STATE_ERROR);
            return 0;
        }
        
        char c = machine->input_buffer[i];
        if (IS_VALID_CHAR(c)) {
            machine->output_buffer[machine->output_pos++] = c;
            if (!CHECK_BOUNDS(machine->output_pos, MAX_OUTPUT_LEN - 1)) {
                TRANSITION(machine, STATE_ERROR);
                return 0;
            }
            machine->output_buffer[machine->output_pos++] = '-';
        }
    }
    
    if (machine->output_pos > 0 && machine->output_buffer[machine->output_pos - 1] == '-') {
        machine->output_buffer[--machine->output_pos] = '\0';
    }
    
    TRANSITION(machine, STATE_WRITING);
    return 1;
}

int handle_writing(state_machine_t *machine) {
    if (!CHECK_BOUNDS(machine->output_pos, MAX_OUTPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    printf("Processed: %s\n", machine->output_buffer);
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int handle_error(state_machine_t *machine) {
    printf("Error: Invalid state transition or buffer overflow\n");
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

int process_input(state_machine_t *machine, char input) {
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine, input);
        case STATE_READING:
            return handle_reading(machine, input);
        case STATE_PROCESSING:
        case STATE_WRITING:
        case STATE_ERROR:
        default:
            return 0;
    }
}

int main(void) {
    state_machine_t machine;
    init_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("Commands: S (start), E (end), valid characters for input\n");
    printf("Enter sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (!process_input(&machine, (char)c)) {
            break;
        }
        
        if (machine.state == STATE_PROCESSING) {
            if (!handle_processing(&machine)) {
                break;
            }
        }
        
        if (machine.state == STATE_WRITING) {
            if (!handle_writing(&machine)) {
                break;
            }
        }
        
        if (machine.state == STATE_ERROR) {
            handle_error(&machine);
        }
    }
    
    if (machine.state == STATE_READING) {
        printf("Error: Input sequence not properly terminated with 'E'\n");
    }
    
    return 0;
}