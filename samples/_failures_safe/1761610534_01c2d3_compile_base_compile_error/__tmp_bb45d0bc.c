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
    char* buffer_heap;
    size_t buffer_size;
    char stack_buffer[BUFFER_SIZE];
    size_t stack_index;
} StateMachine;

int validate_transition(const Transition* transitions, int count, State current, char trigger, State* next) {
    if (transitions == NULL || next == NULL || count <= 0 || count > MAX_TRANSITIONS) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].trigger == trigger) {
            *next = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

int initialize_machine(StateMachine* machine) {
    if (machine == NULL) {
        return 0;
    }
    
    machine->current_state = STATE_IDLE;
    machine->buffer_heap = malloc(BUFFER_SIZE);
    if (machine->buffer_heap == NULL) {
        return 0;
    }
    machine->buffer_size = BUFFER_SIZE;
    machine->stack_index = 0;
    memset(machine->stack_buffer, 0, BUFFER_SIZE);
    memset(machine->buffer_heap, 0, BUFFER_SIZE);
    
    return 1;
}

void cleanup_machine(StateMachine* machine) {
    if (machine != NULL && machine->buffer_heap != NULL) {
        free(machine->buffer_heap);
        machine->buffer_heap = NULL;
    }
}

int process_input(StateMachine* machine, const Transition* transitions, int trans_count, char input) {
    if (machine == NULL || transitions == NULL || trans_count <= 0) {
        return 0;
    }
    
    State next_state;
    if (!validate_transition(transitions, trans_count, machine->current_state, input, &next_state)) {
        machine->current_state = STATE_ERROR;
        return 0;
    }
    
    machine->current_state = next_state;
    
    switch (machine->current_state) {
        case STATE_READING:
            if (machine->stack_index < BUFFER_SIZE - 1) {
                machine->stack_buffer[machine->stack_index++] = input;
                machine->stack_buffer[machine->stack_index] = '\0';
            }
            break;
            
        case STATE_PROCESSING:
            if (machine->stack_index > 0 && machine->buffer_heap != NULL) {
                size_t copy_size = machine->stack_index;
                if (copy_size >= machine->buffer_size) {
                    copy_size = machine->buffer_size - 1;
                }
                memcpy(machine->buffer_heap, machine->stack_buffer, copy_size);
                machine->buffer_heap[copy_size] = '\0';
                machine->stack_index = 0;
                memset(machine->stack_buffer, 0, BUFFER_SIZE);
            }
            break;
            
        case STATE_WRITING:
            if (machine->buffer_heap != NULL && strlen(machine->buffer_heap) > 0) {
                printf("Processed: %s\n", machine->buffer_heap);
                memset(machine->buffer_heap, 0, machine->buffer_size);
            }
            break;
            
        case STATE_ERROR:
            printf("Error state reached\n");
            break;
            
        default:
            break;
    }
    
    return 1;
}

int main(void) {
    StateMachine machine;
    if (!initialize_machine(&machine)) {
        fprintf(stderr, "Failed to initialize state machine\n");
        return 1;
    }
    
    Transition transitions[] = {
        {STATE_IDLE, STATE_READING, 'r'},
        {STATE_READING, STATE_READING, 'r'},
        {STATE_READING, STATE_PROCESSING, 'p'},
        {STATE_PROCESSING, STATE_WRITING, 'w'},
        {STATE_WRITING, STATE_IDLE, 'i'},
        {STATE_READING, STATE_ERROR, 'e'},
        {STATE_PROCESSING, STATE_ERROR, 'e'},
        {STATE_WRITING, STATE_ERROR, 'e'},
        {STATE_ERROR, STATE_IDLE, 'i'}
    };
    
    int trans_count = sizeof(transitions) / sizeof(transitions[0]);
    
    printf("State Machine Controller\n");
    printf("Commands: r(read), p(process), w(write), i(idle), e(error), q(quit)\n");
    
    char input;
    while (1) {
        printf("Current state: ");
        switch (machine.current_state) {
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
        
        if (input == 'q') {
            break;
        }
        
        if (!