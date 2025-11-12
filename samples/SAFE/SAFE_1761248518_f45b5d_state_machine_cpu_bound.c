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
            if (input % 7 == 0) {
                next_state = STATE_COMPLETING;
            } else if (input % 13 == 0) {
                next_state = STATE_ERROR;
            } else if (input > 900) {
                next_state = STATE_WAITING;
            }
            break;
            
        case STATE_WAITING:
            if (input % 3 == 0) {
                next_state = STATE_PROCESSING;
            } else if (input > 950) {
                next_state = STATE_RESET;
            }
            break;
            
        case STATE_COMPLETING:
            next_state = STATE_IDLE;
            machine->processed_count++;
            break;
            
        case STATE_ERROR:
            next_state = STATE_RESET;
            machine->error_count++;
            break;
            
        case STATE_RESET:
            next_state = STATE_IDLE;
            break;
            
        case STATE_CALIBRATING:
            machine->calibration_value = (input % 256) + 1;
            next_state = STATE_IDLE;
            break;
            
        case STATE_SHUTDOWN:
            break;
    }
    
    return next_state;
}

static uint32_t perform_cpu_work(uint32_t value, uint32_t calibration) {
    uint32_t result = value;
    
    for (uint32_t i = 0; i < calibration; i++) {
        result = ((result * 1103515245) + 12345) & 0x7FFFFFFF;
        result ^= (result >> 16);
        result = (result * 16807) % 2147483647;
    }
    
    return result;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    srand((unsigned int)time(NULL));
    uint32_t input_sequence = 1;
    
    printf("Starting state machine simulation...\n");
    
    while (machine.current_state != STATE_SHUTDOWN && machine.iteration_count < MAX_ITERATIONS) {
        uint32_t input_value;
        
        if (machine.iteration_count < 100) {
            input_value = (uint32_t)(rand() % 1000);
        } else {
            input_value = input_sequence;
            input_sequence = (input_sequence * 1664525 + 1013904223) & 0x7FFFFFFF;
        }
        
        system_state_t previous_state = machine.current_state;
        machine.current_state = process_state_transition(&machine, input_value);
        
        if (machine.current_state == STATE_PROCESSING || machine.current_state == STATE_CALIBRATING) {
            uint32_t work_result = perform_cpu_work(input_value, machine.calibration_value);
            if (work_result == 0) {
                machine.current_state = STATE_ERROR;
            }
        }
        
        if (machine.error_count >= 100) {
            machine.current_state = STATE_SHUTDOWN;
        }
        
        if (machine.processed_count >= 10000) {
            machine.current_state = STATE_SHUTDOWN;
        }
        
        machine.iteration_count++;
        
        if (machine.iteration_count % 100000 == 0) {
            printf("Iteration: %lu, State: %d, Processed: %u, Errors: %u\n",
                   machine.iteration_count, machine.current_state,
                   machine.processed_count, machine.error_count);
        }
    }
    
    printf("Simulation completed.\n");
    printf("Total iterations: %lu\n", machine.iteration_count);
    printf("Final state: %d\n", machine.current_state);
    printf("Total processed: %u\n", machine.processed_count);
    printf("Total errors: %u\n", machine.error_count);
    printf("Final calibration: %u\n", machine.calibration_value);
    
    return 0;
}