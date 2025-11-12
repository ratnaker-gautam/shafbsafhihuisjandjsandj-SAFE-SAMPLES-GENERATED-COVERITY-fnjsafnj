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
    uint32_t success_count;
    double accumulator;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->iteration_count = 0;
    machine->error_count = 0;
    machine->success_count = 0;
    machine->accumulator = 0.0;
}

static system_state_t process_state_transition(state_machine_t *machine) {
    double computation;
    uint32_t decision;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            computation = (double)(machine->iteration_count % 1000) * 0.001;
            machine->accumulator += computation * computation;
            decision = (uint32_t)(machine->accumulator * 1000.0) % STATE_COUNT;
            return (system_state_t)decision;
            
        case STATE_PROCESSING:
            for (int i = 0; i < 100; i++) {
                computation = (double)i * 0.01;
                machine->accumulator += computation * computation;
            }
            if (machine->accumulator > 1000.0) {
                return STATE_COMPLETING;
            }
            return STATE_PROCESSING;
            
        case STATE_WAITING:
            machine->accumulator += 0.1;
            if (machine->accumulator > 50.0) {
                return STATE_PROCESSING;
            }
            return STATE_WAITING;
            
        case STATE_COMPLETING:
            machine->success_count++;
            machine->accumulator = 0.0;
            return STATE_IDLE;
            
        case STATE_ERROR:
            machine->error_count++;
            if (machine->error_count > 10) {
                return STATE_SHUTDOWN;
            }
            return STATE_RESET;
            
        case STATE_RESET:
            machine->accumulator = 0.0;
            return STATE_IDLE;
            
        case STATE_CALIBRATING:
            for (int i = 0; i < 50; i++) {
                computation = (double)(i * i) * 0.0001;
                machine->accumulator += computation;
            }
            if (machine->accumulator > 10.0) {
                return STATE_IDLE;
            }
            return STATE_CALIBRATING;
            
        case STATE_SHUTDOWN:
        default:
            return STATE_SHUTDOWN;
    }
}

static void execute_state_operations(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_PROCESSING:
            for (int i = 0; i < 1000; i++) {
                double value = (double)i * 0.001;
                machine->accumulator += value * value * value;
            }
            break;
            
        case STATE_CALIBRATING:
            for (int i = 0; i < 500; i++) {
                double calc = (double)(i % 100) * 0.01;
                machine->accumulator += calc * calc;
            }
            break;
            
        case STATE_IDLE:
            for (int i = 0; i < 100; i++) {
                machine->accumulator += (double)i * 0.0001;
            }
            break;
            
        default:
            break;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    while (machine.iteration_count < MAX_ITERATIONS && 
           machine.current_state != STATE_SHUTDOWN) {
        
        execute_state_operations(&machine);
        
        system_state_t next_state = process_state_transition(&machine);
        
        if (next_state >= STATE_COUNT) {
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = next_state;
        }
        
        machine.iteration_count++;
        
        if (machine.iteration_count % 100000 == 0) {
            printf("Iteration: %lu, State: %d, Success: %u, Errors: %u\n",
                   machine.iteration_count, machine.current_state,
                   machine.success_count, machine.error_count);
        }
    }
    
    printf("Final results - Iterations: %lu, Success: %u, Errors: %u, Final State: %d\n",
           machine.iteration_count, machine.success_count, 
           machine.error_count, machine.current_state);
    
    return 0;
}