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
} SystemState;

typedef struct {
    SystemState current_state;
    SystemState next_state;
    int trigger;
} StateTransition;

typedef struct {
    SystemState state;
    int data;
    char* message;
} StateContext;

StateTransition* create_transitions(int* count) {
    *count = 5;
    StateTransition* transitions = malloc(*count * sizeof(StateTransition));
    if (!transitions) return NULL;
    
    transitions[0] = (StateTransition){STATE_IDLE, STATE_ACTIVE, 1};
    transitions[1] = (StateTransition){STATE_ACTIVE, STATE_PAUSED, 2};
    transitions[2] = (StateTransition){STATE_ACTIVE, STATE_COMPLETED, 3};
    transitions[3] = (StateTransition){STATE_PAUSED, STATE_ACTIVE, 4};
    transitions[4] = (StateTransition){STATE_ACTIVE, STATE_ERROR, 5};
    
    return transitions;
}

StateContext* create_state_context(SystemState initial_state) {
    StateContext* context = malloc(sizeof(StateContext));
    if (!context) return NULL;
    
    context->state = initial_state;
    context->data = 0;
    context->message = malloc(BUFFER_SIZE);
    if (!context->message) {
        free(context);
        return NULL;
    }
    strncpy(context->message, "Initial state", BUFFER_SIZE - 1);
    context->message[BUFFER_SIZE - 1] = '\0';
    
    return context;
}

void destroy_state_context(StateContext* context) {
    if (context) {
        free(context->message);
        free(context);
    }
}

SystemState process_transition(SystemState current_state, int trigger, StateTransition* transitions, int transition_count) {
    for (int i = 0; i < transition_count; i++) {
        if (transitions[i].current_state == current_state && transitions[i].trigger == trigger) {
            return transitions[i].next_state;
        }
    }
    return current_state;
}

void update_state_context(StateContext* context, SystemState new_state) {
    if (!context) return;
    
    context->state = new_state;
    context->data++;
    
    const char* state_names[] = {"IDLE", "ACTIVE", "PAUSED", "COMPLETED", "ERROR"};
    if (new_state >= STATE_IDLE && new_state <= STATE_ERROR) {
        snprintf(context->message, BUFFER_SIZE, "State: %s, Operations: %d", 
                state_names[new_state], context->data);
    }
}

void print_state_info(StateContext* context) {
    if (context) {
        printf("%s\n", context->message);
    }
}

int main() {
    int transition_count;
    StateTransition* transitions = create_transitions(&transition_count);
    if (!transitions) {
        fprintf(stderr, "Failed to create transitions\n");
        return 1;
    }
    
    StateContext* context = create_state_context(STATE_IDLE);
    if (!context) {
        fprintf(stderr, "Failed to create state context\n");
        free(transitions);
        return 1;
    }
    
    printf("State Machine Started\n");
    print_state_info(context);
    
    int triggers[] = {1, 2, 4, 3, 5, 1};
    int trigger_count = sizeof(triggers) / sizeof(triggers[0]);
    
    for (int i = 0; i < trigger_count; i++) {
        SystemState new_state = process_transition(context->state, triggers[i], transitions, transition_count);
        if (new_state != context->state) {
            update_state_context(context, new_state);
            printf("Trigger %d: ", triggers[i]);
            print_state_info(context);
        } else {
            printf("Trigger %d: No state change\n", triggers[i]);
        }
        
        if (context->state == STATE_COMPLETED || context->state == STATE_ERROR) {
            break;
        }
    }
    
    printf("Final state: ");
    print_state_info(context);
    
    destroy_state_context(context);
    free(transitions);
    
    return 0;
}