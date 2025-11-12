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

StateContext* create_state_context(SystemState state, int data, const char* desc) {
    if (desc == NULL) return NULL;
    
    size_t desc_len = strlen(desc);
    if (desc_len >= BUFFER_SIZE) return NULL;
    
    StateContext* context = malloc(sizeof(StateContext));
    if (context == NULL) return NULL;
    
    context->description = malloc(desc_len + 1);
    if (context->description == NULL) {
        free(context);
        return NULL;
    }
    
    context->state = state;
    context->data = data;
    strncpy(context->description, desc, desc_len);
    context->description[desc_len] = '\0';
    
    return context;
}

void free_state_context(StateContext* context) {
    if (context != NULL) {
        free(context->description);
        free(context);
    }
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
    
    SystemState current_state = STATE_IDLE;
    char input_buffer[BUFFER_SIZE];
    
    printf("State Machine Simulation\n");
    printf("Available triggers: 1(IDLE->ACTIVE), 2(ACTIVE->PAUSED), 3(ACTIVE->COMPLETED), 4(PAUSED->ACTIVE), 5(PAUSED->ERROR)\n");
    printf("Enter 'quit' to exit\n");
    
    while (1) {
        StateContext* context = create_state_context(current_state, 0, state_to_string(current_state));
        if (context == NULL) {
            fprintf(stderr, "Failed to create state context\n");
            free(transitions);
            return 1;
        }
        
        printf("\nCurrent state: %s (%s)\n", state_to_string(current_state), context->description);
        printf("Enter trigger: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            free_state_context(context);
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) {
            free_state_context(context);
            break;
        }
        
        char* endptr;
        long trigger = strtol(input_buffer, &endptr, 10);
        if (endptr == input_buffer || *endptr != '\0') {
            printf("Invalid input. Please enter a number or 'quit'\n");
            free_state_context(context);
            continue;
        }
        
        if (trigger < 1 || trigger > 5) {
            printf("Trigger must be between 1 and 5\n");
            free_state_context(context);
            continue;
        }
        
        SystemState new_state = process_transition(current_state, (int)trigger, transitions, transition_count);
        
        if (new_state == STATE_ERROR) {
            printf("Invalid transition from %s with trigger %ld\n", state_to_string(current_state), trigger);
        } else {
            current_state = new_state;
            printf("Transitioned to: %s\n", state_to_string(current_state));
        }
        
        free_state_context(context);
    }
    
    free(transitions);
    printf("State machine terminated\n");
    return 0