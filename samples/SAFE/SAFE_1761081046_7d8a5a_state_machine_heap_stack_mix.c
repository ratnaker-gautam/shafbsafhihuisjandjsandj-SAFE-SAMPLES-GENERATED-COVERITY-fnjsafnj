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
    if (sm == NULL || sm->transitions == NULL) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            return 1;
        }
    }
    return 0;
}

State get_next_state(const StateMachine *sm, char trigger) {
    if (sm == NULL || sm->transitions == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            return sm->transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_buffer(StateMachine *sm) {
    if (sm == NULL || sm->buffer == NULL) return 0;
    
    size_t len = strnlen(sm->buffer, sm->buffer_size);
    if (len == 0 || len >= sm->buffer_size) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char trigger = sm->buffer[i];
        
        if (!validate_transition(sm, trigger)) {
            sm->current_state = STATE_ERROR;
            return 0;
        }
        
        sm->current_state = get_next_state(sm, trigger);
        
        if (sm->current_state == STATE_ERROR) {
            return 0;
        }
        
        if (sm->current_state == STATE_DONE) {
            return 1;
        }
    }
    
    return 1;
}

void cleanup_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->transitions != NULL) {
        free(sm->transitions);
        sm->transitions = NULL;
    }
    
    if (sm->buffer != NULL) {
        free(sm->buffer);
        sm->buffer = NULL;
    }
    
    sm->transition_count = 0;
    sm->buffer_size = 0;
}

int main(void) {
    StateMachine machine = {STATE_IDLE, NULL, 0, NULL, 0};
    
    machine.transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (machine.transitions == NULL) {
        return EXIT_FAILURE;
    }
    
    machine.transition_count = 6;
    machine.transitions[0] = (Transition){STATE_IDLE, STATE_ACTIVE, 's'};
    machine.transitions[1] = (Transition){STATE_ACTIVE, STATE_PAUSED, 'p'};
    machine.transitions[2] = (Transition){STATE_ACTIVE, STATE_DONE, 'e'};
    machine.transitions[3] = (Transition){STATE_PAUSED, STATE_ACTIVE, 'r'};
    machine.transitions[4] = (Transition){STATE_PAUSED, STATE_ERROR, 'x'};
    machine.transitions[5] = (Transition){STATE_ERROR, STATE_IDLE, 'i'};
    
    machine.buffer_size = BUFFER_SIZE;
    machine.buffer = malloc(machine.buffer_size);
    if (machine.buffer == NULL) {
        cleanup_machine(&machine);
        return EXIT_FAILURE;
    }
    
    printf("Enter command sequence (s=start, p=pause, r=resume, e=end, x=error, i=idle): ");
    
    if (fgets(machine.buffer, (int)machine.buffer_size, stdin) == NULL) {
        cleanup_machine(&machine);
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(machine.buffer, machine.buffer_size);
    if (len > 0 && machine.buffer[len-1] == '\n') {
        machine.buffer[len-1] = '\0';
    }
    
    if (!process_buffer(&machine)) {
        printf("State machine ended in error state\n");
        cleanup_machine(&machine);
        return EXIT_FAILURE;
    }
    
    const char *state_names[] = {"IDLE", "ACTIVE", "PAUSED", "ERROR", "DONE"};
    if (machine.current_state >= STATE_IDLE && machine.current_state <= STATE_DONE) {
        printf("Final state: %s\n", state_names[machine.current_state]);
    } else {
        printf("Invalid final state\n");
    }
    
    cleanup_machine(&machine);
    return EXIT_SUCCESS;
}