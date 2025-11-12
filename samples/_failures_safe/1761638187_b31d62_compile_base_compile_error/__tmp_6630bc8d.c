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
    State state;
    char name[32];
    int data;
} StateInfo;

typedef struct {
    StateInfo *states_heap;
    Transition *transitions_heap;
    int state_count;
    int transition_count;
    char buffer_stack[BUFFER_SIZE];
} StateMachine;

int validate_state(State state) {
    return state >= STATE_IDLE && state <= STATE_DONE;
}

int add_state(StateMachine *sm, State state, const char *name, int data) {
    if (sm == NULL || name == NULL || !validate_state(state)) return 0;
    if (sm->state_count >= MAX_STATES) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states_heap[i].state == state) return 0;
    }
    
    if (strlen(name) >= 32) return 0;
    
    sm->states_heap[sm->state_count].state = state;
    strncpy(sm->states_heap[sm->state_count].name, name, 31);
    sm->states_heap[sm->state_count].name[31] = '\0';
    sm->states_heap[sm->state_count].data = data;
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine *sm, State from, State to, const char *condition) {
    if (sm == NULL || condition == NULL || !validate_state(from) || !validate_state(to)) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    if (strlen(condition) >= 32) return 0;
    
    sm->transitions_heap[sm->transition_count].current = from;
    sm->transitions_heap[sm->transition_count].next = to;
    strncpy(sm->transitions_heap[sm->transition_count].condition, condition, 31);
    sm->transitions_heap[sm->transition_count].condition[31] = '\0';
    sm->transition_count++;
    return 1;
}

State process_transition(StateMachine *sm, State current, const char *input) {
    if (sm == NULL || input == NULL || !validate_state(current)) return current;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions_heap[i].current == current && 
            strcmp(sm->transitions_heap[i].condition, input) == 0) {
            return sm->transitions_heap[i].next;
        }
    }
    return current;
}

void print_state_info(StateMachine *sm, State state) {
    if (sm == NULL || !validate_state(state)) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states_heap[i].state == state) {
            printf("Current state: %s (data: %d)\n", 
                   sm->states_heap[i].name, sm->states_heap[i].data);
            return;
        }
    }
}

int main() {
    StateMachine sm;
    sm.state_count = 0;
    sm.transition_count = 0;
    
    sm.states_heap = malloc(MAX_STATES * sizeof(StateInfo));
    sm.transitions_heap = malloc(MAX_TRANSITIONS * sizeof(Transition));
    
    if (sm.states_heap == NULL || sm.transitions_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        if (sm.states_heap) free(sm.states_heap);
        if (sm.transitions_heap) free(sm.transitions_heap);
        return 1;
    }
    
    add_state(&sm, STATE_IDLE, "Idle", 0);
    add_state(&sm, STATE_ACTIVE, "Active", 100);
    add_state(&sm, STATE_PAUSED, "Paused", 50);
    add_state(&sm, STATE_ERROR, "Error", -1);
    add_state(&sm, STATE_DONE, "Done", 999);
    
    add_transition(&sm, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(&sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(&sm, STATE_ACTIVE, STATE_DONE, "complete");
    add_transition(&sm, STATE_ACTIVE, STATE_ERROR, "error");
    add_transition(&sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(&sm, STATE_PAUSED, STATE_ERROR, "error");
    add_transition(&sm, STATE_ERROR, STATE_IDLE, "reset");
    
    State current_state = STATE_IDLE;
    
    printf("State Machine Simulation\n");
    printf("Available commands: start, pause, resume, complete, error, reset, quit\n");
    
    while (1) {
        print_state_info(&sm, current_state);
        printf("Enter command: ");
        
        if (fgets(sm.buffer_stack, BUFFER_SIZE, stdin) == NULL) {
            break