//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    uint32_t data_value;
    uint8_t error_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->data_value = 0;
    machine->error_count = 0;
}

static system_state_t state_transition(state_machine_t *machine) {
    uint32_t computation;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter < 100) {
                machine->data_value = (machine->data_value * 17 + 13) % 9973;
                return STATE_PROCESSING;
            }
            return STATE_COMPLETE;
            
        case STATE_PROCESSING:
            computation = (machine->data_value * machine->data_value) % 10007;
            if (computation > 5000) {
                return STATE_WAITING;
            } else if (computation < 100) {
                machine->error_count++;
                return STATE_ERROR;
            }
            return STATE_VALIDATE;
            
        case STATE_WAITING:
            if (machine->counter % 3 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_WAITING;
            
        case STATE_ERROR:
            if (machine->error_count > 5) {
                return STATE_RESET;
            }
            return STATE_IDLE;
            
        case STATE_RESET:
            machine->error_count = 0;
            machine->data_value = 0;
            return STATE_IDLE;
            
        case STATE_VALIDATE:
            if (machine->data_value % 7 == 0) {
                return STATE_FINALIZE;
            }
            return STATE_PROCESSING;
            
        case STATE_FINALIZE:
            if (machine->counter > 50) {
                return STATE_COMPLETE;
            }
            return STATE_IDLE;
            
        case STATE_COMPLETE:
            return STATE_COMPLETE;
            
        default:
            return STATE_ERROR;
    }
}

static void execute_state_action(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_PROCESSING:
            machine->data_value = (machine->data_value + 1) % 10000;
            break;
        case STATE_VALIDATE:
            machine->data_value = (machine->data_value * 3 + 1) % 99991;
            break;
        case STATE_FINALIZE:
            machine->data_value = (machine->data_value / 2) % 1000;
            break;
        default:
            break;
    }
}

int main(void) {
    state_machine_t machine;
    uint64_t step_count = 0;
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (machine.current_state != STATE_COMPLETE && step_count < MAX_STEPS) {
        execute_state_action(&machine);
        
        system_state_t next_state = state_transition(&machine);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = next_state;
        }
        
        machine.counter++;
        step_count++;
        
        if (step_count % 10000 == 0) {
            printf("Step: %lu, State: %d, Data: %u\n", 
                   step_count, machine.current_state, machine.data_value);
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total steps: %lu\n", step_count);
    printf("Final data value: %u\n", machine.data_value);
    printf("Error count: %u\n", machine.error_count);
    
    return machine.current_state == STATE_COMPLETE ? 0 : 1;
}