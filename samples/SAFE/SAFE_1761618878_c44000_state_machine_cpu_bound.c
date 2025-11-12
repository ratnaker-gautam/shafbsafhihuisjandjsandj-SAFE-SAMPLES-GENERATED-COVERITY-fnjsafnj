//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_SUSPEND,
    STATE_TERMINATE
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t iteration_count;
    uint32_t data_value;
    uint8_t error_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->iteration_count = 0;
    machine->data_value = 1;
    machine->error_count = 0;
}

static system_state_t process_state_transition(state_machine_t *machine) {
    uint32_t computation;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            computation = (machine->data_value * 3 + 1) % 1000;
            machine->data_value = computation;
            return STATE_PROCESSING;
            
        case STATE_PROCESSING:
            computation = (machine->data_value * machine->data_value) % 10000;
            machine->data_value = computation;
            if (machine->data_value % 7 == 0) {
                return STATE_WAITING;
            }
            return STATE_COMPLETING;
            
        case STATE_WAITING:
            computation = (machine->data_value + 17) % 500;
            machine->data_value = computation;
            return STATE_PROCESSING;
            
        case STATE_COMPLETING:
            computation = (machine->data_value / 2) % 100;
            machine->data_value = computation;
            if (machine->data_value < 10) {
                machine->error_count++;
                if (machine->error_count > 3) {
                    return STATE_TERMINATE;
                }
                return STATE_ERROR;
            }
            return STATE_IDLE;
            
        case STATE_ERROR:
            machine->data_value = (machine->data_value + 100) % 1000;
            return STATE_RESET;
            
        case STATE_RESET:
            machine->data_value = 1;
            return STATE_IDLE;
            
        case STATE_SUSPEND:
            return STATE_TERMINATE;
            
        case STATE_TERMINATE:
            return STATE_TERMINATE;
            
        default:
            return STATE_ERROR;
    }
}

static void print_state_info(const state_machine_t *machine) {
    const char *state_names[] = {
        "IDLE", "PROCESSING", "WAITING", "COMPLETING",
        "ERROR", "RESET", "SUSPEND", "TERMINATE"
    };
    
    printf("Iteration: %lu, State: %s, Data: %u, Errors: %u\n",
           machine->iteration_count,
           state_names[machine->current_state],
           machine->data_value,
           machine->error_count);
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    while (machine.iteration_count < MAX_ITERATIONS) {
        if (machine.iteration_count % 10000 == 0) {
            print_state_info(&machine);
        }
        
        system_state_t next_state = process_state_transition(&machine);
        machine.current_state = next_state;
        machine.iteration_count++;
        
        if (machine.current_state == STATE_TERMINATE) {
            break;
        }
        
        if (machine.iteration_count >= MAX_ITERATIONS) {
            machine.current_state = STATE_TERMINATE;
            break;
        }
    }
    
    printf("Final state: ");
    print_state_info(&machine);
    printf("Simulation completed after %lu iterations.\n", machine.iteration_count);
    
    return 0;
}