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
    size_t input_len;
    size_t output_len;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
    memset(machine->output_buffer, 0, MAX_OUTPUT_LEN);
}

static int handle_idle(state_machine_t *machine) {
    if (machine == NULL) return -1;
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    char input_line[MAX_INPUT_LEN + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
        len--;
    }
    
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input_line[i])) {
            return -1;
        }
    }
    
    strncpy(machine->input_buffer, input_line, MAX_INPUT_LEN - 1);
    machine->input_buffer[MAX_INPUT_LEN - 1] = '\0';
    machine->input_len = strlen(machine->input_buffer);
    
    TRANSITION(machine, STATE_READING);
    return 0;
}

static int handle_reading(state_machine_t *machine) {
    if (machine == NULL) return -1;
    if (machine->input_len == 0) {
        TRANSITION(machine, STATE_ERROR);
        return -1;
    }
    
    TRANSITION(machine, STATE_PROCESSING);
    return 0;
}

static int handle_processing(state_machine_t *machine) {
    if (machine == NULL) return -1;
    machine->output_len = 0;
    
    for (size_t i = 0; i < machine->input_len; i++) {
        if (!CHECK_BOUNDS(machine->output_len, MAX_OUTPUT_LEN - 1)) {
            TRANSITION(machine, STATE_ERROR);
            return -1;
        }
        
        char c = machine->input_buffer[i];
        if (c >= 'a' && c <= 'z') {
            machine->output_buffer[machine->output_len++] = c - 32;
        } else if (c >= 'A' && c <= 'Z') {
            machine->output_buffer[machine->output_len++] = c + 32;
        } else {
            machine->output_buffer[machine->output_len++] = c;
        }
    }
    
    if (machine->output_len < MAX_OUTPUT_LEN) {
        machine->output_buffer[machine->output_len] = '\0';
    } else {
        machine->output_buffer[MAX_OUTPUT_LEN - 1] = '\0';
    }
    TRANSITION(machine, STATE_WRITING);
    return 0;
}

static int handle_writing(state_machine_t *machine) {
    if (machine == NULL) return -1;
    printf("Processed output: %s\n", machine->output_buffer);
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

static int handle_error(state_machine_t *machine) {
    if (machine == NULL) return -1;
    printf("Error: Invalid input or processing error\n");
    initialize_machine(machine);
    return 0;
}

static int run_state_machine(state_machine_t *machine) {
    if (machine == NULL) return -1;
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine);
        case STATE_READING:
            return handle_reading(machine);
        case STATE_PROCESSING:
            return handle_processing(machine);
        case STATE_WRITING:
            return handle_writing(machine);
        case STATE_ERROR:
            return handle_error(machine);
        default:
            machine->state = STATE_ERROR;
            return -1;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("State Machine Text Processor (Ctrl+C to exit)\n");
    
    int cycles = 0;
    while (cycles < 10) {
        if (run_state_machine(&machine) < 0) {
            machine.state = STATE_ERROR;
        }
        cycles++;
    }
    
    printf("Processing complete after %d cycles\n",