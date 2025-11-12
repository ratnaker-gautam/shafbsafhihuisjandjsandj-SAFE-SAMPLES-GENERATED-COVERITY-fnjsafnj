//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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

enum State process_state(struct Context* ctx, char input) {
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
        ctx->current_state = process_state(ctx, input[i]);
        if (ctx->current_state == ERROR) {
            printf("Invalid input at position %zu\n", i);
            return;
        }
    }
    
    if (ctx->current_state != FINISHED) {
        printf("Incomplete input sequence\n");
    }
}

void process_input_recursive(struct Context* ctx, const char* input, size_t pos, size_t len) {
    if (ctx == NULL || input == NULL || pos >= len) return;
    
    ctx->current_state = process_state(ctx, input[pos]);
    
    if (ctx->current_state == ERROR) {
        printf("Invalid input at position %zu\n", pos);
        return;
    }
    
    process_input_recursive(ctx, input, pos + 1, len);
    
    if (pos == len - 1 && ctx->current_state != FINISHED) {
        printf("Incomplete input sequence\n");
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    char input[512];
    printf("Enter input sequence: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_input_recursive(&ctx, input, 0, len);
    
    return 0;
}