//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_FINAL
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    uint8_t error_code;
} state_machine_t;

static state_t state_transitions[STATE_COUNT][STATE_COUNT] = {
    {STATE_IDLE, STATE_READY, STATE_IDLE, STATE_IDLE, STATE_IDLE, STATE_ERROR, STATE_IDLE, STATE_IDLE},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_READY, STATE_READY, STATE_ERROR, STATE_RESET, STATE_READY},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_WAITING, STATE_COMPLETING, STATE_ERROR, STATE_RESET, STATE_PROCESSING},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_WAITING, STATE_COMPLETING, STATE_ERROR, STATE_RESET, STATE_WAITING},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_WAITING, STATE_COMPLETING, STATE_ERROR, STATE_RESET, STATE_FINAL},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_WAITING, STATE_COMPLETING, STATE_ERROR, STATE_RESET, STATE_ERROR},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_WAITING, STATE_COMPLETING, STATE_ERROR, STATE_RESET, STATE_RESET},
    {STATE_IDLE, STATE_READY, STATE_PROCESSING, STATE_WAITING, STATE_COMPLETING, STATE_ERROR, STATE_RESET, STATE_FINAL}
};

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

static state_t get_next_state(state_machine_t *machine) {
    if (machine == NULL) return STATE_ERROR;
    
    uint32_t state_idx = machine->current_state;
    if (state_idx >= STATE_COUNT) return STATE_ERROR;
    
    uint32_t next_idx = (machine->counter * 7 + machine->iterations * 13) % STATE_COUNT;
    if (next_idx >= STATE_COUNT) return STATE_ERROR;
    
    return state_transitions[state_idx][next_idx];
}

static void process_state(state_machine_t *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter += 1;
            break;
        case STATE_READY:
            machine->counter = machine->counter * 3 + 1;
            break;
        case STATE_PROCESSING:
            for (int i = 0; i < 100; i++) {
                machine->counter = (machine->counter * 1103515245 + 12345) & 0x7FFFFFFF;
            }
            break;
        case STATE_WAITING:
            machine->counter = (machine->counter >> 1) | ((machine->counter & 1) << 63);
            break;
        case STATE_COMPLETING:
            machine->counter = ~machine->counter;
            break;
        case STATE_ERROR:
            machine->error_code = (machine->counter % 256) + 1;
            break;
        case STATE_RESET:
            machine->counter = 0;
            machine->error_code = 0;
            break;
        case STATE_FINAL:
            machine->counter += 0x123456789ABCDEF;
            break;
        default:
            machine->current_state = STATE_ERROR;
            break;
    }
    
    machine->iterations++;
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }
    
    if (!validate_input(argv[1])) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    char *endptr;
    long input_iterations = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid number format\n");
        return 1;
    }
    
    if (input_iterations <= 0 || input_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations out of range (1-%d)\n", MAX_ITERATIONS);
        return 1;
    }
    
    state_m