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

static system_state_t state_transition(state_machine_t *machine) {
    uint32_t local_iter = machine->iterations;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (local_iter < 10) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
            
        case STATE_PROCESSING:
            if (local_iter < 100) {
                machine->counter += local_iter * 2;
                return STATE_WAITING;
            }
            return STATE_ERROR;
            
        case STATE_WAITING:
            if (local_iter % 3 == 0) {
                return STATE_VALIDATE;
            }
            return STATE_PROCESSING;
            
        case STATE_VALIDATE:
            if (machine->counter > 1000 && local_iter < 50) {
                return STATE_COMPLETE;
            }
            return STATE_RESET;
            
        case STATE_COMPLETE:
            return STATE_FINALIZE;
            
        case STATE_ERROR:
            if (local_iter > 5) {
                return STATE_RESET;
            }
            return STATE_ERROR;
            
        case STATE_RESET:
            machine->counter = 0;
            machine->iterations = 0;
            return STATE_IDLE;
            
        case STATE_FINALIZE:
            return STATE_FINALIZE;
            
        default:
            return STATE_ERROR;
    }
}

static void process_data(state_machine_t *machine) {
    for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
        if (i < sizeof(machine->data_buffer)) {
            machine->data_buffer[i] = (uint8_t)((machine->counter + i) % 256);
        }
    }
    
    uint32_t checksum = 0;
    for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
        checksum += machine->data_buffer[i];
    }
    
    if (checksum % 7 == 0) {
        machine->counter += checksum;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    uint32_t step_count = 0;
    system_state_t previous_state = machine.current_state;
    
    while (step_count < MAX_STEPS) {
        if (machine.current_state >= STATE_COUNT) {
            machine.current_state = STATE_ERROR;
        }
        
        process_data(&machine);
        
        previous_state = machine.current_state;
        machine.current_state = state_transition(&machine);
        machine.iterations++;
        
        if (machine.iterations > 1000000) {
            machine.iterations = 1000000;
        }
        
        if (machine.current_state == STATE_FINALIZE) {
            break;
        }
        
        if (machine.current_state == previous_state && machine.iterations > 100) {
            machine.current_state = STATE_RESET;
        }
        
        step_count++;
        if (step_count >= MAX_STEPS) {
            break;
        }
    }
    
    uint64_t final_result = machine.counter;
    for (uint32_t i = 0; i < 1000; i++) {
        if (i % 2 == 0) {
            final_result += i;
        } else {
            final_result -= i % 100;
        }
    }
    
    printf("Final state: %d\n", (int)machine.current_state);
    printf("Iterations: %u\n", machine.iterations);
    printf("Final counter: %lu\n", final_result);
    
    return 0;
}