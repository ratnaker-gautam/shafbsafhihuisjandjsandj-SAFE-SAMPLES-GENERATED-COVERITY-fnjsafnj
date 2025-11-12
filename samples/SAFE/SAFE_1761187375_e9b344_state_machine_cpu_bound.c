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
            machine->error_count++;
            if (input % 2 == 0) {
                next_state = STATE_RESET;
            } else {
                next_state = STATE_SHUTDOWN;
            }
            break;
            
        case STATE_RESET:
            next_state = STATE_IDLE;
            machine->calibration_value = (input % 50) + 1;
            break;
            
        case STATE_CALIBRATING:
            machine->calibration_value = (input % 100) + 1;
            next_state = STATE_IDLE;
            break;
            
        case STATE_SHUTDOWN:
            break;
    }
    
    return next_state;
}

static uint32_t generate_input(uint64_t iteration, uint32_t calibration) {
    uint32_t base = (uint32_t)(iteration % 1000);
    uint32_t modulated = base * calibration;
    return (modulated % 997) + 1;
}

static void perform_cpu_work(uint32_t value) {
    uint64_t result = 0;
    for (uint32_t i = 1; i <= value % 100; i++) {
        result += (uint64_t)i * i;
        if (result > 1000000000ULL) {
            result %= 1000000000ULL;
        }
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting CPU-bound state machine simulation\n");
    
    while (machine.iteration_count < MAX_ITERATIONS && 
           machine.current_state != STATE_SHUTDOWN) {
        
        uint32_t input = generate_input(machine.iteration_count, 
                                      machine.calibration_value);
        
        system_state_t next_state = process_state_transition(&machine, input);
        
        perform_cpu_work(input);
        
        machine.current_state = next_state;
        machine.iteration_count++;
        
        if (machine.iteration_count % 100000 == 0) {
            printf("Progress: %lu iterations, %u processed, %u errors\n",
                   (unsigned long)machine.iteration_count,
                   machine.processed_count,
                   machine.error_count);
        }
    }
    
    printf("Final state: %u\n", (unsigned)machine.current_state);
    printf("Total iterations: %lu\n", (unsigned long)machine.iteration_count);
    printf("Processed items: %u\n", machine.processed_count);
    printf("Error count: %u\n", machine.error_count);
    printf("Final calibration: %u\n", machine.calibration_value);
    
    return 0;
}