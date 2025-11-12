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

enum State handle_start(struct Context* ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->buffer[ctx->position++] = input;
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
    if ((input >= '0' && input <= '9') || (input >= 'A' && input <= 'Z')) {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx) {
    char* endptr;
    long val = strtol(ctx->buffer + 1, &endptr, 10);
    if (endptr == ctx->buffer + 1 || *endptr != '\0' || val < 0 || val > 1000) {
        return ERROR;
    }
    ctx->value = (int)val;
    ctx->count = 0;
    return FINISHED;
}

void process_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    if (input_len == 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (ctx->current_state == START) {
        ctx->current_state = handle_start(ctx, input[0]);
        if (input_len > 1) {
            process_state_machine(ctx, input + 1, input_len - 1);
        }
    } else if (ctx->current_state == READING) {
        ctx->current_state = handle_reading(ctx, input[0]);
        if (input_len > 1 && ctx->current_state != ERROR) {
            process_state_machine(ctx, input + 1, input_len - 1);
        }
    } else if (ctx->current_state == PROCESSING) {
        ctx->current_state = handle_processing(ctx);
    }
}

int recursive_count(int n, int depth) {
    if (depth <= 0 || n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return 1 + recursive_count(n / 2, depth - 1);
}

int main(void) {
    struct Context ctx;
    char input[512];
    
    printf("Enter sequence (format: B<digits>E): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[--len] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    
    process_state_machine(&ctx, input, len);
    
    if (ctx.current_state == FINISHED) {
        ctx.count = recursive_count(ctx.value, 10);
        printf("Processed value: %d\n", ctx.value);
        printf("Recursive count: %d\n", ctx.count);
    } else {
        fprintf(stderr, "State machine error\n");
        return 1;
    }
    
    return 0;
}