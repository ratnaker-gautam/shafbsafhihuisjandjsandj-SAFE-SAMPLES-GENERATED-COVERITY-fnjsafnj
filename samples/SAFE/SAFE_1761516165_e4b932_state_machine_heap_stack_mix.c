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
    char condition;
} Transition;

typedef struct {
    State state;
    char* buffer;
    size_t buffer_size;
    int data;
} StateContext;

typedef struct {
    Transition* transitions;
    size_t count;
    size_t capacity;
} TransitionTable;

void init_transition_table(TransitionTable* table) {
    table->capacity = MAX_TRANSITIONS;
    table->count = 0;
    table->transitions = malloc(table->capacity * sizeof(Transition));
    if (!table->transitions) exit(EXIT_FAILURE);
}

void add_transition(TransitionTable* table, State current, State next, char condition) {
    if (table->count >= table->capacity) return;
    table->transitions[table->count].current = current;
    table->transitions[table->count].next = next;
    table->transitions[table->count].condition = condition;
    table->count++;
}

State get_next_state(TransitionTable* table, State current, char condition) {
    for (size_t i = 0; i < table->count; i++) {
        if (table->transitions[i].current == current && 
            table->transitions[i].condition == condition) {
            return table->transitions[i].next;
        }
    }
    return current;
}

void init_context(StateContext* context) {
    context->state = STATE_IDLE;
    context->buffer_size = BUFFER_SIZE;
    context->buffer = malloc(context->buffer_size);
    if (!context->buffer) exit(EXIT_FAILURE);
    memset(context->buffer, 0, context->buffer_size);
    context->data = 0;
}

void cleanup_context(StateContext* context) {
    if (context->buffer) {
        free(context->buffer);
        context->buffer = NULL;
    }
}

void process_state(StateContext* context, TransitionTable* table) {
    char input;
    printf("Current state: %d. Enter command (a=activate, p=pause, r=resume, e=error, d=done, q=quit): ", context->state);
    
    if (scanf(" %c", &input) != 1) {
        while (getchar() != '\n');
        return;
    }
    
    if (input == 'q') {
        context->state = STATE_DONE;
        return;
    }
    
    State next = get_next_state(table, context->state, input);
    
    switch (next) {
        case STATE_ACTIVE:
            if (context->state != STATE_ACTIVE) {
                printf("Activating...\n");
                context->data = 100;
            }
            break;
        case STATE_PAUSED:
            if (context->state != STATE_PAUSED) {
                printf("Pausing...\n");
                context->data /= 2;
            }
            break;
        case STATE_ERROR:
            printf("Error state!\n");
            context->data = -1;
            break;
        case STATE_DONE:
            printf("Completion state!\n");
            break;
        default:
            break;
    }
    
    context->state = next;
    
    if (context->buffer && context->data > 0) {
        snprintf(context->buffer, context->buffer_size, "State: %d, Data: %d", context->state, context->data);
        printf("%s\n", context->buffer);
    }
}

int main(void) {
    TransitionTable table;
    StateContext context;
    
    init_transition_table(&table);
    init_context(&context);
    
    add_transition(&table, STATE_IDLE, STATE_ACTIVE, 'a');
    add_transition(&table, STATE_IDLE, STATE_ERROR, 'e');
    add_transition(&table, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(&table, STATE_ACTIVE, STATE_ERROR, 'e');
    add_transition(&table, STATE_ACTIVE, STATE_DONE, 'd');
    add_transition(&table, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(&table, STATE_PAUSED, STATE_ERROR, 'e');
    add_transition(&table, STATE_ERROR, STATE_IDLE, 'a');
    add_transition(&table, STATE_ERROR, STATE_DONE, 'd');
    
    printf("State Machine Demo\n");
    
    while (context.state != STATE_DONE) {
        process_state(&context, &table);
    }
    
    cleanup_context(&context);
    free(table.transitions);
    
    printf("Program completed.\n");
    return EXIT_SUCCESS;
}