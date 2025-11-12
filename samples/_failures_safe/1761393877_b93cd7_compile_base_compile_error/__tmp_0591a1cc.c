//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_SIZE 100

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    State next_state;
    char trigger_char;
} Transition;

typedef struct {
    State *state_stack;
    int stack_top;
    int stack_capacity;
} StateStack;

typedef struct {
    Transition *transitions;
    int transition_count;
    int transition_capacity;
} TransitionTable;

StateStack* create_stack(int capacity) {
    if (capacity <= 0) return NULL;
    StateStack *stack = malloc(sizeof(StateStack));
    if (!stack) return NULL;
    stack->state_stack = malloc(sizeof(State) * capacity);
    if (!stack->state_stack) {
        free(stack);
        return NULL;
    }
    stack->stack_top = -1;
    stack->stack_capacity = capacity;
    return stack;
}

void destroy_stack(StateStack *stack) {
    if (stack) {
        free(stack->state_stack);
        free(stack);
    }
}

int push_state(StateStack *stack, State state) {
    if (!stack || stack->stack_top >= stack->stack_capacity - 1) return 0;
    stack->state_stack[++stack->stack_top] = state;
    return 1;
}

int pop_state(StateStack *stack, State *state) {
    if (!stack || !state || stack->stack_top < 0) return 0;
    *state = stack->state_stack[stack->stack_top--];
    return 1;
}

TransitionTable* create_transition_table(int capacity) {
    if (capacity <= 0) return NULL;
    TransitionTable *table = malloc(sizeof(TransitionTable));
    if (!table) return NULL;
    table->transitions = malloc(sizeof(Transition) * capacity);
    if (!table->transitions) {
        free(table);
        return NULL;
    }
    table->transition_count = 0;
    table->transition_capacity = capacity;
    return table;
}

void destroy_transition_table(TransitionTable *table) {
    if (table) {
        free(table->transitions);
        free(table);
    }
}

int add_transition(TransitionTable *table, State current, State next, char trigger) {
    if (!table || table->transition_count >= table->transition_capacity) return 0;
    table->transitions[table->transition_count].current_state = current;
    table->transitions[table->transition_count].next_state = next;
    table->transitions[table->transition_count].trigger_char = trigger;
    table->transition_count++;
    return 1;
}

State process_transition(TransitionTable *table, State current, char input) {
    if (!table) return current;
    for (int i = 0; i < table->transition_count; i++) {
        if (table->transitions[i].current_state == current && 
            table->transitions[i].trigger_char == input) {
            return table->transitions[i].next_state;
        }
    }
    return current;
}

void initialize_transitions(TransitionTable *table) {
    if (!table) return;
    add_transition(table, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(table, STATE_ACTIVE, STATE_PROCESSING, 'p');
    add_transition(table, STATE_PROCESSING, STATE_COMPLETE, 'c');
    add_transition(table, STATE_PROCESSING, STATE_ERROR, 'e');
    add_transition(table, STATE_ERROR, STATE_IDLE, 'r');
    add_transition(table, STATE_COMPLETE, STATE_IDLE, 'r');
}

int main(void) {
    StateStack *stack = create_stack(MAX_STATES);
    if (!stack) {
        fprintf(stderr, "Failed to create state stack\n");
        return 1;
    }

    TransitionTable *transitions = create_transition_table(MAX_TRANSITIONS);
    if (!transitions) {
        fprintf(stderr, "Failed to create transition table\n");
        destroy_stack(stack);
        return 1;
    }

    initialize_transitions(transitions);
    
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_SIZE];
    
    printf("State Machine Started (IDLE state)\n");
    printf("Available transitions:\n");
    printf("s: IDLE -> ACTIVE\n");
    printf("p: ACTIVE -> PROCESSING\n");
    printf("c: PROCESSING -> COMPLETE\n");
    printf("e: PROCESSING -> ERROR\n");
    printf("r: ERROR/COMPLETE -> IDLE\n");
    printf("q: Quit\n\n");

    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_ACTIVE: printf("ACTIVE"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter transition character: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            break;
        }

        if (input_buffer[0] == 'q') {