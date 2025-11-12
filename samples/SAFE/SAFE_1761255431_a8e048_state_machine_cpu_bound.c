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
    uint32_t steps;
    uint8_t data_buffer[64];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->steps = 0;
    memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
}

static system_state_t state_transition(system_state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_PROCESSING : STATE_WAITING;
        case STATE_PROCESSING:
            return (counter % 3 == 0) ? STATE_COMPLETE : STATE_VALIDATE;
        case STATE_WAITING:
            return (counter % 5 == 0) ? STATE_PROCESSING : STATE_ERROR;
        case STATE_COMPLETE:
            return STATE_FINALIZE;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_VALIDATE:
            return (counter % 7 == 0) ? STATE_COMPLETE : STATE_PROCESSING;
        case STATE_FINALIZE:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static void process_state(state_machine_t *machine) {
    for (int i = 0; i < 64; i++) {
        machine->data_buffer[i] = (uint8_t)((machine->counter + i) % 256);
    }
    
    uint64_t checksum = 0;
    for (int i = 0; i < 64; i++) {
        checksum += machine->data_buffer[i];
    }
    
    machine->counter += checksum;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    clock_t start_time = clock();
    if (start_time == (clock_t)-1) {
        fprintf(stderr, "Error: Failed to get start time\n");
        return EXIT_FAILURE;
    }
    
    while (machine.steps < MAX_STEPS) {
        machine.current_state = state_transition(machine.current_state, machine.counter);
        
        if (machine.current_state >= STATE_COUNT) {
            fprintf(stderr, "Error: Invalid state detected\n");
            return EXIT_FAILURE;
        }
        
        process_state(&machine);
        machine.steps++;
        
        if (machine.counter > UINT64_MAX - 1000000) {
            machine.counter = 0;
        }
        
        if (machine.current_state == STATE_FINALIZE) {
            break;
        }
    }
    
    clock_t end_time = clock();
    if (end_time == (clock_t)-1) {
        fprintf(stderr, "Error: Failed to get end time\n");
        return EXIT_FAILURE;
    }
    
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("State machine completed %u steps\n", machine.steps);
    printf("Final counter value: %lu\n", machine.counter);
    printf("CPU time used: %.6f seconds\n", cpu_time);
    printf("Final state: %d\n", machine.current_state);
    
    return EXIT_SUCCESS;
}