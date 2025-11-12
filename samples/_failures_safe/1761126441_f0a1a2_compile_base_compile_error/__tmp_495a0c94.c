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
    State state;
    char *buffer;
    size_t buffer_size;
    size_t data_length;
} StateMachine;

StateMachine* create_state_machine(size_t initial_size) {
    if (initial_size == 0 || initial_size > BUFFER_SIZE) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
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

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(Transition *transitions, int *count, State current, State next, char trigger) {
    if (!transitions || !count || *count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    transitions[*count].current = current;
    transitions[*count].next = next;
    transitions[*count].trigger = trigger;
    (*count)++;
    return 1;
}

State process_transition(State current, char input, Transition *transitions, int count) {
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int read_data(StateMachine *sm, const char *data, size_t data_len) {
    if (!sm || !data || data_len == 0 || data_len > BUFFER_SIZE) {
        return 0;
    }
    
    if (sm->data_length + data_len > sm->buffer_size) {
        size_t new_size = sm->buffer_size * 2;
        if (new_size > BUFFER_SIZE) new_size = BUFFER_SIZE;
        
        char *new_buffer = realloc(sm->buffer, new_size);
        if (!new_buffer) return 0;
        
        sm->buffer = new_buffer;
        sm->buffer_size = new_size;
    }
    
    memcpy(sm->buffer + sm->data_length, data, data_len);
    sm->data_length += data_len;
    return 1;
}

int process_data(StateMachine *sm) {
    if (!sm || sm->data_length == 0) return 0;
    
    for (size_t i = 0; i < sm->data_length; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    return 1;
}

int write_data(const StateMachine *sm) {
    if (!sm || sm->data_length == 0) return 0;
    
    for (size_t i = 0; i < sm->data_length; i++) {
        putchar(sm->buffer[i]);
    }
    putchar('\n');
    return 1;
}

int main(void) {
    Transition transitions_stack[MAX_TRANSITIONS];
    int transition_count = 0;
    
    add_transition(transitions_stack, &transition_count, STATE_IDLE, STATE_READING, 'R');
    add_transition(transitions_stack, &transition_count, STATE_READING, STATE_PROCESSING, 'P');
    add_transition(transitions_stack, &transition_count, STATE_PROCESSING, STATE_WRITING, 'W');
    add_transition(transitions_stack, &transition_count, STATE_WRITING, STATE_IDLE, 'I');
    add_transition(transitions_stack, &transition_count, STATE_ERROR, STATE_IDLE, 'R');
    
    StateMachine *machine = create_state_machine(64);
    if (!machine) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_sequence[] = {'R', 'P', 'W', 'I', 'R', 'P', 'W', 'I'};
    char *test_data[] = {"hello", "world"};
    size_t data_index = 0;
    
    for (size_t i = 0; i < sizeof(input_sequence); i++) {
        char cmd = input_sequence[i];
        State next_state = process_transition(machine->state, cmd, transitions_stack, transition_count);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid transition from state %d with command %c\n", machine->state, cmd);
            machine->state = STATE_ERROR;
            continue;
        }
        
        machine->state = next_state;
        
        switch (machine->state) {
            case STATE_READING:
                if (data_index < 2) {
                    if (!read_data(machine, test_data[data_index], strlen(test_data[data_index]))) {
                        machine->state = STATE_ERROR;
                    }
                    data_index++;