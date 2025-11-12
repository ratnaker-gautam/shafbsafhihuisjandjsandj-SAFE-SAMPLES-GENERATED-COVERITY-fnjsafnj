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
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATE,
    STATE_FINALIZE
} State;

typedef struct {
    State current_state;
    uint64_t counter;
    uint32_t steps;
    uint8_t data[16];
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->steps = 0;
    memset(machine->data, 0, sizeof(machine->data));
}

State transition_function(State current, uint64_t counter, uint8_t *data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (current) {
        case STATE_IDLE:
            if (counter % 2 == 0) return STATE_PROCESSING;
            return STATE_WAITING;
            
        case STATE_PROCESSING:
            if (counter % 3 == 0) return STATE_VALIDATE;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_WAITING;
            
        case STATE_WAITING:
            if (counter % 5 == 0) return STATE_PROCESSING;
            if (counter % 11 == 0) return STATE_COMPLETE;
            return STATE_WAITING;
            
        case STATE_VALIDATE:
            if (counter % 13 == 0) return STATE_FINALIZE;
            return STATE_PROCESSING;
            
        case STATE_FINALIZE:
            return STATE_COMPLETE;
            
        case STATE_COMPLETE:
            return STATE_RESET;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            return STATE_IDLE;
            
        default:
            return STATE_ERROR;
    }
}

void update_data(uint8_t *data, uint64_t counter) {
    if (data == NULL) return;
    
    for (size_t i = 0; i < 16; i++) {
        uint8_t val = (uint8_t)((counter + i * 17) % 256);
        data[i] = val ^ data[i];
    }
}

int simulate_state_machine(StateMachine *machine, uint32_t max_steps) {
    if (machine == NULL || max_steps == 0 || max_steps > MAX_STEPS) {
        return -1;
    }
    
    while (machine->steps < max_steps) {
        if (machine->current_state == STATE_COMPLETE) {
            break;
        }
        
        State next_state = transition_function(machine->current_state, 
                                             machine->counter, 
                                             machine->data);
        
        update_data(machine->data, machine->counter);
        
        machine->current_state = next_state;
        machine->counter++;
        machine->steps++;
        
        if (machine->counter == UINT64_MAX) {
            machine->current_state = STATE_ERROR;
            break;
        }
    }
    
    return 0;
}

int main(void) {
    StateMachine machine;
    uint32_t simulation_steps;
    
    printf("Enter number of simulation steps (1-1000000): ");
    if (scanf("%u", &simulation_steps) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (simulation_steps == 0 || simulation_steps > MAX_STEPS) {
        fprintf(stderr, "Steps out of range\n");
        return 1;
    }
    
    initialize_machine(&machine);
    
    clock_t start_time = clock();
    if (start_time == (clock_t)-1) {
        fprintf(stderr, "Clock error\n");
        return 1;
    }
    
    int result = simulate_state_machine(&machine, simulation_steps);
    if (result != 0) {
        fprintf(stderr, "Simulation failed\n");
        return 1;
    }
    
    clock_t end_time = clock();
    if (end_time == (clock_t)-1) {
        fprintf(stderr, "Clock error\n");
        return 1;
    }
    
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Simulation completed in %u steps\n", machine.steps);
    printf("Final state: %d\n", machine.current_state);
    printf("Final counter: %lu\n", machine.counter);
    printf("CPU time: %.6f seconds\n", cpu_time);
    
    return 0;
}