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
        ctx->depth = 1;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->pos >= 255) return ERROR;
    
    if (input == '(') {
        ctx->buffer[ctx->pos++] = input;
        ctx->depth++;
    } else if (input == ')') {
        ctx->buffer[ctx->pos++] = input;
        ctx->depth--;
        if (ctx->depth == 0) return PARSING;
    } else if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->pos++] = input;
    } else if (input == ' ' || input == '\t') {
        return READING;
    } else {
        return ERROR;
    }
    
    return READING;
}

enum State handle_parsing(struct Context *ctx) {
    if (ctx->depth != 0) return ERROR;
    
    ctx->buffer[ctx->pos] = '\0';
    printf("Parsed expression: %s\n", ctx->buffer);
    return FINISHED;
}

enum State process_input(struct Context *ctx, char input) {
    switch (ctx->current) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PARSING:
            return handle_parsing(ctx);
        default:
            return ctx->current;
    }
}

void run_state_machine(struct Context *ctx, const char *input, int index) {
    if (ctx->current == FINISHED || ctx->current == ERROR) return;
    
    if (index >= strlen(input)) {
        if (ctx->current == PARSING) {
            ctx->current = handle_parsing(ctx);
        } else if (ctx->current != FINISHED) {
            ctx->current = ERROR;
        }
        return;
    }
    
    ctx->current = process_input(ctx, input[index]);
    run_state_machine(ctx, input, index + 1);
}

int main(void) {
    char input[512];
    
    printf("Enter a nested parentheses expression (numbers and parens only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Context ctx = { START, {0}, 0, 0 };
    run_state_machine(&ctx, input, 0);
    
    if (ctx.current == FINISHED) {
        printf("Successfully processed expression\n");
        return 0;
    } else {
        fprintf(stderr, "Error processing expression\n");
        return 1;
    }
}