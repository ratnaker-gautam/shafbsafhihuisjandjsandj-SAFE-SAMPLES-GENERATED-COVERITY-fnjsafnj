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
    State current_state;
    Transition *transitions;
    size_t transition_count;
    char *buffer;
    size_t buffer_size;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer[0] = '\0';
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, const char *trigger) {
    if (!sm || !trigger || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    if (strlen(trigger) >= 32) {
        return 0;
    }
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, const char *input) {
    if (!sm || !input) return sm ? sm->current_state : STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && strcmp(t->trigger, input) == 0) {
            sm->current_state = t->next;
            
            size_t input_len = strlen(input);
            if (input_len < BUFFER_SIZE - 1) {
                strncpy(sm->buffer, input, BUFFER_SIZE - 1);
                sm->buffer[BUFFER_SIZE - 1] = '\0';
            }
            
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

const char* state_to_string(State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start") ||
        !add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause") ||
        !add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume") ||
        !add_transition(sm, STATE_ACTIVE, STATE_DONE, "complete") ||
        !add_transition(sm, STATE_PAUSED, STATE_IDLE, "cancel") ||
        !add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error") ||
        !add_transition(sm, STATE_ERROR, STATE_IDLE, "reset")) {
        fprintf(stderr, "Failed to add transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    char input[64];
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, complete, cancel, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        State new_state = process_input(sm, input);
        printf("New state: %s\n\n", state_to_string(new_state));
    }
    
    destroy_state_machine(sm);
    return 0;
}