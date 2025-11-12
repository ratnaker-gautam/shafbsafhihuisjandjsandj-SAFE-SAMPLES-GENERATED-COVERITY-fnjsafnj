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
    STATE_CALIBRATING,
    STATE_SHUTDOWN
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t iteration_count;
    uint32_t error_count;
    uint32_t processed_count;
    uint32_t calibration_value;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->iteration_count = 0;
    machine->error_count = 0;
    machine->processed_count = 0;
    machine->calibration_value = 1;
}

static system_state_t process_state_transition(state_machine_t *machine, uint32_t input) {
    system_state_t next_state = machine->current_state;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (input < 100) {
                next_state = STATE_PROCESSING;
            } else if (input < 200) {
                next_state = STATE_CALIBRATING;
            }
            break;
            
        case STATE_PROCESSING:
            if (input % 2 == 0) {
                next_state = STATE_WAITING;
            } else if (input % 3 == 0) {
                next_state = STATE_COMPLETING;
            } else if (input % 13 == 0) {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_WAITING:
            if (input > 500) {
                next_state = STATE_PROCESSING;
            } else if (input < 100) {
                next_state = STATE_COMPLETING;
            }
            break;
            
        case STATE_COMPLETING:
            if (input % 7 == 0) {
                next_state = STATE_IDLE;
            } else {
                next_state = STATE_PROCESSING;
            }
            break;
            
        case STATE_ERROR:
            if (input % 2 == 0) {
                next_state = STATE_RESET;
            }
            break;
            
        case STATE_RESET:
            next_state = STATE_IDLE;
            break;
            
        case STATE_CALIBRATING:
            if (input > 300) {
                next_state = STATE_IDLE;
            } else if (input < 50) {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_SHUTDOWN:
            break;
    }
    
    return next_state;
}

static void execute_state_action(state_machine_t *machine, uint32_t input) {
    switch (machine->current_state) {
        case STATE_PROCESSING:
            machine->processed_count++;
            machine->calibration_value = (machine->calibration_value * 3 + input) % 1000;
            break;
            
        case STATE_COMPLETING:
            machine->calibration_value = (machine->calibration_value + 17) % 1000;
            break;
            
        case STATE_ERROR:
            machine->error_count++;
            break;
            
        case STATE_CALIBRATING:
            machine->calibration_value = (machine->calibration_value + input) % 1000;
            break;
            
        case STATE_RESET:
            machine->calibration_value = 1;
            break;
            
        default:
            break;
    }
}

static uint32_t generate_input(uint64_t iteration) {
    return (uint32_t)((iteration * 1103515245 + 12345) % 1000);
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    for (uint64_t i = 0; i < MAX_ITERATIONS; i++) {
        if (machine.current_state == STATE_SHUTDOWN) {
            break;
        }
        
        uint32_t input = generate_input(i);
        system_state_t next_state = process_state_transition(&machine, input);
        
        if (next_state >= STATE_IDLE && next_state <= STATE_SHUTDOWN) {
            machine.current_state = next_state;
        } else {
            machine.current_state = STATE_ERROR;
        }
        
        execute_state_action(&machine, input);
        machine.iteration_count++;
        
        if (machine.error_count > 100) {
            machine.current_state = STATE_SHUTDOWN;
        }
        
        if (machine.processed_count > 50000) {
            machine.current_state = STATE_SHUTDOWN;
        }
    }
    
    printf("Simulation completed.\n");
    printf("Final state: %d\n", machine.current_state);
    printf("Total iterations: %lu\n", machine.iteration_count);
    printf("Processed items: %u\n", machine.processed_count);
    printf("Error count: %u\n", machine.error_count);
    printf("Final calibration: %u\n", machine.calibration_value);
    
    return 0;
}