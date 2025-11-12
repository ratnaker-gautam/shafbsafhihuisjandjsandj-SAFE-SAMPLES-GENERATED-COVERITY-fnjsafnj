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

#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_len;
    size_t output_len;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    if (!machine) return;
    machine->state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    memset(machine->output_buffer, 0, sizeof(machine->output_buffer));
}

static int validate_input(const char *input, size_t len) {
    if (!input) return 0;
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static void process_data(state_machine_t *machine) {
    if (!machine || machine->state != STATE_PROCESSING) return;
    
    if (machine->input_len == 0) {
        machine->state = STATE_ERROR;
        return;
    }
    
    machine->output_len = 0;
    for (size_t i = 0; i < machine->input_len && machine->input_buffer[i] != 0; i++) {
        if (machine->output_len >= MAX_OUTPUT_LEN - 1) {
            machine->state = STATE_ERROR;
            return;
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
    machine->output_buffer[machine->output_len] = 0;
    machine->state = STATE_WRITING;
}

static void handle_idle(state_machine_t *machine) {
    if (!machine || machine->state != STATE_IDLE) return;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        machine->state = STATE_ERROR;
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = 0;
        len--;
    }
    
    if (!validate_input(input, len)) {
        machine->state = STATE_ERROR;
        return;
    }
    
    if (len >= MAX_INPUT_LEN) {
        machine->state = STATE_ERROR;
        return;
    }
    
    memcpy(machine->input_buffer, input, len + 1);
    machine->input_len = len;
    machine->state = STATE_READING;
}

static void handle_reading(state_machine_t *machine) {
    if (!machine || machine->state != STATE_READING) return;
    machine->state = STATE_PROCESSING;
}

static void handle_writing(state_machine_t *machine) {
    if (!machine || machine->state != STATE_WRITING) return;
    
    if (machine->output_len == 0 || machine->output_len >= MAX_OUTPUT_LEN) {
        machine->state = STATE_ERROR;
        return;
    }
    
    printf("Processed: %s\n", machine->output_buffer);
    machine->state = STATE_IDLE;
}

static void handle_error(state_machine_t *machine) {
    if (!machine || machine->state != STATE_ERROR) return;
    printf("Error occurred in state machine\n");
    initialize_machine(machine);
}

static void run_state_machine(state_machine_t *machine) {
    if (!machine) return;
    
    int cycles = 0;
    while (machine->state != STATE_ERROR && cycles < 10) {
        switch (machine->state) {
            case STATE_IDLE:
                handle_idle(machine);
                break;
            case STATE_READING:
                handle_reading(machine);
                break;
            case STATE_PROCESSING:
                process_data(machine);
                break;
            case STATE_WRITING:
                handle_writing(machine);
                break;
            case STATE_ERROR:
                handle_error(machine);
                break;
            default:
                machine->state = STATE_ERROR;
                break;
        }
        cycles++;
    }
    
    if (machine->state == STATE_ERROR) {
        handle_error(machine);
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    run_state_machine(&machine);
    return 0;
}