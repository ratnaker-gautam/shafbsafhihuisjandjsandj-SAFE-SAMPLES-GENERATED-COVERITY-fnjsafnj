//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    State next;
    char trigger;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    char *name;
} StateMachine;

StateMachine* create_state_machine(const char *name) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->name = malloc(strlen(name) + 1);
    if (!sm->name) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm->name);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->name);
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, char trigger) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return sm->current_state;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main() {
    char buffer[BUFFER_SIZE];
    StateMachine *machine = create_state_machine("DemoMachine");
    
    if (!machine) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_transition(machine, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(machine, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(machine, STATE_ACTIVE, STATE_DONE, 'f');
    add_transition(machine, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(machine, STATE_PAUSED, STATE_ERROR, 'e');
    add_transition(machine, STATE_ERROR, STATE_IDLE, 'r');
    
    printf("State Machine: %s\n", machine->name);
    printf("Current state: %s\n", state_to_string(machine->current_state));
    printf("Available triggers: s(start), p(pause), r(reset), f(finish), e(error)\n");
    printf("Enter 'q' to quit\n");
    
    while (1) {
        printf("\nEnter trigger: ");
        if (!fgets(buffer, BUFFER_SIZE, stdin)) break;
        
        if (buffer[0] == 'q' && buffer[1] == '\n') break;
        
        if (strlen(buffer) != 2 || buffer[1] != '\n') {
            printf("Invalid input. Enter single character followed by newline.\n");
            continue;
        }
        
        State new_state = process_trigger(machine, buffer[0]);
        printf("State changed to: %s\n", state_to_string(new_state));
        
        if (new_state == STATE_DONE) {
            printf("Process completed successfully.\n");
            break;
        }
    }
    
    destroy_state_machine(machine);
    return 0;
}