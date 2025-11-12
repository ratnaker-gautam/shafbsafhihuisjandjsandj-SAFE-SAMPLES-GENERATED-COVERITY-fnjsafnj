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

StateMachine* create_state_machine(void) {
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
    if (!sm || !trigger) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    size_t len = strlen(trigger);
    if (len >= 32) return 0;
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, const char *input) {
    if (!sm || !input) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= BUFFER_SIZE) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && strcmp(t->trigger, input) == 0) {
            sm->current_state = t->next;
            
            if (strlen(sm->buffer) + input_len + 2 < sm->buffer_size) {
                strcat(sm->buffer, input);
                strcat(sm->buffer, " ");
            }
            return 1;
        }
    }
    return 0;
}

void print_state_info(const StateMachine *sm) {
    if (!sm) return;
    
    const char *state_names[] = {
        "IDLE", "ACTIVE", "PAUSED", "ERROR", "DONE"
    };
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Buffer: %s\n", sm->buffer);
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(sm, STATE_ACTIVE, STATE_DONE, "complete");
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(sm, STATE_PAUSED, STATE_ERROR, "error");
    add_transition(sm, STATE_ERROR, STATE_IDLE, "reset");
    
    char input[64];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, complete, error, reset\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        print_state_info(sm);
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) break;
        
        if (!process_input(sm, input)) {
            printf("Invalid transition from current state\n");
        }
        
        if (sm->current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}