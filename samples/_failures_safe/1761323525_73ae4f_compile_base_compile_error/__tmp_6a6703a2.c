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
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State *stack;
    int top;
    int capacity;
} StateStack;

typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} HeapBuffer;

StateStack* create_stack(int capacity) {
    if (capacity <= 0 || capacity > STACK_SIZE) {
        return NULL;
    }
    StateStack *stack = malloc(sizeof(StateStack));
    if (!stack) return NULL;
    stack->stack = malloc(sizeof(State) * capacity);
    if (!stack->stack) {
        free(stack);
        return NULL;
    }
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

int push(StateStack *stack, State state) {
    if (!stack || stack->top >= stack->capacity - 1) {
        return 0;
    }
    stack->stack[++stack->top] = state;
    return 1;
}

int pop(StateStack *stack, State *state) {
    if (!stack || !state || stack->top < 0) {
        return 0;
    }
    *state = stack->stack[stack->top--];
    return 1;
}

void destroy_stack(StateStack *stack) {
    if (stack) {
        free(stack->stack);
        free(stack);
    }
}

HeapBuffer* create_buffer(size_t capacity) {
    if (capacity == 0 || capacity > HEAP_BUFFER_SIZE) {
        return NULL;
    }
    HeapBuffer *buffer = malloc(sizeof(HeapBuffer));
    if (!buffer) return NULL;
    buffer->buffer = malloc(capacity);
    if (!buffer->buffer) {
        free(buffer);
        return NULL;
    }
    buffer->size = 0;
    buffer->capacity = capacity;
    return buffer;
}

int append_to_buffer(HeapBuffer *buffer, const char *data, size_t len) {
    if (!buffer || !data || len == 0) {
        return 0;
    }
    if (buffer->size + len > buffer->capacity) {
        return 0;
    }
    memcpy(buffer->buffer + buffer->size, data, len);
    buffer->size += len;
    return 1;
}

void destroy_buffer(HeapBuffer *buffer) {
    if (buffer) {
        free(buffer->buffer);
        free(buffer);
    }
}

State process_input(State current, char input, const Transition *transitions, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

int main(void) {
    Transition transitions[] = {
        {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
        {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_A},
        {STATE_E, '0', STATE_B}, {STATE_E, '1', STATE_C},
        {STATE_F, '0', STATE_D}, {STATE_F, '1', STATE_E},
        {STATE_G, '0', STATE_F}, {STATE_G, '1', STATE_G},
        {STATE_H, '0', STATE_H}, {STATE_H, '1', STATE_A}
    };
    int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

    StateStack *stack = create_stack(STACK_SIZE);
    if (!stack) {
        fprintf(stderr, "Failed to create stack\n");
        return 1;
    }

    HeapBuffer *input_buffer = create_buffer(HEAP_BUFFER_SIZE);
    if (!input_buffer) {
        fprintf(stderr, "Failed to create buffer\n");
        destroy_stack(stack);
        return 1;
    }

    printf("Enter input sequence (max %d chars, 0/1 only): ", MAX_INPUT_SIZE);
    char input_line[MAX_INPUT_SIZE + 2];
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        fprintf(stderr, "Failed to read input\n");
        destroy_stack(stack);
        destroy_buffer(input_buffer);
        return 1;
    }

    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }

    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        destroy_stack(stack);
        destroy_buffer(input_buffer);
        return 1;
    }

    if (!append_to_buffer(input_buffer, input_line, input_len)) {
        fprintf(stderr, "Failed to store input\n");
        destroy_stack(stack);
        destroy_buffer(input_buffer);
        return 1;
    }

    State current_state = STATE_A;