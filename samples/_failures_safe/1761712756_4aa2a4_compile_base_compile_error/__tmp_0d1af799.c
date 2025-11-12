//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR
} State;

typedef struct {
    State current;
    State next;
    char condition;
} Transition;

typedef struct {
    State current_state;
    char* buffer;
    size_t buffer_size;
    size_t data_length;
    Transition* transitions;
    size_t transition_count;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer = malloc(sm->buffer_size);
    if (!sm->buffer) {
        free(sm);
        return NULL;
    }
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm->buffer);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_length = 0;
    sm->transition_count = 0;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        free(sm->buffer);
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine* sm, State from, State to, char condition) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].condition = condition;
    sm->transition_count++;
    return 1;
}

State process_transition(StateMachine* sm, char input) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].condition == input) {
            return sm->transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int handle_state(StateMachine* sm, char input) {
    if (!sm) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                sm->data_length = 0;
                return 1;
            }
            break;
            
        case STATE_READING:
            if (sm->data_length < sm->buffer_size - 1) {
                sm->buffer[sm->data_length++] = input;
                return 1;
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->data_length > 0) {
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                return 1;
            }
            break;
            
        case STATE_WRITING:
            if (sm->data_length > 0) {
                printf("Processed: %.*s\n", (int)sm->data_length, sm->buffer);
                sm->data_length = 0;
                return 1;
            }
            break;
            
        case STATE_ERROR:
            printf("Error state reached\n");
            return 0;
            
        default:
            return 0;
    }
    return 0;
}

int main() {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_READING, 'S');
    add_transition(sm, STATE_READING, STATE_PROCESSING, 'P');
    add_transition(sm, STATE_PROCESSING, STATE_WRITING, 'W');
    add_transition(sm, STATE_WRITING, STATE_IDLE, 'I');
    add_transition(sm, STATE_READING, STATE_ERROR, 'E');
    add_transition(sm, STATE_PROCESSING, STATE_ERROR, 'E');
    add_transition(sm, STATE_WRITING, STATE_ERROR, 'E');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'R');
    
    char input;
    printf("State Machine Controller\n");
    printf("Commands: S(start), P(process), W(write), I(idle), E(error), R(reset), Q(quit)\n");
    
    while (1) {
        printf("Current state: ");
        switch (sm->current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (scanf(" %c", &input) != 1) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        if (input