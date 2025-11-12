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
    uint32_t data_value;
    uint8_t error_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->data_value = 0;
    machine->error_count = 0;
}

static system_state_t process_state_transition(state_machine_t *machine) {
    uint32_t computation;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter < 1000) {
                machine->data_value = (machine->data_value * 17 + 13) % 7919;
                return STATE_PROCESSING;
            }
            return STATE_VALIDATE;
            
        case STATE_PROCESSING:
            computation = (machine->data_value * machine->data_value) % 7919;
            if (computation > 5000) {
                return STATE_WAITING;
            }
            return STATE_COMPLETE;
            
        case STATE_WAITING:
            if (machine->error_count < 3) {
                machine->data_value = (machine->data_value + 1) % 7919;
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
            
        case STATE_COMPLETE:
            if (machine->counter % 100 == 0) {
                return STATE_FINALIZE;
            }
            return STATE_IDLE;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            machine->error_count = 0;
            machine->data_value = 0;
            return STATE_IDLE;
            
        case STATE_VALIDATE:
            if (machine->data_value % 2 == 0) {
                return STATE_FINALIZE;
            }
            return STATE_IDLE;
            
        case STATE_FINALIZE:
            return STATE_IDLE;
            
        default:
            return STATE_ERROR;
    }
}

static void execute_state_action(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter++;
            break;
            
        case STATE_PROCESSING:
            machine->data_value = (machine->data_value * 3 + 7) % 7919;
            break;
            
        case STATE_WAITING:
            machine->error_count++;
            break;
            
        case STATE_COMPLETE:
            machine->counter += 2;
            break;
            
        case STATE_ERROR:
            machine->error_count = 5;
            break;
            
        case STATE_RESET:
            machine->counter = 0;
            break;
            
        case STATE_VALIDATE:
            machine->data_value = (machine->data_value + 11) % 7919;
            break;
            
        case STATE_FINALIZE:
            machine->counter = machine->counter * 2;
            if (machine->counter > 1000000) {
                machine->counter = 1000000;
            }
            break;
    }
}

int main(void) {
    state_machine_t machine;
    uint64_t step_count = 0;
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    while (step_count < MAX_STEPS) {
        if (machine.current_state >= STATE_COUNT) {
            printf("Invalid state detected, terminating\n");
            return 1;
        }
        
        execute_state_action(&machine);
        
        system_state_t next_state = process_state_transition(&machine);
        
        if (next_state >= STATE_COUNT) {
            printf("Invalid transition detected, terminating\n");
            return 1;
        }
        
        machine.current_state = next_state;
        step_count++;
        
        if (machine.current_state == STATE_FINALIZE && machine.counter >= 500000) {
            break;
        }
    }
    
    printf("Simulation completed after %lu steps\n", (unsigned long)step_count);
    printf("Final counter: %lu\n", (unsigned long)machine.counter);
    printf("Final data value: %u\n", machine.data_value);
    printf("Error count: %u\n", machine.error_count);
    
    return 0;
}