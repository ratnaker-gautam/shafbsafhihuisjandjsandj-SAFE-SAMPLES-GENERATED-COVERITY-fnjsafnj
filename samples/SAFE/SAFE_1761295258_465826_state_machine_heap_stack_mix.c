//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUT_SIZE 64

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
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
    char* buffer;
    size_t buffer_size;
    size_t data_length;
    Transition* transitions;
    size_t transition_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->buffer_size = 256;
    sm->buffer = malloc(sm->buffer_size);
    if (!sm->buffer) {
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_length = 0;
    
    sm->transition_count = 6;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm->buffer);
        free(sm);
        return NULL;
    }
    
    sm->transitions[0] = (Transition){STATE_IDLE, STATE_READING, 'R'};
    sm->transitions[1] = (Transition){STATE_READING, STATE_PROCESSING, 'P'};
    sm->transitions[2] = (Transition){STATE_PROCESSING, STATE_WRITING, 'W'};
    sm->transitions[3] = (Transition){STATE_WRITING, STATE_DONE, 'D'};
    sm->transitions[4] = (Transition){STATE_READING, STATE_ERROR, 'E'};
    sm->transitions[5] = (Transition){STATE_ERROR, STATE_IDLE, 'R'};
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        free(sm->buffer);
        free(sm->transitions);
        free(sm);
    }
}

State process_transition(StateMachine* sm, char input) {
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == input) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

void simulate_state_action(StateMachine* sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("State: IDLE - Ready to start\n");
            break;
        case STATE_READING:
            if (sm->data_length < sm->buffer_size - 1) {
                sm->buffer[sm->data_length++] = 'X';
                printf("State: READING - Data length: %zu\n", sm->data_length);
            }
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING - Processing %zu bytes\n", sm->data_length);
            break;
        case STATE_WRITING:
            printf("State: WRITING - Writing data\n");
            sm->data_length = 0;
            break;
        case STATE_ERROR:
            printf("State: ERROR - Reset required\n");
            sm->data_length = 0;
            break;
        case STATE_DONE:
            printf("State: DONE - Operation complete\n");
            break;
    }
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_SIZE];
    printf("State Machine Controller\n");
    printf("Commands: R(ead), P(rocess), W(rite), D(one), E(rror), Q(uit)\n");
    
    while (1) {
        printf("\nEnter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (input_buffer[0] == 'Q' || input_buffer[0] == 'q') {
            break;
        }
        
        if (strlen(input_buffer) < 1) {
            continue;
        }
        
        char command = input_buffer[0];
        State new_state = process_transition(sm, command);
        
        if (new_state != sm->current_state) {
            sm->current_state = new_state;
            simulate_state_action(sm);
        } else {
            printf("Invalid transition from current state\n");
        }
        
        if (sm->current_state == STATE_DONE) {
            printf("Resetting to IDLE state\n");
            sm->current_state = STATE_IDLE;
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}