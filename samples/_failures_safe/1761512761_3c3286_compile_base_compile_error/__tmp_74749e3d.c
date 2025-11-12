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

int add_transition(StateMachine* sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine* sm, char trigger) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

int read_data(StateMachine* sm) {
    if (!sm || sm->current_state != STATE_READING) return 0;
    
    printf("Enter data (max %zu characters): ", sm->buffer_size - 1);
    if (!fgets(sm->buffer, sm->buffer_size, stdin)) return 0;
    
    sm->data_length = strlen(sm->buffer);
    if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
        sm->buffer[sm->data_length - 1] = '\0';
        sm->data_length--;
    }
    
    return sm->data_length > 0;
}

int process_data(StateMachine* sm) {
    if (!sm || sm->current_state != STATE_PROCESSING) return 0;
    if (sm->data_length == 0) return 0;
    
    for (size_t i = 0; i < sm->data_length; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return 1;
}

int write_data(StateMachine* sm) {
    if (!sm || sm->current_state != STATE_WRITING) return 0;
    
    printf("Processed data: %s\n", sm->buffer);
    sm->data_length = 0;
    return 1;
}

void print_state(State state) {
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (state < STATE_IDLE || state > STATE_ERROR) {
        printf("UNKNOWN\n");
    } else {
        printf("%s\n", state_names[state]);
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
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'i');
    
    printf("State Machine Controller\n");
    printf("Commands: r=read, p=process, w=write, i=idle, q=quit\n");
    
    char command;
    int running = 1;
    
    while (running) {
        printf("\nCurrent state: ");
        print_state(sm->current_state);
        printf("Enter command: ");
        
        if (scanf(" %c", &command) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (command == 'q') {