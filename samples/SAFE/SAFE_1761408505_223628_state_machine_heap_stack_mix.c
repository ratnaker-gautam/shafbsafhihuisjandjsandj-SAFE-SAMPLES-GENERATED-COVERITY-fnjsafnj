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
    STATE_COMPLETED,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    State next_state;
    char trigger[32];
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    char* name;
} StateMachine;

StateMachine* create_state_machine(const char* name) {
    if (name == NULL || strlen(name) == 0) return NULL;
    
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->name = malloc(strlen(name) + 1);
    if (sm->name == NULL) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->state_count = 0;
    sm->transition_count = 0;
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    free(sm->name);
    free(sm);
}

int add_state(StateMachine* sm, State state) {
    if (sm == NULL || sm->state_count >= MAX_STATES) return 0;
    sm->states[sm->state_count++] = state;
    return 1;
}

int add_transition(StateMachine* sm, State from, State to, const char* trigger) {
    if (sm == NULL || trigger == NULL || strlen(trigger) == 0) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    Transition* t = &sm->transitions[sm->transition_count++];
    t->current_state = from;
    t->next_state = to;
    strncpy(t->trigger, trigger, sizeof(t->trigger) - 1);
    t->trigger[sizeof(t->trigger) - 1] = '\0';
    return 1;
}

State process_transition(StateMachine* sm, State current, const char* trigger) {
    if (sm == NULL || trigger == NULL) return current;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* t = &sm->transitions[i];
        if (t->current_state == current && strcmp(t->trigger, trigger) == 0) {
            return t->next_state;
        }
    }
    return current;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_COMPLETED: return "COMPLETED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine* sm = create_state_machine("TaskProcessor");
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, STATE_IDLE);
    add_state(sm, STATE_ACTIVE);
    add_state(sm, STATE_PAUSED);
    add_state(sm, STATE_COMPLETED);
    add_state(sm, STATE_ERROR);
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(sm, STATE_ACTIVE, STATE_COMPLETED, "finish");
    add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error");
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(sm, STATE_PAUSED, STATE_ERROR, "error");
    add_transition(sm, STATE_ERROR, STATE_IDLE, "reset");
    add_transition(sm, STATE_COMPLETED, STATE_IDLE, "reset");
    
    State current_state = STATE_IDLE;
    char input[BUFFER_SIZE];
    
    printf("State Machine: %s\n", sm->name);
    printf("Current state: %s\n", state_to_string(current_state));
    printf("Available triggers: start, pause, resume, finish, error, reset\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("Enter trigger: ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) break;
        if (strlen(input) == 0) continue;
        
        State new_state = process_transition(sm, current_state, input);
        
        if (new_state != current_state) {
            printf("Transition: %s -> %s\n", state_to_string(current_state), state_to_string(new_state));
            current_state = new_state;
        } else {
            printf("Invalid transition from %s with trigger '%s'\n", state_to_string(current_state), input);
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}