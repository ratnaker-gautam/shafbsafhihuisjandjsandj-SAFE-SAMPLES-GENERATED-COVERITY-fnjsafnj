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
    uint32_t iterations;
    uint8_t data_buffer[64];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
}

static system_state_t state_transition(state_machine_t *machine) {
    uint32_t local_iter = machine->iterations;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (local_iter < 10) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
            
        case STATE_PROCESSING:
            if (local_iter < 100) {
                machine->counter += local_iter * 2;
                return STATE_WAITING;
            }
            return STATE_ERROR;
            
        case STATE_WAITING:
            if (local_iter % 3 == 0) {
                return STATE_VALIDATE;
            }
            return STATE_PROCESSING;
            
        case STATE_VALIDATE:
            if (machine->counter > 1000 && local_iter < 50) {
                return STATE_COMPLETE;
            }
            return STATE_RESET;
            
        case STATE_COMPLETE:
            return STATE_FINALIZE;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            machine->counter = 0;
            return STATE_IDLE;
            
        case STATE_FINALIZE:
            return STATE_FINALIZE;
            
        default:
            return STATE_ERROR;
    }
}

static void process_state(state_machine_t *machine) {
    uint32_t i;
    
    switch (machine->current_state) {
        case STATE_PROCESSING:
            for (i = 0; i < 16; i++) {
                if (i < sizeof(machine->data_buffer)) {
                    machine->data_buffer[i] = (uint8_t)((machine->counter + i) & 0xFF);
                }
            }
            break;
            
        case STATE_VALIDATE:
            for (i = 0; i < 8; i++) {
                if (i < sizeof(machine->data_buffer)) {
                    machine->data_buffer[i] ^= 0xAA;
                }
            }
            break;
            
        case STATE_FINALIZE:
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] = 0xFF;
            }
            break;
            
        default:
            break;
    }
}

int main(void) {
    state_machine_t machine;
    uint32_t step_count = 0;
    
    initialize_machine(&machine);
    
    srand((unsigned int)time(NULL));
    machine.iterations = (uint32_t)(rand() % 150) + 1;
    
    if (machine.iterations == 0 || machine.iterations > MAX_STEPS) {
        machine.iterations = 100;
    }
    
    printf("Starting state machine with %u iterations\n", machine.iterations);
    
    while (step_count < MAX_STEPS) {
        process_state(&machine);
        
        system_state_t next_state = state_transition(&machine);
        
        if (next_state == machine.current_state && next_state == STATE_FINALIZE) {
            break;
        }
        
        machine.current_state = next_state;
        step_count++;
        
        if (step_count >= machine.iterations) {
            machine.current_state = STATE_FINALIZE;
        }
    }
    
    if (step_count >= MAX_STEPS) {
        printf("State machine reached maximum steps\n");
        return 1;
    }
    
    printf("State machine completed in %u steps\n", step_count);
    printf("Final counter value: %lu\n", machine.counter);
    
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < sizeof(machine.data_buffer); i++) {
        checksum += machine.data_buffer[i];
    }
    printf("Data buffer checksum: %u\n", checksum);
    
    return 0;
}