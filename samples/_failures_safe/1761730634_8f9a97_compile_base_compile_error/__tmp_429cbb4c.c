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
    if (context == NULL) return;
    if (context->description != NULL) {
        free(context->description);
    }
    free(context);
}

SystemState process_transition(SystemState current, int trigger, StateTransition* transitions, int count) {
    if (transitions == NULL || count <= 0) return STATE_ERROR;
    
    for (int i = 0; i < count; i++) {
        if (transitions[i].current_state == current && transitions[i].trigger == trigger) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
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
    int transition_count;
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
    printf("Available triggers: 1(START), 2(PAUSE), 3(COMPLETE), 4(RESUME), 5(ERROR)\n");
    printf("Current state: %s\n", state_to_string(heap_context->state));
    
    int user_input;
    char input_buffer[32];
    
    while (heap_context->state != STATE_COMPLETED && heap_context->state != STATE_ERROR) {
        printf("Enter trigger (0 to exit): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        if (sscanf(input_buffer, "%d", &user_input) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (user_input == 0) {
            printf("Exiting state machine\n");
            break;
        }
        
        SystemState new_state = process_transition(heap_context->state, user_input, transitions, transition_count);
        
        if (new_state == STATE_ERROR) {
            printf("Invalid transition from state %s with trigger %d\n", 
                   state_to_string(heap_context->state), user_input);
        } else {
            heap_context->state = new_state;
            stack_context.state = new_state;
            heap_context->data += 10;
            stack_context.data += 5;
            
            printf("Transition successful. New state: %s\n", state_to_string(heap_context->state));
            printf("Heap data: %d, Stack data: %d\n", heap_context->data, stack_context.data);
        }
    }
    
    printf("Final state: %s\n", state_to_string(heap_context->state));
    
    destroy_state_context(heap_context);
    free(transitions);