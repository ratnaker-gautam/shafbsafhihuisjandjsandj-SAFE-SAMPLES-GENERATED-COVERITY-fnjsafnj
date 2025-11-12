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
    STATE_READY,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_RESET,
    STATE_SHUTDOWN
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t cycle_count;
    uint32_t data_value;
    uint8_t error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->cycle_count = 0;
    machine->data_value = 0;
    machine->error_code = 0;
}

static system_state_t process_state_transition(state_machine_t *machine) {
    uint32_t computation;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->cycle_count < 10) {
                machine->data_value += 1;
                return STATE_IDLE;
            }
            return STATE_READY;
            
        case STATE_READY:
            computation = machine->data_value * 3;
            if (computation / 3 != machine->data_value) {
                machine->error_code = 1;
                return STATE_ERROR;
            }
            machine->data_value = computation;
            return STATE_PROCESSING;
            
        case STATE_PROCESSING:
            if (machine->data_value > 1000000) {
                machine->error_code = 2;
                return STATE_ERROR;
            }
            
            for (int i = 0; i < 100; i++) {
                machine->data_value = (machine->data_value * 1103515245 + 12345) & 0x7FFFFFFF;
            }
            
            if (machine->data_value % 100 == 0) {
                return STATE_WAITING;
            }
            return STATE_COMPLETE;
            
        case STATE_WAITING:
            if (machine->cycle_count % 5 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_WAITING;
            
        case STATE_COMPLETE:
            if (machine->cycle_count < 50) {
                return STATE_COMPLETE;
            }
            return STATE_RESET;
            
        case STATE_ERROR:
            if (machine->error_code == 1) {
                machine->data_value = 1;
                return STATE_RESET;
            }
            return STATE_SHUTDOWN;
            
        case STATE_RESET:
            machine->data_value = 0;
            machine->error_code = 0;
            return STATE_IDLE;
            
        case STATE_SHUTDOWN:
        default:
            return STATE_SHUTDOWN;
    }
}

static void run_state_machine(state_machine_t *machine, uint32_t max_cycles) {
    uint32_t cycles = 0;
    
    while (cycles < max_cycles && machine->current_state != STATE_SHUTDOWN) {
        machine->cycle_count++;
        cycles++;
        
        system_state_t next_state = process_state_transition(machine);
        
        if (next_state < STATE_IDLE || next_state > STATE_SHUTDOWN) {
            machine->current_state = STATE_SHUTDOWN;
            break;
        }
        
        machine->current_state = next_state;
        
        if (cycles % 10000 == 0) {
            printf("Cycle: %u, State: %d, Data: %u\n", 
                   cycles, machine->current_state, machine->data_value);
        }
    }
    
    if (machine->current_state == STATE_SHUTDOWN) {
        printf("Machine shutdown after %u cycles\n", cycles);
    } else {
        printf("Completed %u cycles, final state: %d\n", cycles, machine->current_state);
    }
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    run_state_machine(&machine, MAX_STEPS);
    
    printf("Final data value: %u\n", machine.data_value);
    printf("Final error code: %u\n", machine.error_code);
    
    return 0;
}