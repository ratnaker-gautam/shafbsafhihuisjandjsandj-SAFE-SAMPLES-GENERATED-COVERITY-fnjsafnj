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
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current_state;
    State next_state;
    char trigger;
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

void destroy_stack(StateStack *stack) {
    if (!stack) return;
    free(stack->state_stack);
    free(stack);
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

int add_transition(TransitionTable *table, State current, State next, char trigger) {
    if (!table || table->transition_count >= table->transition_capacity) return 0;
    table->transitions[table->transition_count].current_state = current;
    table->transitions[table->transition_count].next_state = next;
    table->transitions[table->transition_count].trigger = trigger;
    table->transition_count++;
    return 1;
}

State process_transition(TransitionTable *table, State current, char trigger) {
    if (!table) return STATE_ERROR;
    for (int i = 0; i < table->transition_count; i++) {
        if (table->transitions[i].current_state == current && 
            table->transitions[i].trigger == trigger) {
            return table->transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

void destroy_transition_table(TransitionTable *table) {
    if (!table) return;
    free(table->transitions);
    free(table);
}

int main() {
    StateStack *stack = create_stack(MAX_STATES);
    if (!stack) {
        printf("Failed to create state stack\n");
        return 1;
    }

    TransitionTable *table = create_transition_table(MAX_TRANSITIONS);
    if (!table) {
        printf("Failed to create transition table\n");
        destroy_stack(stack);
        return 1;
    }

    add_transition(table, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(table, STATE_ACTIVE, STATE_PROCESSING, 'p');
    add_transition(table, STATE_PROCESSING, STATE_DONE, 'c');
    add_transition(table, STATE_PROCESSING, STATE_ERROR, 'e');
    add_transition(table, STATE_ERROR, STATE_IDLE, 'r');
    add_transition(table, STATE_DONE, STATE_IDLE, 'r');

    State current_state = STATE_IDLE;
    char input[BUFFER_SIZE];
    
    printf("State Machine Demo\n");
    printf("Commands: s(start), p(process), c(complete), e(error), r(reset), q(quit)\n");

    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_ACTIVE: printf("ACTIVE"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            case STATE_DONE: printf("DONE"); break;
        }
        printf("\nEnter command: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Input error\n");
            break;
        }

        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }

        if (strlen(input) != 2 || input[1] != '\n') {
            printf("Invalid input format\n");
            continue;
        }

        if (!push_state(stack, current_state)) {
            printf("Stack overflow\n");
            break;
        }

        State new_state = process_transition(table, current_state, input[0]);
        if (new_state == STATE_ERROR) {
            printf("Invalid transition from current state