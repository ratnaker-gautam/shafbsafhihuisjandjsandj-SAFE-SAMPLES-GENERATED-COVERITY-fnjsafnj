//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    int position;
    int depth;
};

enum State process_start(struct Context *ctx, char input);
enum State process_reading(struct Context *ctx, char input);
enum State process_parsing(struct Context *ctx, char input);
enum State process_finished(struct Context *ctx, char input);
enum State process_error(struct Context *ctx, char input);

enum State transition(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case START:
            return process_start(ctx, input);
        case READING:
            return process_reading(ctx, input);
        case PARSING:
            return process_parsing(ctx, input);
        case FINISHED:
            return process_finished(ctx, input);
        case ERROR:
            return process_error(ctx, input);
        default:
            return ERROR;
    }
}

enum State process_start(struct Context *ctx, char input) {
    if (input == '(') {
        ctx->buffer[0] = input;
        ctx->position = 1;
        ctx->depth = 1;
        return READING;
    }
    return ERROR;
}

enum State process_reading(struct Context *ctx, char input) {
    if (ctx->position >= 255) {
        return ERROR;
    }
    if (input == '(') {
        ctx->depth++;
    } else if (input == ')') {
        ctx->depth--;
        if (ctx->depth == 0) {
            ctx->buffer[ctx->position] = '\0';
            return PARSING;
        }
    }
    ctx->buffer[ctx->position++] = input;
    return READING;
}

enum State process_parsing(struct Context *ctx, char input) {
    if (input == '\n' || input == '\0') {
        return FINISHED;
    }
    return ERROR;
}

enum State process_finished(struct Context *ctx, char input) {
    return FINISHED;
}

enum State process_error(struct Context *ctx, char input) {
    return ERROR;
}

void recursive_state_machine(struct Context *ctx, const char *input, int index) {
    if (input[index] == '\0') {
        return;
    }
    if (index >= 1000) {
        ctx->current_state = ERROR;
        return;
    }
    ctx->current_state = transition(ctx, input[index]);
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    recursive_state_machine(ctx, input, index + 1);
}

int main(void) {
    struct Context ctx;
    char input[1024];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    ctx.current_state = START;
    ctx.position = 0;
    ctx.depth = 0;
    
    recursive_state_machine(&ctx, input, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("Valid expression: %s\n", ctx.buffer);
        return EXIT_SUCCESS;
    } else {
        printf("Invalid expression\n");
        return EXIT_FAILURE;
    }
}