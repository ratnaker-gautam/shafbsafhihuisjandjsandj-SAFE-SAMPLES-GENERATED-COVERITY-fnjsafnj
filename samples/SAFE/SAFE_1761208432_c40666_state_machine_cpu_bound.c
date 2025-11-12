//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATE,
    STATE_FINALIZE
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    uint8_t data_buffer[64];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
}

static system_state_t state_transition(system_state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return STATE_PROCESSING;
        case STATE_PROCESSING:
            if (counter % 3 == 0) return STATE_WAITING;
            return STATE_VALIDATE;
        case STATE_WAITING:
            return STATE_VALIDATE;
        case STATE_VALIDATE:
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_COMPLETE;
        case STATE_COMPLETE:
            return STATE_FINALIZE;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_FINALIZE:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

static void process_state(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter = 0;
            break;
        case STATE_PROCESSING:
            for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] = (uint8_t)((machine->counter + i) % 256);
            }
            machine->counter++;
            break;
        case STATE_WAITING:
            machine->counter += 2;
            break;
        case STATE_VALIDATE:
            for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
                if (machine->data_buffer[i] != (uint8_t)((machine->counter + i) % 256)) {
                    machine->current_state = STATE_ERROR;
                    return;
                }
            }
            break;
        case STATE_COMPLETE:
            machine->counter *= 2;
            break;
        case STATE_ERROR:
            machine->counter = 0;
            memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
            break;
        case STATE_RESET:
            machine->iterations++;
            break;
        case STATE_FINALIZE:
            machine->counter = 0;
            machine->iterations++;
            break;
        default:
            break;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    uint32_t max_iterations;
    printf("Enter maximum iterations (1-1000000): ");
    if (scanf("%u", &max_iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (max_iterations < 1 || max_iterations > MAX_STEPS) {
        fprintf(stderr, "Iterations out of range\n");
        return 1;
    }
    
    clock_t start_time = clock();
    if (start_time == (clock_t)-1) {
        fprintf(stderr, "Clock error\n");
        return 1;
    }
    
    while (machine.iterations < max_iterations) {
        process_state(&machine);
        machine.current_state = state_transition(machine.current_state, machine.counter);
        
        if (machine.current_state == STATE_FINALIZE) {
            machine.iterations++;
        }
        
        if (machine.counter > UINT64_MAX - 1000) {
            machine.counter = 0;
        }
    }
    
    clock_t end_time = clock();
    if (end_time == (clock_t)-1) {
        fprintf(stderr, "Clock error\n");
        return 1;
    }
    
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Completed %u iterations in %.3f seconds\n", machine.iterations, cpu_time);
    printf("Final counter value: %lu\n", machine.counter);
    
    return 0;
}