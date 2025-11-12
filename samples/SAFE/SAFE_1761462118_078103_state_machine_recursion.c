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

enum State transition(struct Context* ctx, char input) {
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
        if (ctx->position > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->position] = input;
        ctx->position++;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != 'P') {
        return ERROR;
    }
    char* endptr;
    long val = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        return ERROR;
    }
    if (val < 0 || val > 10000) {
        return ERROR;
    }
    ctx->value = (int)val;
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        printf("Result: %d\n", ctx->value);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void process_input(struct Context* ctx, const char* input, size_t len) {
    if (input == NULL || len == 0) {
        return;
    }
    for (size_t i = 0; i < len; i++) {
        ctx->current_state = transition(ctx, input[i]);
        if (ctx->current_state == ERROR) {
            printf("Invalid input at position %zu\n", i);
            return;
        }
        if (ctx->current_state == FINISHED) {
            return;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    
    char input[512];
    printf("Enter sequence (format: B<digits>EPX): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    process_input(&ctx, input, len);
    return 0;
}