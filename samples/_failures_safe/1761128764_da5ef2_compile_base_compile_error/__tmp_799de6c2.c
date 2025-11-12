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
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current_state;
    State next_state;
    char trigger[32];
} Transition;

typedef struct {
    State state;
    char data[BUFFER_SIZE];
    size_t data_len;
} StateData;

typedef struct {
    StateData* heap_data;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
    State current_state;
} StateMachine;

int add_transition(StateMachine* sm, State from, State to, const char* trigger) {
    if (sm == NULL || trigger == NULL) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    size_t trigger_len = strlen(trigger);
    if (trigger_len >= 32) return 0;
    
    Transition* t = &sm->transitions[sm->transition_count];
    t->current_state = from;
    t->next_state = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    
    sm->transition_count++;
    return 1;
}

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->heap_data = malloc(sizeof(StateData));
    if (sm->heap_data == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->heap_data->state = STATE_IDLE;
    sm->heap_data->data_len = 0;
    sm->heap_data->data[0] = '\0';
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start")) return NULL;
    if (!add_transition(sm, STATE_ACTIVE, STATE_PROCESSING, "process")) return NULL;
    if (!add_transition(sm, STATE_PROCESSING, STATE_DONE, "complete")) return NULL;
    if (!add_transition(sm, STATE_PROCESSING, STATE_ERROR, "error")) return NULL;
    if (!add_transition(sm, STATE_ERROR, STATE_IDLE, "reset")) return NULL;
    if (!add_transition(sm, STATE_DONE, STATE_IDLE, "reset")) return NULL;
    
    return sm;
}

int process_trigger(StateMachine* sm, const char* trigger) {
    if (sm == NULL || trigger == NULL) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* t = &sm->transitions[i];
        if (t->current_state == sm->current_state && 
            strcmp(t->trigger, trigger) == 0) {
            sm->current_state = t->next_state;
            sm->heap_data->state = t->next_state;
            return 1;
        }
    }
    return 0;
}

void update_state_data(StateMachine* sm, const char* data) {
    if (sm == NULL || data == NULL) return;
    
    size_t data_len = strlen(data);
    if (data_len >= BUFFER_SIZE) return;
    
    strncpy(sm->heap_data->data, data, BUFFER_SIZE - 1);
    sm->heap_data->data[BUFFER_SIZE - 1] = '\0';
    sm->heap_data->data_len = data_len;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

void print_state_info(StateMachine* sm) {
    if (sm == NULL) return;
    printf("Current State: %s\n", state_to_string(sm->current_state));
    printf("Data: %s (length: %zu)\n", sm->heap_data->data, sm->heap_data->data_len);
}

void destroy_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    if (sm->heap_data != NULL) {
        free(sm->heap_data);
    }
    free(sm);
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    int running = 1;
    
    printf("State Machine Controller\n");
    printf("Available triggers: start, process, complete, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (running) {
        print_state_info(sm);
        printf("\nEnter trigger: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;