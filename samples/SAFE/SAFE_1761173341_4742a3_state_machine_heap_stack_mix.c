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
    char trigger[32];
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = STATE_IDLE;
    
    sm->states[sm->state_count++] = STATE_IDLE;
    sm->states[sm->state_count++] = STATE_ACTIVE;
    sm->states[sm->state_count++] = STATE_PAUSED;
    sm->states[sm->state_count++] = STATE_ERROR;
    sm->states[sm->state_count++] = STATE_DONE;
    
    strncpy(sm->transitions[0].trigger, "start", 31);
    sm->transitions[0].current = STATE_IDLE;
    sm->transitions[0].next = STATE_ACTIVE;
    sm->transition_count++;
    
    strncpy(sm->transitions[1].trigger, "pause", 31);
    sm->transitions[1].current = STATE_ACTIVE;
    sm->transitions[1].next = STATE_PAUSED;
    sm->transition_count++;
    
    strncpy(sm->transitions[2].trigger, "resume", 31);
    sm->transitions[2].current = STATE_PAUSED;
    sm->transitions[2].next = STATE_ACTIVE;
    sm->transition_count++;
    
    strncpy(sm->transitions[3].trigger, "error", 31);
    sm->transitions[3].current = STATE_ACTIVE;
    sm->transitions[3].next = STATE_ERROR;
    sm->transition_count++;
    
    strncpy(sm->transitions[4].trigger, "finish", 31);
    sm->transitions[4].current = STATE_ACTIVE;
    sm->transitions[4].next = STATE_DONE;
    sm->transition_count++;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) free(sm);
}

const char* state_to_string(State s) {
    switch(s) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int process_transition(StateMachine* sm, const char* trigger) {
    if (!sm || !trigger) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (strncmp(sm->transitions[i].trigger, trigger, 31) == 0 &&
            sm->transitions[i].current == sm->current_state) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

int main() {
    StateMachine* machine = create_state_machine();
    if (!machine) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    
    printf("State Machine Controller\n");
    printf("Available triggers: start, pause, resume, error, finish\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(machine->current_state));
        printf("Enter trigger: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strncmp(input, "quit", 4) == 0) {
            break;
        }
        
        if (process_transition(machine, input)) {
            printf("Transition successful\n");
        } else {
            printf("Invalid transition from current state\n");
        }
        
        printf("\n");
        
        if (machine->current_state == STATE_DONE) {
            printf("Process completed successfully\n");
            break;
        }
        
        if (machine->current_state == STATE_ERROR) {
            printf("Process ended with error\n");
            break;
        }
    }
    
    destroy_state_machine(machine);
    return 0;
}