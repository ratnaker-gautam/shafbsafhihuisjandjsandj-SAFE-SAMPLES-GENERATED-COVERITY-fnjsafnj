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

#define TRANSITION(machine, new_state) do { (machine)->state = (new_state); } while(0)
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_len;
    size_t output_len;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
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
        if (machine->input_len > 0) {
            TRANSITION(machine, STATE_PROCESSING);
            return 1;
        } else {
            TRANSITION(machine, STATE_ERROR);
            return 0;
        }
    }
    
    if (!IS_VALID_CHAR(input)) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    if (!CHECK_BOUNDS(machine->input_len + 1, MAX_INPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    machine->input_buffer[machine->input_len++] = input;
    return 1;
}

static int handle_processing(state_machine_t *machine) {
    if (machine->input_len == 0) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < machine->input_len; i++) {
        char c = machine->input_buffer[i];
        if (!CHECK_BOUNDS(machine->output_len + 2, MAX_OUTPUT_LEN)) {
            TRANSITION(machine, STATE_ERROR);
            return 0;
        }
        
        machine->output_buffer[machine->output_len++] = c;
        if (i < machine->input_len - 1) {
            machine->output_buffer[machine->output_len++] = '-';
        }
    }
    
    TRANSITION(machine, STATE_WRITING);
    return 1;
}

static int handle_writing(state_machine_t *machine) {
    if (machine->output_len == 0) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    if (fwrite(machine->output_buffer, 1, machine->output_len, stdout) != machine->output_len) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    printf("\n");
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

static int handle_error(state_machine_t *machine) {
    printf("ERROR: Invalid state transition or input\n");
    initialize_machine(machine);
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
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("Commands: S (start), E (end), characters (input), Q (quit)\n");
    
    int running = 1;
    while (running) {
        int c = getchar();
        if (c == EOF) {
            break;
        }
        
        while (getchar() != '\n' && c != EOF);
        
        if (c == 'Q') {
            running = 0;
            continue;
        }
        
        if (!process_input(&machine, (char)c)) {
            if (machine.state == STATE_ERROR) {
                handle_error(&machine);
            }
        }
        
        if (machine.state == STATE_PROCESSING) {
            process_input(&machine, 0);
        }
        if (machine.state == STATE_WRITING) {
            process_input(&machine, 0);
        }
    }
    
    return 0;
}