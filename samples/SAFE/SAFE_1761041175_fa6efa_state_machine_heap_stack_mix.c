//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    char condition;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
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
    
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer = malloc(sm->buffer_size);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    memset(sm->buffer, 0, sm->buffer_size);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, char condition) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].condition = condition;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].condition == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

void process_string(StateMachine *sm, const char *input) {
    if (!sm || !input) return;
    
    size_t len = strlen(input);
    if (len >= sm->buffer_size - 1) return;
    
    strncpy(sm->buffer, input, sm->buffer_size - 1);
    sm->buffer[sm->buffer_size - 1] = '\0';
    
    for (size_t i = 0; i < len; i++) {
        process_input(sm, sm->buffer[i]);
    }
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
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 'e');
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(sm, STATE_PAUSED, STATE_ERROR, 'x');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    
    char input_buffer[BUFFER_SIZE];
    
    printf("Enter command sequence (s=start, p=pause, r=resume, e=end, x=error): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    printf("Initial state: ");
    print_state(sm->current_state);
    printf("\n");
    
    process_string(sm, input_buffer);
    
    printf("Final state: ");
    print_state(sm->current_state);
    printf("\n");
    
    destroy_state_machine(sm);
    return 0;
}