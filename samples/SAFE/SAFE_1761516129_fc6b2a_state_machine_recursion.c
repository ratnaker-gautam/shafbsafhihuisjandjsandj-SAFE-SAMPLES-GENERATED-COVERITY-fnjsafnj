//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, FINISHED, ERROR };

struct Context {
    enum State current;
    char buffer[256];
    int pos;
    int depth;
};

enum State handle_start(struct Context *ctx, char input) {
    if (input == '(') {
        ctx->buffer[ctx->pos++] = input;
        ctx->depth++;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->pos >= 255) return ERROR;
    
    if (input == '(') {
        ctx->buffer[ctx->pos++] = input;
        ctx->depth++;
        return READING;
    } else if (input == ')') {
        ctx->buffer[ctx->pos++] = input;
        ctx->depth--;
        if (ctx->depth == 0) {
            return FINISHED;
        }
        return READING;
    } else if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z')) {
        ctx->buffer[ctx->pos++] = input;
        return READING;
    }
    return ERROR;
}

enum State process_input(struct Context *ctx, const char *input, int index) {
    if (ctx->current == FINISHED || ctx->current == ERROR) {
        return ctx->current;
    }
    
    if (input[index] == '\0') {
        if (ctx->depth == 0 && ctx->current != START) {
            return FINISHED;
        }
        return ERROR;
    }
    
    char c = input[index];
    
    switch (ctx->current) {
        case START:
            ctx->current = handle_start(ctx, c);
            break;
        case READING:
            ctx->current = handle_reading(ctx, c);
            break;
        default:
            ctx->current = ERROR;
            break;
    }
    
    return process_input(ctx, input, index + 1);
}

int validate_parentheses(const char *input) {
    if (input == NULL) return 0;
    
    struct Context ctx;
    ctx.current = START;
    ctx.pos = 0;
    ctx.depth = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    enum State result = process_input(&ctx, input, 0);
    
    if (result == FINISHED) {
        printf("Valid expression: %s\n", ctx.buffer);
        return 1;
    } else {
        printf("Invalid expression\n");
        return 0;
    }
}

int main() {
    char input[512];
    
    printf("Enter a parenthesized expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    return validate_parentheses(input) ? 0 : 1;
}