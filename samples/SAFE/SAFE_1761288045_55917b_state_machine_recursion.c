//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int value;
};

enum State handle_start(struct Context* ctx, char input);
enum State handle_reading(struct Context* ctx, char input);
enum State handle_processing(struct Context* ctx, char input);
enum State handle_finished(struct Context* ctx, char input);
enum State handle_error(struct Context* ctx, char input);

enum State process_input(struct Context* ctx, char input) {
    if (ctx == NULL) return ERROR;
    
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context* ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (ctx->position >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    
    if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        return PROCESSING;
    }
    
    if ((input >= '0' && input <= '9') || input == '-') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != 'P') return ERROR;
    
    char* endptr;
    long num = strtol(ctx->buffer, &endptr, 10);
    
    if (endptr == ctx->buffer || *endptr != '\0') {
        return ERROR;
    }
    
    if (num < -1000 || num > 1000) {
        return ERROR;
    }
    
    ctx->value = (int)num;
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        printf("Result: %d\n", ctx->value * 2);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void run_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    if (ctx == NULL || input == NULL) return;
    
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_state = process_input(ctx, input[i]);
        if (ctx->current_state == ERROR) {
            printf("Invalid input at position %zu\n", i);
            return;
        }
    }
    
    if (ctx->current_state == FINISHED) {
        printf("Processing completed successfully\n");
    } else {
        printf("Incomplete input sequence\n");
    }
}

void process_recursive(struct Context* ctx, const char* input, size_t index, size_t max_len) {
    if (ctx == NULL || input == NULL || index >= max_len) {
        return;
    }
    
    ctx->current_state = process_input(ctx, input[index]);
    
    if (ctx->current_state == ERROR) {
        printf("Error at position %zu\n", index);
        return;
    }
    
    process_recursive(ctx, input, index + 1, max_len);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    
    char test_input[] = "B123E PX";
    size_t input_len = strlen(test_input);
    
    printf("Testing iterative state machine:\n");
    run_state_machine(&ctx, test_input, input_len);
    
    printf("\nTesting recursive state machine:\n");
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    process_recursive(&ctx, test_input, 0, input_len);
    
    return 0;
}