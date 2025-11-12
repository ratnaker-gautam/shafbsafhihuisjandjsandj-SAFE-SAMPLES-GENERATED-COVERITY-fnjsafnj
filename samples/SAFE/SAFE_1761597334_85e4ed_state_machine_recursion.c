//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int value;
};

enum State handle_start(struct Context* ctx, char input);
enum State handle_reading(struct Context* ctx, char input);
enum State handle_parsing(struct Context* ctx, char input);
enum State handle_finished(struct Context* ctx, char input);
enum State handle_error(struct Context* ctx, char input);

enum State process_input(struct Context* ctx, char input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PARSING: return handle_parsing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context* ctx, char input) {
    if (input == ' ') return START;
    if (input >= '0' && input <= '9') {
        ctx->buffer[0] = input;
        ctx->position = 1;
        ctx->buffer[ctx->position] = '\0';
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (input >= '0' && input <= '9') {
        if (ctx->position >= 255) return ERROR;
        ctx->buffer[ctx->position++] = input;
        ctx->buffer[ctx->position] = '\0';
        return READING;
    }
    if (input == ' ' || input == '\n') {
        char* endptr;
        long val = strtol(ctx->buffer, &endptr, 10);
        if (endptr == ctx->buffer || *endptr != '\0') return ERROR;
        if (val < -2147483648 || val > 2147483647) return ERROR;
        ctx->value = (int)val;
        return PARSING;
    }
    return ERROR;
}

enum State handle_parsing(struct Context* ctx, char input) {
    if (input == ' ') return PARSING;
    if (input == '\n') return FINISHED;
    return ERROR;
}

enum State handle_finished(struct Context* ctx, char input) {
    return FINISHED;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void process_recursive(struct Context* ctx) {
    int c = getchar();
    if (c == EOF) return;
    
    char input_char = (char)c;
    enum State next_state = process_input(ctx, input_char);
    ctx->current_state = next_state;
    
    process_recursive(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("Enter a number followed by newline: ");
    fflush(stdout);
    
    process_recursive(&ctx);
    
    if (ctx.current_state == FINISHED) {
        printf("Parsed value: %d\n", ctx.value);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Invalid input\n");
        return EXIT_FAILURE;
    }
}