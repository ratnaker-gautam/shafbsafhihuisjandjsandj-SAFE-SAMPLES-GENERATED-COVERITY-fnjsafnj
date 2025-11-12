//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 16
#define MAX_TRANSITIONS 64

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current;
    char trigger;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    uint32_t data_value;
    uint32_t processed_count;
    transition_t transitions[MAX_TRANSITIONS];
    int num_transitions;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->data_value = 0;
    machine->processed_count = 0;
    machine->num_transitions = 0;
    
    transition_t trans[] = {
        {STATE_IDLE, 'R', STATE_READING},
        {STATE_READING, 'P', STATE_PROCESSING},
        {STATE_PROCESSING, 'W', STATE_WRITING},
        {STATE_PROCESSING, 'E', STATE_ERROR},
        {STATE_WRITING, 'I', STATE_IDLE},
        {STATE_ERROR, 'R', STATE_READING}
    };
    
    int count = sizeof(trans) / sizeof(trans[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        machine->transitions[i] = trans[i];
    }
    machine->num_transitions = count;
}

int find_transition(state_machine_t *machine, char trigger) {
    for (int i = 0; i < machine->num_transitions; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].trigger == trigger) {
            return i;
        }
    }
    return -1;
}

uint32_t process_data(uint32_t input) {
    uint32_t result = input;
    for (int i = 0; i < 1000; i++) {
        result = (result * 1103515245 + 12345) & 0x7FFFFFFF;
        result ^= (result >> 16);
        result = (result * 16807) & 0x7FFFFFFF;
    }
    return result;
}

int execute_state_action(state_machine_t *machine, char input) {
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->data_value = 0;
            break;
            
        case STATE_READING:
            if (input >= '0' && input <= '9') {
                uint32_t digit = input - '0';
                if (machine->data_value <= (UINT32_MAX - digit) / 10) {
                    machine->data_value = machine->data_value * 10 + digit;
                } else {
                    return -1;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (machine->processed_count < UINT32_MAX) {
                machine->data_value = process_data(machine->data_value);
                machine->processed_count++;
            }
            break;
            
        case STATE_WRITING:
            printf("Output: %u\n", machine->data_value);
            break;
            
        case STATE_ERROR:
            printf("Error state encountered\n");
            machine->data_value = 0;
            break;
    }
    return 0;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("State Machine Controller\n");
    printf("Commands: R(ead), P(rocess), W(rite), I(dle), E(rror), Q(uit)\n");
    
    char input;
    while (1) {
        printf("Current state: ");
        switch (machine.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Data: %u | Processed: %u\n", machine.data_value, machine.processed_count);
        
        printf("Enter command: ");
        int result = scanf(" %c", &input);
        if (result != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 'Q' || input == 'q') {
            break;
        }
        
        int trans_index = find_transition(&machine, input);
        if (trans_index == -1) {
            printf("Invalid transition from current state\n");
            continue;
        }
        
        if (execute_state_action(&machine, input) != 0) {
            printf("State action failed\n");
            continue;
        }
        
        machine.current_state = machine.transitions[trans_index].next;
    }
    
    printf("Final state: ");
    switch (machine.current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_WRITING: printf("WRITING"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf(" | Total processed: %u