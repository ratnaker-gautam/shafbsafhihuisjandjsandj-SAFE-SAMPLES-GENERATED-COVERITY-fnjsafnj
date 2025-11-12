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

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
    memset(sm->buffer, 0, BUFFER_SIZE);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, char trigger) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return STATE_ERROR;
}

int validate_input(const char *input, size_t len) {
    if (!input || len == 0 || len >= BUFFER_SIZE) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 'a' || input[i] > 'z') return 0;
    }
    return 1;
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 'd');
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(sm, STATE_PAUSED, STATE_DONE, 'd');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    
    printf("State Machine Controller\n");
    printf("Commands: s(start), p(pause), r(resume), d(done), q(quit)\n");
    printf("Current state: IDLE\n");
    
    char input[BUFFER_SIZE];
    while (1) {
        printf("Enter command: ");
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (!validate_input(input, len)) {
            if (len == 1 && input[0] == 'q') {
                printf("Quitting...\n");
                break;
            }
            printf("Invalid input. Use single lowercase letters only.\n");
            continue;
        }
        
        char command = input[0];
        State new_state = process_trigger(sm, command);
        
        switch (new_state) {
            case STATE_IDLE:
                printf("State: IDLE\n");
                break;
            case STATE_ACTIVE:
                printf("State: ACTIVE\n");
                break;
            case STATE_PAUSED:
                printf("State: PAUSED\n");
                break;
            case STATE_DONE:
                printf("State: DONE - Process completed\n");
                break;
            case STATE_ERROR:
                printf("State: ERROR - Invalid transition\n");
                break;
        }
        
        if (new_state == STATE_DONE) {
            printf("Process finished. Resetting...\n");
            sm->current_state = STATE_IDLE;
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}