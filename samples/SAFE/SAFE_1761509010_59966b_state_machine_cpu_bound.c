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
            computation = machine->data_value * 3 + 1;
            if (computation < machine->data_value) return STATE_ERROR;
            machine->data_value = computation;
            return STATE_PROCESSING;
            
        case STATE_PROCESSING:
            if (machine->data_value % 2 == 0) {
                computation = machine->data_value / 2;
                if (computation == 0) return STATE_ERROR;
                machine->data_value = computation;
                return STATE_WAITING;
            } else {
                computation = machine->data_value * 3 + 1;
                if (computation < machine->data_value) return STATE_ERROR;
                machine->data_value = computation;
                return STATE_COMPLETING;
            }
            
        case STATE_WAITING:
            computation = machine->data_value + 1;
            if (computation <= machine->data_value) return STATE_ERROR;
            machine->data_value = computation;
            return STATE_SUSPEND;
            
        case STATE_COMPLETING:
            computation = machine->data_value - 1;
            if (computation >= machine->data_value) return STATE_ERROR;
            machine->data_value = computation;
            return machine->data_value == 1 ? STATE_TERMINATE : STATE_RESET;
            
        case STATE_ERROR:
            machine->error_count++;
            if (machine->error_count > 5) return STATE_TERMINATE;
            machine->data_value = 1;
            return STATE_RESET;
            
        case STATE_RESET:
            machine->data_value = (machine->data_value % 100) + 1;
            return STATE_IDLE;
            
        case STATE_SUSPEND:
            return machine->data_value > 1000 ? STATE_TERMINATE : STATE_PROCESSING;
            
        case STATE_TERMINATE:
        default:
            return STATE_TERMINATE;
    }
}

static void execute_cpu_workload(state_machine_t *machine) {
    uint32_t temp;
    for (int i = 0; i < 1000; i++) {
        temp = machine->data_value;
        for (int j = 0; j < 10; j++) {
            temp = (temp * 1103515245 + 12345) & 0x7FFFFFFF;
        }
        machine->data_value = (temp % 1000) + 1;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (machine.current_state != STATE_TERMINATE) {
        if (machine.iteration_count >= MAX_ITERATIONS) {
            printf("Maximum iterations reached\n");
            break;
        }
        
        execute_cpu_workload(&machine);
        
        system_state_t next_state = process_state_transition(&machine);
        
        if (next_state < STATE_IDLE || next_state > STATE_TERMINATE) {
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = next_state;
        }
        
        machine.iteration_count++;
        
        if (machine.iteration_count % 10000 == 0) {
            printf("Iteration: %lu, State: %d, Data: %u\n", 
                   machine.iteration_count, machine.current_state, machine.data_value);
        }
    }
    
    printf("Final state: %d, Iterations: %lu, Data value: %u, Errors: %u\n",
           machine.current_state, machine.iteration_count, machine.data_value, machine.error_count);
    
    return machine.current_state == STATE_TERMINATE ? EXIT_SUCCESS : EXIT_FAILURE;
}