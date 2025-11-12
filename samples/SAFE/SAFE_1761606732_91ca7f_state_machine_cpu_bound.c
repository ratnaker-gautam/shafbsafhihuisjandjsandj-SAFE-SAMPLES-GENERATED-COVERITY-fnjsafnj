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
    int error_code;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter, int *error) {
    *error = 0;
    
    switch (current) {
        case STATE_IDLE:
            if (counter % 2 == 0) return STATE_READY;
            return STATE_PROCESSING;
            
        case STATE_READY:
            if (counter % 3 == 0) return STATE_PROCESSING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_WAITING;
            
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_COMPLETING;
            if (counter % 11 == 0) return STATE_ERROR;
            return STATE_WAITING;
            
        case STATE_WAITING:
            if (counter % 13 == 0) return STATE_PROCESSING;
            if (counter % 17 == 0) return STATE_RESET;
            return STATE_COMPLETING;
            
        case STATE_COMPLETING:
            if (counter % 19 == 0) return STATE_FINAL;
            return STATE_IDLE;
            
        case STATE_ERROR:
            *error = (int)(counter % 256) + 1;
            return STATE_RESET;
            
        case STATE_RESET:
            if (counter % 23 == 0) return STATE_IDLE;
            return STATE_RESET;
            
        case STATE_FINAL:
            return STATE_FINAL;
            
        default:
            *error = 255;
            return STATE_ERROR;
    }
}

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

static int validate_iterations(unsigned int iterations) {
    if (iterations == 0 || iterations > MAX_ITERATIONS) {
        return 0;
    }
    return 1;
}

static void print_state_name(state_t state) {
    const char *names[] = {
        "IDLE", "READY", "PROCESSING", "WAITING", 
        "COMPLETING", "ERROR", "RESET", "FINAL"
    };
    
    if (state >= STATE_IDLE && state <= STATE_FINAL) {
        printf("%s", names[state]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    state_machine_t machine;
    unsigned int max_iterations;
    char input_buffer[32];
    
    printf("Enter number of state transitions (1-%u): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long input_value = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input_value <= 0 || input_value > MAX_ITERATIONS) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    max_iterations = (unsigned int)input_value;
    
    if (!validate_iterations(max_iterations)) {
        fprintf(stderr, "Invalid iteration count\n");
        return 1;
    }
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    printf("Initial state: ");
    print_state_name(machine.current_state);
    printf("\n");
    
    for (machine.iterations = 0; machine.iterations < max_iterations; machine.iterations++) {
        if (machine.current_state == STATE_FINAL) {
            break;
        }
        
        state_t next_state = state_transition(machine.current_state, machine.counter, &machine.error_code);
        
        printf("Step %u: ", machine.iterations + 1);
        print_state_name(machine.current_state);
        printf(" -> ");
        print_state_name(next_state);
        
        if (machine.error_code != 0) {
            printf(" [Error: %d]", machine.error_code);
        }
        printf("\n");
        
        machine.current_state = next_state;
        machine.counter++;
        
        if (machine.counter == UINT64_MAX) {
            machine.counter = 0;
        }
    }
    
    printf("Final state: ");
    print_state_name(machine.current_state);
    printf(" after %u iterations\n", machine.iterations);
    
    return 0;
}