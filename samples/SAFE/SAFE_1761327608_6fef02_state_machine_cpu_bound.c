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
    STATE_READY,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETED,
    STATE_ERROR,
    STATE_RESET,
    STATE_SHUTDOWN
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
            if (machine->data_value < 1000) {
                return STATE_READY;
            }
            return STATE_SHUTDOWN;
            
        case STATE_READY:
            computation = machine->data_value * 3;
            if (computation / 3 != machine->data_value) {
                machine->error_count++;
                return STATE_ERROR;
            }
            machine->data_value = computation;
            return STATE_PROCESSING;
            
        case STATE_PROCESSING:
            if (machine->data_value > 1000000) {
                machine->error_count++;
                return STATE_ERROR;
            }
            computation = machine->data_value + 7919;
            if (computation < machine->data_value) {
                machine->error_count++;
                return STATE_ERROR;
            }
            machine->data_value = computation;
            return STATE_WAITING;
            
        case STATE_WAITING:
            if (machine->data_value % 7 == 0) {
                return STATE_COMPLETED;
            }
            return STATE_PROCESSING;
            
        case STATE_COMPLETED:
            if (machine->data_value < 50000) {
                machine->data_value *= 2;
                if (machine->data_value < 50000) {
                    return STATE_READY;
                }
            }
            return STATE_RESET;
            
        case STATE_ERROR:
            if (machine->error_count < 3) {
                machine->data_value /= 2;
                return STATE_RESET;
            }
            return STATE_SHUTDOWN;
            
        case STATE_RESET:
            machine->data_value = 1;
            machine->error_count = 0;
            return STATE_IDLE;
            
        case STATE_SHUTDOWN:
        default:
            return STATE_SHUTDOWN;
    }
}

static void perform_cpu_work(uint32_t value) {
    volatile uint64_t result = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        result += value * i;
        result ^= (result >> 13);
        result *= 0x9E3779B1;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting CPU-bound state machine simulation\n");
    
    while (machine.current_state != STATE_SHUTDOWN && 
           machine.iteration_count < MAX_ITERATIONS) {
        
        perform_cpu_work(machine.data_value);
        
        system_state_t next_state = process_state_transition(&machine);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            machine.current_state = STATE_ERROR;
            machine.error_count++;
        } else {
            machine.current_state = next_state;
        }
        
        machine.iteration_count++;
        
        if (machine.iteration_count % 100000 == 0) {
            printf("Iteration: %lu, State: %d, Data: %u\n", 
                   (unsigned long)machine.iteration_count,
                   machine.current_state,
                   machine.data_value);
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total iterations: %lu\n", (unsigned long)machine.iteration_count);
    printf("Final data value: %u\n", machine.data_value);
    printf("Error count: %u\n", machine.error_count);
    
    return machine.current_state == STATE_SHUTDOWN ? EXIT_SUCCESS : EXIT_FAILURE;
}