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
    if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    } else if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        return PROCESSING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->position > 1) {
        char* endptr;
        long num = strtol(ctx->buffer + 1, &endptr, 10);
        if (endptr != ctx->buffer + ctx->position - 1) {
            return ERROR;
        }
        if (num < 0 || num > 10000) {
            return ERROR;
        }
        ctx->value = (int)num;
        return FINISHED;
    }
    return ERROR;
}

void process_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    if (input_len == 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < input_len && ctx->current_state != FINISHED && ctx->current_state != ERROR; i++) {
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx, input[i]);
                break;
            case READING:
                ctx->current_state = handle_reading(ctx, input[i]);
                break;
            case PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
    
    if (ctx->current_state == PROCESSING) {
        ctx->current_state = handle_processing(ctx);
    }
}

void recursive_output(int value, int depth) {
    if (depth <= 0) {
        return;
    }
    
    if (depth == 1) {
        printf("%d", value);
    } else {
        printf("%d, ", value);
        recursive_output(value - 1, depth - 1);
    }
}

int main(void) {
    char input[512];
    printf("Enter sequence (format: B<digits>E): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    
    process_state_machine(&ctx, input, len);
    
    if (ctx.current_state == FINISHED) {
        printf("Parsed value: %d\n", ctx.value);
        printf("Countdown: ");
        recursive_output(ctx.value, ctx.value);
        printf("\n");
        return 0;
    } else {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
}