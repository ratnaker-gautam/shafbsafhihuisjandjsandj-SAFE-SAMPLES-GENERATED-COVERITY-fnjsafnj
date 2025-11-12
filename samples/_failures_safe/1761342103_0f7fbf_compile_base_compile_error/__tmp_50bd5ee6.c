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
            } else {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (input % 7 == 0) {
                next_state = STATE_COMPLETING;
            } else if (input % 13 == 0) {
                next_state = STATE_WAITING;
            } else if (input > 1000) {
                next_state = STATE_ERROR;
            } else {
                next_state = STATE_PROCESSING;
            }
            break;
            
        case STATE_WAITING:
            if (input % 3 == 0) {
                next_state = STATE_PROCESSING;
            } else if (input > 500) {
                next_state = STATE_RESET;
            } else {
                next_state = STATE_WAITING;
            }
            break;
            
        case STATE_COMPLETING:
            next_state = STATE_IDLE;
            break;
            
        case STATE_ERROR:
            if (input % 2 == 0) {
                next_state = STATE_RESET;
            } else {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_RESET:
            next_state = STATE_IDLE;
            break;
            
        case STATE_CALIBRATING:
            if (input < 50) {
                next_state = STATE_IDLE;
            } else {
                next_state = STATE_CALIBRATING;
            }
            break;
            
        case STATE_SHUTDOWN:
            next_state = STATE_SHUTDOWN;
            break;
            
        default:
            next_state = STATE_ERROR;
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
            machine->processed_count += 2;
            break;
            
        case STATE_ERROR:
            machine->error_count++;
            break;
            
        case STATE_CALIBRATING:
            machine->calibration_value = (machine->calibration_value + input) % 256;
            break;
            
        case STATE_RESET:
            machine->error_count = 0;
            machine->calibration_value = 1;
            break;
            
        default:
            break;
    }
}

static void simulate_state_machine(state_machine_t *machine) {
    uint32_t input_sequence = 42;
    
    while (machine->iteration_count < MAX_ITERATIONS && machine->current_state != STATE_SHUTDOWN) {
        input_sequence = (input_sequence * 1103515245 + 12345) % 2147483647;
        uint32_t current_input = input_sequence % 1500;
        
        system_state_t next_state = process_state_transition(machine, current_input);
        
        if (next_state >= STATE_IDLE && next_state <= STATE_SHUTDOWN) {
            machine->current_state = next_state;
        } else {
            machine->current_state = STATE_ERROR;
        }
        
        execute_state_action(machine, current_input);
        
        machine->iteration_count++;
        
        if (machine->error_count > 1000) {
            machine->current_state = STATE_SHUTDOWN;
        }
        
        if (machine->iteration_count % 100000 == 0) {
            if (machine->iteration_count + 100000 > MAX_ITERATIONS) {
                break;
            }
        }
    }
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine);
    
    clock_t start_time = clock();
    if (start_time == (clock_t)-1) {
        return EXIT_FAILURE;
    }
    
    simulate_state_machine(&machine);
    
    clock_t end_time = clock();
    if (end_time == (clock_t)-1) {
        return EXIT_FAILURE;
    }
    
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("State machine simulation completed\n");
    printf("Final state: %u\n", (unsigned int)machine.current_state);
    printf("Iter