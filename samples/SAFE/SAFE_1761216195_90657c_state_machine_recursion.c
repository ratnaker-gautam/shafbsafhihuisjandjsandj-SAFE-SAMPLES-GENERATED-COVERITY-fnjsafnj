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
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != 'P') {
        return ERROR;
    }
    char* endptr;
    long num = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        return ERROR;
    }
    if (num < 0 || num > 10000) {
        return ERROR;
    }
    ctx->value = (int)num;
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        printf("Processed value: %d\n", ctx->value);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void run_state_machine(struct Context* ctx, const char* input, size_t index) {
    if (index >= strlen(input)) {
        return;
    }
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    char current_char = input[index];
    enum State next_state = process_input(ctx, current_char);
    ctx->current_state = next_state;
    run_state_machine(ctx, input, index + 1);
}

int main(void) {
    char user_input[512];
    printf("Enter sequence: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    if (strlen(user_input) == 0) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    struct Context ctx = {START, "", 0, 0};
    run_state_machine(&ctx, user_input, 0);
    if (ctx.current_state != FINISHED) {
        printf("Invalid sequence\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}