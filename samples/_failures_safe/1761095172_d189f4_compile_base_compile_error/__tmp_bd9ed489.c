//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    if (sm == NULL) {
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states[0] = STATE_IDLE;
    sm->states[1] = STATE_ACTIVE;
    sm->states[2] = STATE_PAUSED;
    sm->states[3] = STATE_COMPLETED;
    sm->states[4] = STATE_ERROR;
    
    sm->transition_count = 0;
    sm->current_state = STATE_IDLE;
    
    return sm;
}

int add_transition(StateMachine* sm, State from, State to, const char* trigger) {
    if (sm == NULL || trigger == NULL) {
        return 0;
    }
    
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    if (strlen(trigger) >= 32) {
        return 0;
    }
    
    Transition* t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine* sm, const char* trigger) {
    if (sm == NULL || trigger == NULL) {
        return sm ? sm->current_state : STATE_ERROR;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* t = &sm->transitions[i];
        if (t->current == sm->current_state && strcmp(t->trigger, trigger) == 0) {
            sm->current_state = t->next;
            break;
        }
    }
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
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
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start")) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause")) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume")) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_ACTIVE, STATE_COMPLETED, "finish")) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error")) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_PAUSED, STATE_ERROR, "error")) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    char input[BUFFER_SIZE];
    printf("State Machine Demo\n");
    printf("Available triggers: start, pause, resume, finish, error\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter trigger: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        State new_state = process_trigger(sm, input);
        printf("New state: %s\n\n", state_to_string(new_state));
        
        if (new_state == STATE_COMPLETED || new_state