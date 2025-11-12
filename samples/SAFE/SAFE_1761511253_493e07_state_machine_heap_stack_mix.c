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

State get_next_state(const StateMachine *sm, char trigger) {
    if (sm == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            return sm->transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_input(StateMachine *sm, char input) {
    if (sm == NULL || sm->buffer == NULL) return 0;
    
    if (!validate_transition(sm, input)) {
        return 0;
    }
    
    State next_state = get_next_state(sm, input);
    
    if (sm->buffer_size >= BUFFER_SIZE - 1) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    
    sm->buffer[sm->buffer_size++] = input;
    sm->buffer[sm->buffer_size] = '\0';
    sm->current_state = next_state;
    
    return 1;
}

void print_state_info(const StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "ACTIVE", "PAUSED", "ERROR", "DONE"
    };
    
    int state_index = sm->current_state;
    if (state_index < 0 || state_index > 4) {
        state_index = 3;
    }
    
    printf("Current state: %s | Buffer: %s (%zu chars)\n", 
           state_names[state_index], sm->buffer, sm->buffer_size);
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_size = 0;
    
    sm.buffer = malloc(BUFFER_SIZE);
    if (sm.buffer == NULL) {
        return EXIT_FAILURE;
    }
    sm.buffer[0] = '\0';
    
    Transition *transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (transitions == NULL) {
        free(sm.buffer);
        return EXIT_FAILURE;
    }
    
    sm.transitions = transitions;
    sm.transition_count = 0;
    
    Transition predefined[] = {
        {STATE_IDLE, STATE_ACTIVE, 's'},
        {STATE_ACTIVE, STATE_PAUSED, 'p'},
        {STATE_ACTIVE, STATE_DONE, 'e'},
        {STATE_PAUSED, STATE_ACTIVE, 'r'},
        {STATE_PAUSED, STATE_ERROR, 'x'},
        {STATE_ERROR, STATE_IDLE, 'r'}
    };
    
    size_t predef_count = sizeof(predefined) / sizeof(predefined[0]);
    if (predef_count > MAX_TRANSITIONS) {
        free(sm.buffer);
        free(transitions);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < predef_count; i++) {
        if (sm.transition_count >= MAX_TRANSITIONS) break;
        sm.transitions[sm.transition_count++] = predefined[i];
    }
    
    printf("State Machine Controller\n");
    printf("Commands: s=start, p=pause, r=resume, e=end, x=error, q=quit\n");
    
    char input;
    int running = 1;
    
    while (running) {
        print_state_info(&sm);
        printf("Enter command: ");
        
        int result = scanf(" %c", &input);
        if (result != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 'q') {
            running = 0;
            continue;
        }
        
        if (!process_input(&sm, input)) {
            printf("Invalid transition from current state!\n");
        }
        
        if (sm.current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            running = 0;
        }
    }
    
    print_state_info(&sm);
    printf("Final state reached.\n");
    
    free(sm.buffer);
    free(transitions);
    
    return EXIT_SUCCESS;
}