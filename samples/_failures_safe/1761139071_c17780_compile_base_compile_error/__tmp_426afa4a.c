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
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR
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
    
    sm->buffer = malloc(BUFFER_SIZE);
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
    sm->buffer_size = BUFFER_SIZE;
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

int add_transition(StateMachine* sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_transition(StateMachine* sm, char input) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

void process_state(StateMachine* sm) {
    if (!sm) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("State: IDLE - Ready for input\n");
            break;
            
        case STATE_READING:
            if (sm->data_length < sm->buffer_size - 1) {
                printf("Enter character: ");
                int c = getchar();
                if (c != EOF && c != '\n') {
                    sm->buffer[sm->data_length++] = (char)c;
                    sm->buffer[sm->data_length] = '\0';
                }
                while (getchar() != '\n');
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->data_length > 0) {
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed data: %s\n", sm->buffer);
            }
            break;
            
        case STATE_WRITING:
            if (sm->data_length > 0) {
                printf("Final output: %s\n", sm->buffer);
                sm->data_length = 0;
                sm->buffer[0] = '\0';
            }
            break;
            
        case STATE_ERROR:
            printf("State: ERROR - Invalid operation\n");
            break;
    }
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_READING, 'r');
    add_transition(sm, STATE_READING, STATE_PROCESSING, 'p');
    add_transition(sm, STATE_PROCESSING, STATE_WRITING, 'w');
    add_transition(sm, STATE_WRITING, STATE_IDLE, 'i');
    add_transition(sm, STATE_READING, STATE_IDLE, 'i');
    add_transition(sm, STATE_PROCESSING, STATE_IDLE, 'i');
    add_transition(sm, STATE_WRITING, STATE_IDLE, 'i');
    add_transition(sm, STATE_IDLE, STATE_ERROR, 'e');
    add_transition(sm, STATE_READING, STATE_ERROR, 'e');
    add_transition(sm, STATE_PROCESSING, STATE_ERROR, 'e');
    add_transition(sm, STATE_WRITING, STATE_ERROR, 'e');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'i');
    
    char input;
    printf("State Machine Controller\n");
    printf("Commands: r=read, p=process, w=write, i=idle, e=error, q=quit\n");
    
    while (1) {
        process_state(sm);
        
        printf("Enter command: ");