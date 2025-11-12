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
    char* heap_buffer;
    char stack_buffer[BUFFER_SIZE];
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 0;
    sm->transition_count = 0;
    
    sm->heap_buffer = malloc(BUFFER_SIZE);
    if (!sm->heap_buffer) {
        free(sm);
        return NULL;
    }
    
    memset(sm->stack_buffer, 0, BUFFER_SIZE);
    memset(sm->heap_buffer, 0, BUFFER_SIZE);
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        free(sm->heap_buffer);
        free(sm);
    }
}

int add_state(StateMachine* sm, State state) {
    if (!sm || sm->state_count >= MAX_STATES) return 0;
    sm->states[sm->state_count++] = state;
    return 1;
}

int add_transition(StateMachine* sm, State from, State to, const char* condition) {
    if (!sm || !condition || sm->transition_count >= MAX_TRANSITIONS) return 0;
    if (strlen(condition) >= 32) return 0;
    
    Transition* t = &sm->transitions[sm->transition_count++];
    t->current = from;
    t->next = to;
    strncpy(t->condition, condition, 31);
    t->condition[31] = '\0';
    
    return 1;
}

State process_input(StateMachine* sm, State current, const char* input) {
    if (!sm || !input) return current;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* t = &sm->transitions[i];
        if (t->current == current && strcmp(t->condition, input) == 0) {
            return t->next;
        }
    }
    return current;
}

int main() {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, STATE_IDLE);
    add_state(sm, STATE_ACTIVE);
    add_state(sm, STATE_PAUSED);
    add_state(sm, STATE_ERROR);
    add_state(sm, STATE_DONE);
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(sm, STATE_ACTIVE, STATE_DONE, "complete");
    add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error");
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(sm, STATE_PAUSED, STATE_ERROR, "error");
    add_transition(sm, STATE_ERROR, STATE_IDLE, "reset");
    
    State current = STATE_IDLE;
    char input[64];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, complete, error, reset, quit\n");
    
    while (1) {
        printf("Current state: ");
        switch (current) {
            case STATE_IDLE: printf("IDLE\n"); break;
            case STATE_ACTIVE: printf("ACTIVE\n"); break;
            case STATE_PAUSED: printf("PAUSED\n"); break;
            case STATE_ERROR: printf("ERROR\n"); break;
            case STATE_DONE: printf("DONE\n"); break;
        }
        
        printf("Enter command: ");
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        State new_state = process_input(sm, current, input);
        if (new_state != current) {
            printf("Transition: ");
            switch (current) {
                case STATE_IDLE: printf("IDLE"); break;
                case STATE_ACTIVE: printf("ACTIVE"); break;
                case STATE_PAUSED: printf("PAUSED"); break;
                case STATE_ERROR: printf("ERROR"); break;
                case STATE_DONE: printf("DONE"); break;
            }
            printf(" -> ");
            switch (new_state) {
                case STATE_IDLE: printf("IDLE\n"); break;
                case STATE_ACTIVE: printf("ACTIVE\n"); break;
                case STATE_PAUSED: printf("PAUSED\n"); break;
                case STATE_ERROR: printf("ERROR\n"); break;
                case STATE_DONE: printf("DONE\n"); break;
            }