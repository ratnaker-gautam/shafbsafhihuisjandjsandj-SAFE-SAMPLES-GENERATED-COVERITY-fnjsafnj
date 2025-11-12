//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
    if (machine == NULL) return;
    machine->state = STATE_IDLE;
    machine->input_pos = 0;
    machine->output_pos = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    memset(machine->output_buffer, 0, sizeof(machine->output_buffer));
}

static int handle_idle(state_machine_t *machine, char input) {
    if (machine == NULL) return 0;
    if (input == 'S') {
        TRANSITION(machine, STATE_READING);
        return 1;
    }
    return 0;
}

static int handle_reading(state_machine_t *machine, char input) {
    if (machine == NULL) return 0;
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
    if (machine == NULL) return 0;
    
    machine->output_pos = 0;
    
    for (i = 0; i < machine->input_pos && machine->output_pos < MAX_OUTPUT_LEN - 2; i++) {
        char c = machine->input_buffer[i];
        if (IS_DIGIT(c)) {
            if (machine->output_pos < MAX_OUTPUT_LEN - 3) {
                machine->output_buffer[machine->output_pos++] = '[';
                machine->output_buffer[machine->output_pos++] = c;
                machine->output_buffer[machine->output_pos++] = ']';
            } else {
                break;
            }
        } else if (IS_VALID_CHAR(c)) {
            machine->output_buffer[machine->output_pos++] = c;
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
    if (machine == NULL) return 0;
    printf("Processed: %s\n", machine->output_buffer);
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

static int handle_error(state_machine_t *machine) {
    if (machine == NULL) return 0;
    printf("Error: Invalid input sequence\n");
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

static int process_input(state_machine_t *machine, char input) {
    if (machine == NULL) return 0;
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine, input);
        case STATE_READING:
            return handle_reading(machine, input);
        case STATE_PROCESSING:
            return 0;
        case STATE_WRITING:
            return 0;
        case STATE_ERROR:
            return handle_error(machine);
        default:
            TRANSITION(machine, STATE_ERROR);
            return 0;
    }
}

static void run_state_machine(state_machine_t *machine) {
    int input;
    int result;
    
    if (machine == NULL) return;
    
    printf("State Machine Started\n");
    printf("Commands: S (start), E (end), Ctrl+D to exit\n");
    
    while (1) {
        if (machine->state == STATE_PROCESSING) {
            result = handle_processing(machine);
            if (!result) continue;
        }
        if (machine->state == STATE_WRITING) {
            result = handle_writing(machine);
            if (!result) continue;
        }
        
        input = getchar();
        if (input == EOF) break;
        if (input == '\n') continue;
        
        result = process_input(machine, (char)input);
        if (!result && machine->state == STATE_ERROR) {
            handle_error(machine);
        }
    }
    
    printf("State Machine Stopped\n");
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine);
    run_state_machine(&machine);
    return