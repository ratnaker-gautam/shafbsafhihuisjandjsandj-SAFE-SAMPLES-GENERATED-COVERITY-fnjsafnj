//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef struct {
    int id;
    char name[32];
} State;

typedef struct {
    int from_state;
    int to_state;
    char input;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int current_state;
    int state_count;
    int transition_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = -1;
    return sm;
}

int add_state(StateMachine* sm, int id, const char* name) {
    if (sm == NULL || sm->state_count >= MAX_STATES) {
        return 0;
    }
    if (id < 0 || name == NULL || strlen(name) >= 32) {
        return 0;
    }
    sm->states[sm->state_count].id = id;
    strncpy(sm->states[sm->state_count].name, name, 31);
    sm->states[sm->state_count].name[31] = '\0';
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char input) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from < 0 || to < 0) {
        return 0;
    }
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].input = input;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (sm == NULL || sm->current_state < 0) {
        return 0;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void print_current_state(StateMachine* sm) {
    if (sm == NULL || sm->current_state < 0) {
        printf("No current state\n");
        return;
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->states[i].id);
            return;
        }
    }
    printf("Invalid current state\n");
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    int stack_states[3] = {0, 1, 2};
    char* state_names[] = {"START", "PROCESSING", "END"};
    
    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, stack_states[i], state_names[i])) {
            fprintf(stderr, "Failed to add state\n");
            free(sm);
            return 1;
        }
    }

    if (!add_transition(sm, 0, 1, 'a') ||
        !add_transition(sm, 1, 2, 'b') ||
        !add_transition(sm, 1, 1, 'c') ||
        !add_transition(sm, 2, 0, 'd')) {
        fprintf(stderr, "Failed to add transition\n");
        free(sm);
        return 1;
    }

    sm->current_state = 0;
    
    printf("State Machine Ready. Enter input characters (a,b,c,d) or 'q' to quit:\n");
    
    char input_buffer[MAX_INPUT_LEN];
    while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if (input_buffer[0] == 'q' && (input_buffer[1] == '\n' || input_buffer[1] == '\0')) {
            break;
        }
        
        for (int i = 0; i < (int)strlen(input_buffer) && input_buffer[i] != '\n'; i++) {
            char input_char = input_buffer[i];
            if (input_char == 'a' || input_char == 'b' || input_char == 'c' || input_char == 'd') {
                if (process_input(sm, input_char)) {
                    printf("Processed '%c': ", input_char);
                    print_current_state(sm);
                } else {
                    printf("No transition for '%c' in current state\n", input_char);
                }
            } else if (input_char != '\n') {
                printf("Invalid input character: '%c'\n", input_char);
            }
        }
        
        printf("Enter next input: ");
    }
    
    free(sm);
    return 0;
}