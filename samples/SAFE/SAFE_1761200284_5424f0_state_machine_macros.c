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
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN + 1];
    char output_buffer[MAX_OUTPUT_LEN + 1];
    size_t input_pos;
    size_t output_pos;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->input_pos = 0;
    machine->output_pos = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    memset(machine->output_buffer, 0, sizeof(machine->output_buffer));
}

static int handle_idle(state_machine_t *machine, char input) {
    if (input == 'S') {
        TRANSITION(machine, STATE_READING);
        return 1;
    }
    return 0;
}

static int handle_reading(state_machine_t *machine, char input) {
    if (input == 'E') {
        if (machine->input_pos > 0) {
            TRANSITION(machine, STATE_PROCESSING);
            return 1;
        } else {
            TRANSITION(machine, STATE_ERROR);
            return 0;
        }
    } else if (IS_VALID_CHAR(input) && machine->input_pos < MAX_INPUT_LEN) {
        machine->input_buffer[machine->input_pos++] = input;
        return 1;
    } else {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
}

static int handle_processing(state_machine_t *machine) {
    size_t i;
    for (i = 0; i < machine->input_pos && machine->output_pos < MAX_OUTPUT_LEN - 2; i++) {
        if (IS_DIGIT(machine->input_buffer[i])) {
            machine->output_buffer[machine->output_pos++] = machine->input_buffer[i];
        }
    }
    if (machine->output_pos > 0) {
        TRANSITION(machine, STATE_WRITING);
        return 1;
    } else {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
}

static int handle_writing(state_machine_t *machine) {
    if (machine->output_pos > 0) {
        printf("Processed digits: %s\n", machine->output_buffer);
        TRANSITION(machine, STATE_IDLE);
        return 1;
    } else {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
}

static int handle_error(state_machine_t *machine) {
    printf("Error: Invalid state transition or input\n");
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

static int process_input(state_machine_t *machine, char input) {
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine, input);
        case STATE_READING:
            return handle_reading(machine, input);
        case STATE_PROCESSING:
            return handle_processing(machine);
        case STATE_WRITING:
            return handle_writing(machine);
        case STATE_ERROR:
            return handle_error(machine);
        default:
            TRANSITION(machine, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    state_machine_t machine;
    char input_char;
    int result;
    
    initialize_machine(&machine);
    
    printf("State Machine Demo - Extract digits from input\n");
    printf("Commands: 'S' to start, input text, 'E' to end processing\n");
    printf("Enter 'Q' to quit\n");
    
    while (1) {
        if (machine.state == STATE_PROCESSING || machine.state == STATE_WRITING || machine.state == STATE_ERROR) {
            process_input(&machine, '\0');
        }
        
        printf("Current state: %d > ", machine.state);
        result = scanf(" %c", &input_char);
        
        if (result != 1) {
            clearerr(stdin);
            while (getchar() != '\n');
            continue;
        }
        
        if (input_char == 'Q') {
            break;
        }
        
        if (!process_input(&machine, input_char)) {
            if (machine.state == STATE_ERROR) {
                initialize_machine(&machine);
            }
        }
    }
    
    printf("Program terminated\n");
    return 0;
}