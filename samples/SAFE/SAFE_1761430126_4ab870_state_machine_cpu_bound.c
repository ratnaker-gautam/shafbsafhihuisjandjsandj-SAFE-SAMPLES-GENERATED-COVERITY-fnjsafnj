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
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_VALIDATE;
        case STATE_WAITING:
            return STATE_PROCESSING;
        case STATE_VALIDATE:
            if (counter % 5 == 0) return STATE_COMPLETE;
            return STATE_PROCESSING;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_COMPLETE:
            return STATE_FINALIZE;
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
        case STATE_PROCESSING:
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] = (uint8_t)((machine->counter + i) % 256);
            }
            machine->counter += 1;
            break;
            
        case STATE_VALIDATE:
            temp = 0;
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                temp += machine->data_buffer[i];
            }
            if (temp % 2 != 0) {
                machine->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WAITING:
            for (i = 0; i < 100; i++) {
                machine->counter = (machine->counter * 1103515245 + 12345) % 2147483648;
            }
            break;
            
        case STATE_ERROR:
            memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
            machine->counter = 0;
            break;
            
        case STATE_COMPLETE:
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] = 0xFF;
            }
            break;
            
        case STATE_FINALIZE:
            machine->counter = 0;
            break;
            
        default:
            break;
    }
}

int main(void) {
    state_machine_t machine;
    uint32_t step_count = 0;
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (step_count < MAX_STEPS) {
        process_state(&machine);
        
        system_state_t next_state = state_transition(machine.current_state, machine.counter);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            next_state = STATE_IDLE;
        }
        
        machine.current_state = next_state;
        machine.iterations++;
        step_count++;
        
        if (machine.current_state == STATE_IDLE && machine.iterations > 1000) {
            break;
        }
        
        if (step_count % 10000 == 0) {
            printf("Step: %u, State: %d, Counter: %lu\n", 
                   step_count, machine.current_state, machine.counter);
        }
    }
    
    printf("Simulation completed after %u steps\n", step_count);
    printf("Final state: %d, Final counter: %lu\n", machine.current_state, machine.counter);
    
    return 0;
}