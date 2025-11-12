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
    char trigger;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    size_t transition_count;
    char *buffer;
    size_t buffer_size;
} StateMachine;

int validate_transition(const StateMachine *sm, char trigger) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state &&
            sm->transitions[i].trigger == trigger) {
            return 1;
        }
    }
    return 0;
}

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (sm->buffer == NULL) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer[0] = '\0';
    
    Transition initial_transitions[] = {
        {STATE_IDLE, STATE_ACTIVE, 's'},
        {STATE_ACTIVE, STATE_PAUSED, 'p'},
        {STATE_ACTIVE, STATE_ERROR, 'e'},
        {STATE_ACTIVE, STATE_DONE, 'f'},
        {STATE_PAUSED, STATE_ACTIVE, 'r'},
        {STATE_PAUSED, STATE_ERROR, 'e'},
        {STATE_ERROR, STATE_IDLE, 'r'},
        {STATE_DONE, STATE_IDLE, 'r'}
    };
    
    size_t count = sizeof(initial_transitions) / sizeof(initial_transitions[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (size_t i = 0; i < count; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count] = initial_transitions[i];
            sm->transition_count++;
        }
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int process_trigger(StateMachine *sm, char trigger) {
    if (sm == NULL) return 0;
    
    if (!validate_transition(sm, trigger)) {
        return 0;
    }
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state &&
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            
            if (sm->buffer_size > 0) {
                size_t len = strlen(sm->buffer);
                if (len < sm->buffer_size - 2) {
                    sm->buffer[len] = trigger;
                    sm->buffer[len + 1] = ' ';
                    sm->buffer[len + 2] = '\0';
                }
            }
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
    StateMachine *sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Controller\n");
    printf("Available triggers: s(start), p(pause), r(reset), e(error), f(finish)\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    
    char input[32];
    int running = 1;
    
    while (running) {
        printf("\nEnter trigger (q to quit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            running = 0;
            continue;
        }
        
        if (strlen(input) < 2 || input[1] != '\n') {
            printf("Invalid input. Enter single character only.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        char trigger = input[0];
        
        if (process_trigger(sm, trigger