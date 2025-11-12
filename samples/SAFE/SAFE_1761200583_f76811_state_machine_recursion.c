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

enum State handle_start(struct Context *ctx, char input);
enum State handle_reading(struct Context *ctx, char input);
enum State handle_processing(struct Context *ctx, char input);
enum State handle_finished(struct Context *ctx, char input);
enum State handle_error(struct Context *ctx, char input);

enum State transition(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->position >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    } else if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        if (ctx->position > 0) {
            char *endptr;
            long num = strtol(ctx->buffer, &endptr, 10);
            if (endptr != ctx->buffer && *endptr == '\0' && num >= 0 && num <= 1000) {
                ctx->value = (int)num;
                return PROCESSING;
            }
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, char input) {
    if (input == 'C') {
        if (ctx->value > 0) {
            ctx->value *= 2;
            if (ctx->value > 1000) {
                ctx->value = 1000;
            }
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, char input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, char input) {
    return ERROR;
}

void process_input_recursive(struct Context *ctx, const char *input, size_t index) {
    if (input == NULL || index >= strlen(input)) {
        return;
    }
    ctx->current_state = transition(ctx, input[index]);
    process_input_recursive(ctx, input, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    char input_line[512];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
    }
    
    process_input_recursive(&ctx, input_line, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("SUCCESS: %d\n", ctx.value);
        return EXIT_SUCCESS;
    } else {
        printf("ERROR\n");
        return EXIT_FAILURE;
    }
}