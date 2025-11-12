//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    char condition[32];
} Transition;

typedef struct {
    State *states;
    Transition *transitions;
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(MAX_STATES * sizeof(State));
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    
    if (!sm->states || !sm->transitions) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states[0] = STATE_IDLE;
    sm->states[1] = STATE_ACTIVE;
    sm->states[2] = STATE_PAUSED;
    sm->states[3] = STATE_ERROR;
    sm->states[4] = STATE_DONE;
    
    sm->transition_count = 6;
    sm->transitions[0] = (Transition){STATE_IDLE, STATE_ACTIVE, "start"};
    sm->transitions[1] = (Transition){STATE_ACTIVE, STATE_PAUSED, "pause"};
    sm->transitions[2] = (Transition){STATE_ACTIVE, STATE_DONE, "complete"};
    sm->transitions[3] = (Transition){STATE_ACTIVE, STATE_ERROR, "error"};
    sm->transitions[4] = (Transition){STATE_PAUSED, STATE_ACTIVE, "resume"};
    sm->transitions[5] = (Transition){STATE_ERROR, STATE_IDLE, "reset"};
    
    sm->current_state = STATE_IDLE;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int process_transition(StateMachine *sm, const char *condition) {
    if (!sm || !condition) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state &&
            strcmp(sm->transitions[i].condition, condition) == 0) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
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
    StateMachine *machine = create_state_machine();
    if (!machine) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    int step = 0;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, complete, error, reset, quit\n");
    
    while (step < 20) {
        printf("\nCurrent state: %s\n", state_to_string(machine->current_state));
        printf("Enter command: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (process_transition(machine, input)) {
            printf("Transition successful to: %s\n", state_to_string(machine->current_state));
        } else {
            printf("Invalid transition from current state\n");
        }
        
        step++;
        
        if (machine->current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    destroy_state_machine(machine);
    return 0;
}