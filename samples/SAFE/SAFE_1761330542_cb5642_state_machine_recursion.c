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
    int count;
};

enum State handle_start(struct Context* ctx, char input);
enum State handle_reading(struct Context* ctx, char input);
enum State handle_processing(struct Context* ctx, char input);
enum State handle_finished(struct Context* ctx, char input);
enum State handle_error(struct Context* ctx, char input);

enum State process_state(struct Context* ctx, char input) {
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
        ctx->count = 0;
        ctx->value = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (input >= '0' && input <= '9') {
        if (ctx->position < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->position++] = input;
            ctx->buffer[ctx->position] = '\0';
            return READING;
        } else {
            return ERROR;
        }
    } else if (input == 'E') {
        if (ctx->position > 0) {
            ctx->buffer[ctx->position] = '\0';
            return PROCESSING;
        } else {
            return ERROR;
        }
    } else {
        return ERROR;
    }
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input == 'P') {
        char* endptr;
        long num = strtol(ctx->buffer, &endptr, 10);
        if (endptr != ctx->buffer && *endptr == '\0' && num >= 0 && num <= 1000) {
            ctx->value = (int)num;
            ctx->count = 0;
            return FINISHED;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void execute_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    if (input == NULL || input_len == 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_state = process_state(ctx, input[i]);
        if (ctx->current_state == ERROR) {
            break;
        }
    }
}

void recursive_output(struct Context* ctx, int depth) {
    if (depth <= 0 || ctx->current_state != FINISHED) {
        return;
    }
    
    if (ctx->count < ctx->value) {
        printf("Count: %d\n", ctx->count);
        ctx->count++;
        recursive_output(ctx, depth - 1);
    }
}

int main(void) {
    char input_sequence[512];
    
    printf("Enter state machine input sequence: ");
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_sequence);
    if (input_len > 0 && input_sequence[input_len - 1] == '\n') {
        input_sequence[--input_len] = '\0';
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    execute_state_machine(&ctx, input_sequence, input_len);
    
    if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully.\n");
        printf("Processing value: %d\n", ctx.value);
        printf("Recursive output:\n");
        recursive_output(&ctx, ctx.value + 1);
    } else {
        printf("State machine ended in error state.\n");
        return 1;
    }
    
    return 0;
}