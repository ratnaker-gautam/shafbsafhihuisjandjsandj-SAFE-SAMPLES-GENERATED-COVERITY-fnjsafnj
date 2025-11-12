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

enum State handle_start(struct Context *ctx, char input);
enum State handle_reading(struct Context *ctx, char input);
enum State handle_parsing(struct Context *ctx, char input);
enum State handle_finished(struct Context *ctx, char input);
enum State handle_error(struct Context *ctx, char input);

enum State process_input(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PARSING:
            return handle_parsing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, char input) {
    if (input == '(') {
        ctx->buffer[0] = input;
        ctx->position = 1;
        ctx->depth = 1;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->position >= 255) {
        return ERROR;
    }
    if (input == '(') {
        ctx->depth++;
        ctx->buffer[ctx->position++] = input;
        return READING;
    } else if (input == ')') {
        ctx->depth--;
        ctx->buffer[ctx->position++] = input;
        if (ctx->depth == 0) {
            ctx->buffer[ctx->position] = '\0';
            return FINISHED;
        }
        return READING;
    } else if (input >= 'a' && input <= 'z') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_parsing(struct Context *ctx, char input) {
    return ERROR;
}

enum State handle_finished(struct Context *ctx, char input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, char input) {
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
    ctx->current_state = process_input(ctx, input[index]);
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    recursive_state_machine(ctx, input, index + 1);
}

int main(void) {
    char user_input[1024];
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    if (strlen(user_input) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    struct Context ctx = {START, {0}, 0, 0};
    recursive_state_machine(&ctx, user_input, 0);
    if (ctx.current_state == FINISHED) {
        printf("Valid expression: %s\n", ctx.buffer);
        return EXIT_SUCCESS;
    } else {
        printf("Invalid expression\n");
        return EXIT_FAILURE;
    }
}