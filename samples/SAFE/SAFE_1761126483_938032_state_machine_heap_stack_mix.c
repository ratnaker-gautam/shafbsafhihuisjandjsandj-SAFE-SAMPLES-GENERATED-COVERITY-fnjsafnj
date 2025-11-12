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

int process_input(MachineInstance* instance, const char* input) {
    if (instance == NULL || input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char current_char = input[i];
        int transition_found = 0;
        
        for (int j = 0; j < instance->machine->transition_count; j++) {
            Transition t = instance->machine->transitions[j];
            if (t.from_state == instance->current_state && t.input_char == current_char) {
                instance->current_state = t.to_state;
                transition_found = 1;
                break;
            }
        }
        
        if (!transition_found) {
            return 0;
        }
    }
    
    for (int i = 0; i < instance->machine->final_count; i++) {
        if (instance->current_state == instance->machine->final_states[i]) {
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
    char input[MAX_INPUT_LEN + 1];
    StateMachine* machine = create_state_machine();
    
    if (machine == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Simulator\n");
    printf("Valid transitions: 0->a->1, 1->b->2, 2->c->0, 1->a->1\n");
    printf("Final state: 2\n");
    printf("Enter input string (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(machine);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    MachineInstance* instance = create_machine_instance(machine);
    if (instance == NULL) {
        fprintf(stderr, "Failed to create machine instance\n");
        destroy_state_machine(machine);
        return 1;
    }
    
    int result = process_input(instance, input);
    
    printf("Input: %s\n", input);
    printf("Final state: %d\n", instance->current_state);
    printf("Accepted: %s\n", result ? "YES" : "NO");
    
    destroy_machine_instance(instance);
    destroy_state_machine(machine);
    
    return 0;
}