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
    
    return sm;
}

void add_transition(StateMachine* sm, State from, State to, const char* condition) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return;
    if (strlen(condition) >= 32) return;
    
    Transition* t = &sm->transitions[sm->transition_count++];
    t->current = from;
    t->next = to;
    strncpy(t->condition, condition, 31);
    t->condition[31] = '\0';
}

int process_transition(StateMachine* sm, const char* input) {
    if (!sm || !input) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* t = &sm->transitions[i];
        if (t->current == sm->current_state && 
            strcmp(t->condition, input) == 0) {
            sm->current_state = t->next;
            return 1;
        }
    }
    return 0;
}

void print_state(State state) {
    switch (state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_ACTIVE: printf("ACTIVE"); break;
        case STATE_PAUSED: printf("PAUSED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        case STATE_DONE: printf("DONE"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main() {
    StateMachine* machine = create_state_machine();
    if (!machine) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(machine, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(machine, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(machine, STATE_ACTIVE, STATE_DONE, "complete");
    add_transition(machine, STATE_ACTIVE, STATE_ERROR, "error");
    add_transition(machine, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(machine, STATE_PAUSED, STATE_ERROR, "error");
    add_transition(machine, STATE_ERROR, STATE_IDLE, "reset");
    
    char input_buffer[BUFFER_SIZE];
    
    printf("State Machine Controller\n");
    printf("Available commands: start, pause, resume, complete, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: ");
        print_state(machine->current_state);
        printf("\nEnter command: ");
        
        if (!fgets(input_buffer, BUFFER_SIZE, stdin)) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (!process_transition(machine, input_buffer)) {
            printf("Invalid transition from current state\n");
        }
        
        if (machine->current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    free(machine);
    return 0;
}