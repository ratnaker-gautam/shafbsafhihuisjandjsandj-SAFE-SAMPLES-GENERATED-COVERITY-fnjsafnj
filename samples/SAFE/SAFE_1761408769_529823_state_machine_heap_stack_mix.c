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
    char* description;
} StateContext;

StateTransition* create_transitions(int* count) {
    if (count == NULL) return NULL;
    
    *count = 5;
    StateTransition* transitions = malloc(*count * sizeof(StateTransition));
    if (transitions == NULL) return NULL;
    
    transitions[0] = (StateTransition){STATE_IDLE, STATE_ACTIVE, 1};
    transitions[1] = (StateTransition){STATE_ACTIVE, STATE_PAUSED, 2};
    transitions[2] = (StateTransition){STATE_ACTIVE, STATE_COMPLETED, 3};
    transitions[3] = (StateTransition){STATE_PAUSED, STATE_ACTIVE, 4};
    transitions[4] = (StateTransition){STATE_PAUSED, STATE_ERROR, 5};
    
    return transitions;
}

StateContext* create_state_context(SystemState state, const char* desc) {
    if (desc == NULL) return NULL;
    
    size_t len = strlen(desc);
    if (len >= BUFFER_SIZE) return NULL;
    
    StateContext* context = malloc(sizeof(StateContext));
    if (context == NULL) return NULL;
    
    context->description = malloc(len + 1);
    if (context->description == NULL) {
        free(context);
        return NULL;
    }
    
    strncpy(context->description, desc, len);
    context->description[len] = '\0';
    context->state = state;
    context->data = 0;
    
    return context;
}

void destroy_state_context(StateContext* context) {
    if (context != NULL) {
        if (context->description != NULL) {
            free(context->description);
        }
        free(context);
    }
}

SystemState process_transition(SystemState current, int trigger, StateTransition* transitions, int count) {
    if (transitions == NULL || count <= 0) return current;
    
    for (int i = 0; i < count; i++) {
        if (transitions[i].current_state == current && transitions[i].trigger == trigger) {
            return transitions[i].next_state;
        }
    }
    return current;
}

const char* state_to_string(SystemState state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_COMPLETED: return "COMPLETED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    int transition_count = 0;
    StateTransition* transitions = create_transitions(&transition_count);
    if (transitions == NULL) {
        fprintf(stderr, "Failed to create transitions\n");
        return 1;
    }
    
    StateContext stack_context;
    stack_context.state = STATE_IDLE;
    stack_context.data = 100;
    stack_context.description = NULL;
    
    StateContext* heap_context = create_state_context(STATE_IDLE, "Initial state");
    if (heap_context == NULL) {
        fprintf(stderr, "Failed to create heap context\n");
        free(transitions);
        return 1;
    }
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_to_string(stack_context.state));
    
    int triggers[] = {1, 2, 4, 3, 5};
    int trigger_count = sizeof(triggers) / sizeof(triggers[0]);
    
    for (int i = 0; i < trigger_count; i++) {
        SystemState new_state = process_transition(stack_context.state, triggers[i], transitions, transition_count);
        
        if (new_state != stack_context.state) {
            printf("Trigger %d: %s -> %s\n", triggers[i], 
                   state_to_string(stack_context.state), 
                   state_to_string(new_state));
            stack_context.state = new_state;
            heap_context->state = new_state;
        } else {
            printf("Trigger %d: No transition from %s\n", triggers[i], 
                   state_to_string(stack_context.state));
        }
        
        if (stack_context.state == STATE_ERROR || stack_context.state == STATE_COMPLETED) {
            break;
        }
    }
    
    printf("Final state: %s\n", state_to_string(stack_context.state));
    printf("Heap context state: %s, description: %s\n", 
           state_to_string(heap_context->state), heap_context->description);
    
    free(transitions);
    destroy_state_context(heap_context);
    
    return 0;
}