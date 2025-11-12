//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 16
#define MAX_TRANSITIONS 64

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} system_state_t;

typedef struct {
    system_state_t from;
    system_state_t to;
    int trigger;
} transition_t;

typedef struct {
    system_state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
    uint64_t cycle_count;
    int error_code;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->transition_count = 0;
    machine->cycle_count = 0;
    machine->error_code = 0;
    
    transition_t transitions[] = {
        {STATE_IDLE, STATE_READY, 1},
        {STATE_READY, STATE_PROCESSING, 2},
        {STATE_PROCESSING, STATE_COMPLETE, 3},
        {STATE_PROCESSING, STATE_ERROR, 4},
        {STATE_COMPLETE, STATE_IDLE, 5},
        {STATE_ERROR, STATE_IDLE, 6}
    };
    
    int count = sizeof(transitions) / sizeof(transitions[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = transitions[i];
            machine->transition_count++;
        }
    }
}

int process_transition(state_machine_t *machine, int trigger) {
    if (machine == NULL) return 0;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].from == machine->current_state && 
            machine->transitions[i].trigger == trigger) {
            machine->current_state = machine->transitions[i].to;
            return 1;
        }
    }
    return 0;
}

void execute_state_action(state_machine_t *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->cycle_count = 0;
            break;
        case STATE_READY:
            machine->cycle_count++;
            break;
        case STATE_PROCESSING:
            for (int i = 0; i < 1000; i++) {
                machine->cycle_count += i * i;
                if (machine->cycle_count > UINT64_MAX / 2) {
                    machine->cycle_count = 0;
                }
            }
            break;
        case STATE_COMPLETE:
            machine->cycle_count += 100;
            break;
        case STATE_ERROR:
            machine->error_code = (int)(machine->cycle_count % 256);
            break;
    }
}

void print_state_info(const state_machine_t *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    int state_index = machine->current_state;
    if (state_index < 0 || state_index >= 5) state_index = 0;
    
    printf("State: %s, Cycles: %lu", state_names[state_index], machine->cycle_count);
    if (machine->current_state == STATE_ERROR) {
        printf(", Error: %d", machine->error_code);
    }
    printf("\n");
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    int max_iterations = 50;
    int iteration = 0;
    
    while (iteration < max_iterations) {
        print_state_info(&machine);
        
        int trigger;
        switch (machine.current_state) {
            case STATE_IDLE:
                trigger = 1;
                break;
            case STATE_READY:
                trigger = 2;
                break;
            case STATE_PROCESSING:
                trigger = (iteration % 10 == 0) ? 4 : 3;
                break;
            case STATE_COMPLETE:
                trigger = 5;
                break;
            case STATE_ERROR:
                trigger = 6;
                break;
        }
        
        if (!process_transition(&machine, trigger)) {
            printf("Invalid transition attempted\n");
            break;
        }
        
        execute_state_action(&machine);
        iteration++;
        
        if (machine.current_state == STATE_IDLE && iteration > 10) {
            break;
        }
    }
    
    printf("Final state after %d iterations:\n", iteration);
    print_state_info(&machine);
    
    return 0;
}