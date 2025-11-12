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
    uint8_t data[256];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->steps = 0;
    memset(machine->data, 0, sizeof(machine->data));
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
            return (counter % 7 == 0) ? STATE_RESET : STATE_IDLE;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_VALIDATE:
            return (counter % 11 == 0) ? STATE_COMPLETE : STATE_PROCESSING;
        case STATE_FINALIZE:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

static void process_state(state_machine_t *machine) {
    uint32_t i;
    uint64_t temp;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter += 1;
            break;
        case STATE_PROCESSING:
            for (i = 0; i < 64; i++) {
                temp = machine->counter * i;
                machine->data[i % 256] = (uint8_t)(temp % 256);
            }
            machine->counter += 3;
            break;
        case STATE_WAITING:
            machine->counter += 7;
            break;
        case STATE_COMPLETE:
            for (i = 0; i < 128; i++) {
                temp = machine->counter + i * i;
                machine->data[(i * 2) % 256] = (uint8_t)(temp % 256);
            }
            machine->counter += 11;
            break;
        case STATE_ERROR:
            machine->counter += 13;
            break;
        case STATE_RESET:
            machine->counter = 1;
            break;
        case STATE_VALIDATE:
            for (i = 0; i < 96; i++) {
                temp = machine->counter - i;
                machine->data[(i * 3) % 256] = (uint8_t)(temp % 256);
            }
            machine->counter += 17;
            break;
        case STATE_FINALIZE:
            for (i = 0; i < 256; i++) {
                machine->data[i] = (uint8_t)((machine->counter + i) % 256);
            }
            machine->counter += 19;
            break;
    }
}

int main(void) {
    state_machine_t machine;
    uint32_t total_steps;
    clock_t start_time, end_time;
    double cpu_time;
    
    initialize_machine(&machine);
    total_steps = 500000;
    
    if (total_steps > MAX_STEPS) {
        fprintf(stderr, "Error: Step count exceeds maximum\n");
        return 1;
    }
    
    start_time = clock();
    if (start_time == (clock_t)-1) {
        fprintf(stderr, "Error: Clock initialization failed\n");
        return 1;
    }
    
    while (machine.steps < total_steps) {
        process_state(&machine);
        machine.current_state = state_transition(machine.current_state, machine.counter);
        machine.steps++;
        
        if (machine.steps % 100000 == 0) {
            printf("Step: %u, State: %d, Counter: %lu\n", 
                   machine.steps, machine.current_state, machine.counter);
        }
    }
    
    end_time = clock();
    if (end_time == (clock_t)-1) {
        fprintf(stderr, "Error: Clock reading failed\n");
        return 1;
    }
    
    cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Completed %u steps in %.3f seconds\n", total_steps, cpu_time);
    printf("Final counter: %lu\n", machine.counter);
    
    return 0;
}