//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUT_SIZE 64
#define STACK_SIZE 32
#define HEAP_BUFFER_SIZE 128

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
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
} StateMachine;

typedef struct {
    StateMachine* heap_machine;
    char stack_buffer[STACK_SIZE];
    int stack_top;
    char* heap_buffer;
} Context;

void initialize_machine(StateMachine* machine) {
    if (machine == NULL) return;
    
    machine->state_count = 0;
    machine->transition_count = 0;
    machine->states[machine->state_count++] = STATE_IDLE;
    machine->states[machine->state_count++] = STATE_READING;
    machine->states[machine->state_count++] = STATE_PROCESSING;
    machine->states[machine->state_count++] = STATE_WRITING;
    machine->states[machine->state_count++] = STATE_ERROR;
    machine->states[machine->state_count++] = STATE_DONE;
    
    Transition* t = &machine->transitions[machine->transition_count++];
    t->current = STATE_IDLE;
    t->next = STATE_READING;
    t->trigger = 'r';
    
    t = &machine->transitions[machine->transition_count++];
    t->current = STATE_READING;
    t->next = STATE_PROCESSING;
    t->trigger = 'p';
    
    t = &machine->transitions[machine->transition_count++];
    t->current = STATE_PROCESSING;
    t->next = STATE_WRITING;
    t->trigger = 'w';
    
    t = &machine->transitions[machine->transition_count++];
    t->current = STATE_WRITING;
    t->next = STATE_DONE;
    t->trigger = 'd';
    
    t = &machine->transitions[machine->transition_count++];
    t->current = STATE_READING;
    t->next = STATE_ERROR;
    t->trigger = 'e';
    
    t = &machine->transitions[machine->transition_count++];
    t->current = STATE_PROCESSING;
    t->next = STATE_ERROR;
    t->trigger = 'e';
    
    t = &machine->transitions[machine->transition_count++];
    t->current = STATE_ERROR;
    t->next = STATE_IDLE;
    t->trigger = 'r';
}

State process_transition(StateMachine* machine, State current, char input) {
    if (machine == NULL) return STATE_ERROR;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == current && 
            machine->transitions[i].trigger == input) {
            return machine->transitions[i].next;
        }
    }
    return current;
}

void process_state_action(Context* ctx, State state) {
    if (ctx == NULL) return;
    
    switch (state) {
        case STATE_IDLE:
            printf("State: IDLE - Ready to start\n");
            break;
        case STATE_READING:
            printf("State: READING - Processing input data\n");
            if (ctx->heap_buffer != NULL) {
                strncpy(ctx->heap_buffer, "Reading data", HEAP_BUFFER_SIZE - 1);
                ctx->heap_buffer[HEAP_BUFFER_SIZE - 1] = '\0';
            }
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING - Analyzing data\n");
            if (ctx->stack_top < STACK_SIZE - 1) {
                ctx->stack_buffer[ctx->stack_top++] = 'P';
                ctx->stack_buffer[ctx->stack_top] = '\0';
            }
            break;
        case STATE_WRITING:
            printf("State: WRITING - Outputting results\n");
            if (ctx->heap_buffer != NULL && ctx->stack_top > 0) {
                printf("Output: %s with stack mark\n", ctx->heap_buffer);
            }
            break;
        case STATE_ERROR:
            printf("State: ERROR - Operation failed\n");
            break;
        case STATE_DONE:
            printf("State: DONE - Operation completed\n");
            break;
        default:
            printf("State: UNKNOWN\n");
            break;
    }
}

int main() {
    Context ctx;
    StateMachine machine_stack;
    State current_state = STATE_IDLE;
    char input[MAX_INPUT_SIZE];
    
    ctx.heap_machine = malloc(sizeof(StateMachine));
    if (ctx.heap_machine == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    ctx.heap_buffer = malloc(HEAP_BUFFER_SIZE);
    if (ctx.heap_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(ctx.heap_machine);
        return 1;
    }
    
    ctx.stack_top = 0;
    memset(ctx.stack_buffer, 0, STACK_SIZE);
    memset(ctx.heap_buffer, 0, HEAP_BUFFER_SIZE);
    
    initialize_machine(&