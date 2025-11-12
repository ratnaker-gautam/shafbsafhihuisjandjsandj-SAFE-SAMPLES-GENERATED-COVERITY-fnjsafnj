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

enum State transition(struct Context* ctx, char input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
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
        if (ctx->position < 255) {
            ctx->buffer[ctx->position++] = input;
            return READING;
        }
        return ERROR;
    } else if (input == 'E') {
        if (ctx->position > 0) {
            ctx->buffer[ctx->position] = '\0';
            return PROCESSING;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input == 'P') {
        char* endptr;
        long num = strtol(ctx->buffer, &endptr, 10);
        if (endptr != ctx->buffer && *endptr == '\0' && num >= 0 && num <= 1000) {
            ctx->value = (int)num;
            ctx->count = 0;
            return FINISHED;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'C') {
        ctx->count++;
        if (ctx->count < ctx->value) {
            return FINISHED;
        }
        return START;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void process_input(struct Context* ctx, const char* input, size_t len) {
    if (len == 0) return;
    
    enum State new_state = transition(ctx, input[0]);
    ctx->current_state = new_state;
    
    if (len > 1) {
        process_input(ctx, input + 1, len - 1);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    
    char input[512];
    printf("Enter sequence: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        return 1;
    }
    
    process_input(&ctx, input, len);
    
    if (ctx.current_state == START) {
        printf("Success: Processed %d items\n", ctx.value);
    } else if (ctx.current_state == ERROR) {
        printf("Error: Invalid sequence\n");
    } else {
        printf("Warning: Incomplete sequence\n");
    }
    
    return 0;
}