//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef struct {
    int from_state;
    char input_char;
    int to_state;
} Transition;

typedef struct {
    int state_count;
    int initial_state;
    int final_states[MAX_STATES];
    int final_count;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

typedef struct {
    StateMachine* machine;
    int current_state;
} MachineInstance;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->state_count = 3;
    sm->initial_state = 0;
    sm->final_count = 1;
    sm->final_states[0] = 2;
    sm->transition_count = 4;
    
    sm->transitions[0] = (Transition){0, 'a', 1};
    sm->transitions[1] = (Transition){1, 'b', 2};
    sm->transitions[2] = (Transition){2, 'c', 0};
    sm->transitions[3] = (Transition){1, 'a', 1};
    
    return sm;
}

MachineInstance* create_machine_instance(StateMachine* machine) {
    if (machine == NULL) return NULL;
    
    MachineInstance* instance = malloc(sizeof(MachineInstance));
    if (instance == NULL) return NULL;
    
    instance->machine = machine;
    instance->current_state = machine->initial_state;
    return instance;
}

int process_input(MachineInstance* instance, char input) {
    if (instance == NULL || instance->machine == NULL) return -1;
    
    StateMachine* sm = instance->machine;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* t = &sm->transitions[i];
        if (t->from_state == instance->current_state && t->input_char == input) {
            instance->current_state = t->to_state;
            return 0;
        }
    }
    
    return -1;
}

int is_final_state(MachineInstance* instance) {
    if (instance == NULL || instance->machine == NULL) return 0;
    
    StateMachine* sm = instance->machine;
    for (int i = 0; i < sm->final_count; i++) {
        if (instance->current_state == sm->final_states[i]) {
            return 1;
        }
    }
    return 0;
}

void destroy_machine_instance(MachineInstance* instance) {
    if (instance != NULL) {
        free(instance);
    }
}

void destroy_state_machine(StateMachine* machine) {
    if (machine != NULL) {
        free(machine);
    }
}

int main() {
    char input_buffer[MAX_INPUT_LEN];
    
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    MachineInstance* instance = create_machine_instance(sm);
    if (instance == NULL) {
        fprintf(stderr, "Failed to create machine instance\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: a, b, c\n");
    printf("Initial state: %d\n", instance->current_state);
    printf("Final states: ");
    for (int i = 0; i < sm->final_count; i++) {
        printf("%d ", sm->final_states[i]);
    }
    printf("\n");
    
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_machine_instance(instance);
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    printf("Processing input: %s\n", input_buffer);
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input_buffer[i];
        
        if (current_char != 'a' && current_char != 'b' && current_char != 'c') {
            printf("Invalid input character: %c\n", current_char);
            continue;
        }
        
        int old_state = instance->current_state;
        int result = process_input(instance, current_char);
        
        if (result == 0) {
            printf("State transition: %d -> %d (input: %c)\n", 
                   old_state, instance->current_state, current_char);
        } else {
            printf("No valid transition from state %d with input %c\n", 
                   instance->current_state, current_char);
        }
    }
    
    printf("Final state: %d\n", instance->current_state);
    
    if (is_final_state(instance)) {
        printf("Input ACCEPTED - reached final state\n");
    } else {
        printf("Input REJECTED - not in final state\n");
    }
    
    destroy_machine_instance(instance);