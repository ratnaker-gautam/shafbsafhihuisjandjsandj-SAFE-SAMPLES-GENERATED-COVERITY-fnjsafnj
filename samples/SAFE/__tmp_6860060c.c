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
    State state;
    char* buffer;
    size_t buffer_size;
    size_t data_length;
} StateMachine;

StateMachine* create_state_machine(size_t initial_size) {
    if (initial_size == 0 || initial_size > BUFFER_SIZE) {
        return NULL;
    }
    
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) {
        return NULL;
    }
    
    sm->buffer = malloc(initial_size);
    if (!sm->buffer) {
        free(sm);
        return NULL;
    }
    
    sm->state = STATE_IDLE;
    sm->buffer_size = initial_size;
    sm->data_length = 0;
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(Transition* transitions, int* count, State current, State next, char trigger) {
    if (!transitions || !count || *count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    transitions[*count].current = current;
    transitions[*count].next = next;
    transitions[*count].trigger = trigger;
    (*count)++;
    return 1;
}

State process_transition(State current, char input, Transition* transitions, int count) {
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int resize_buffer(StateMachine* sm, size_t new_size) {
    if (!sm || new_size == 0 || new_size > BUFFER_SIZE) {
        return 0;
    }
    
    char* new_buffer = realloc(sm->buffer, new_size);
    if (!new_buffer) {
        return 0;
    }
    
    sm->buffer = new_buffer;
    sm->buffer_size = new_size;
    if (sm->data_length > new_size) {
        sm->data_length = new_size;
    }
    return 1;
}

int main(void) {
    StateMachine* sm = create_state_machine(64);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    Transition transitions[MAX_TRANSITIONS];
    int transition_count = 0;
    
    add_transition(transitions, &transition_count, STATE_IDLE, STATE_READING, 'r');
    add_transition(transitions, &transition_count, STATE_READING, STATE_PROCESSING, 'p');
    add_transition(transitions, &transition_count, STATE_PROCESSING, STATE_WRITING, 'w');
    add_transition(transitions, &transition_count, STATE_WRITING, STATE_IDLE, 'i');
    add_transition(transitions, &transition_count, STATE_READING, STATE_ERROR, 'e');
    add_transition(transitions, &transition_count, STATE_PROCESSING, STATE_ERROR, 'e');
    add_transition(transitions, &transition_count, STATE_WRITING, STATE_ERROR, 'e');
    add_transition(transitions, &transition_count, STATE_ERROR, STATE_IDLE, 'r');
    
    printf("State Machine Simulation\n");
    printf("Commands: r=read, p=process, w=write, i=idle, e=error, q=quit\n");
    
    char input;
    while (1) {
        printf("Current state: ");
        switch (sm->state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (scanf(" %c", &input) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 'q') {
            break;
        }
        
        State new_state = process_transition(sm->state, input, transitions, transition_count);
        
        switch (new_state) {
            case STATE_READING:
                if (sm->data_length < sm->buffer_size) {
                    sm->buffer[sm->data_length++] = 'A' + (rand() % 26);
                    printf("Added data to buffer. Length: %zu\n", sm->data_length);
                } else {
                    printf("Buffer full, cannot add more data\n");
                    new_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                if (sm->data_length > 0) {
                    for (size_t i = 0; i < sm->data_length; i++) {
                        sm->buffer[i] = (sm->buffer[i] - 'A' + 1) % 26 + 'A';
                    }
                    printf("Processed data in