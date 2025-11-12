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

enum State handle_start(struct Context *ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
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

enum State handle_processing(struct Context *ctx) {
    char *endptr;
    long num = strtol(ctx->buffer + 1, &endptr, 10);
    if (endptr == ctx->buffer + 1 || *endptr != 'E' || num < 0 || num > 1000) {
        return ERROR;
    }
    ctx->value = (int)num;
    ctx->count = 0;
    return FINISHED;
}

enum State handle_finished(struct Context *ctx) {
    if (ctx->count < ctx->value) {
        ctx->count++;
        return FINISHED;
    }
    return START;
}

enum State process_input(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return ERROR;
    }
    return ERROR;
}

void recursive_state_machine(struct Context *ctx, const char *input, size_t index) {
    if (index >= strlen(input)) {
        return;
    }
    
    ctx->current_state = process_input(ctx, input[index]);
    
    if (ctx->current_state == ERROR) {
        printf("Error at position %zu\n", index);
        return;
    }
    
    if (ctx->current_state == FINISHED) {
        printf("Processed value: %d\n", ctx->value);
    }
    
    recursive_state_machine(ctx, input, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    
    char input[512];
    printf("Enter input sequence: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
    
    recursive_state_machine(&ctx, input, 0);
    
    if (ctx.current_state != START && ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Incomplete input sequence\n");
    }
    
    return 0;
}