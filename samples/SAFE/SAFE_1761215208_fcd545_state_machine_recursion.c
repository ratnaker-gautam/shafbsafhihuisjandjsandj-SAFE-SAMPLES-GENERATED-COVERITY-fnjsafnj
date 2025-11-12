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

enum State handle_start(struct Context *ctx, char input);
enum State handle_reading(struct Context *ctx, char input);
enum State handle_processing(struct Context *ctx, char input);
enum State handle_finished(struct Context *ctx, char input);
enum State handle_error(struct Context *ctx, char input);

enum State process_input(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->buffer[0] = '\0';
        ctx->value = 0;
        ctx->count = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (input >= '0' && input <= '9') {
        if (ctx->position < 255) {
            ctx->buffer[ctx->position++] = input;
            ctx->buffer[ctx->position] = '\0';
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

enum State handle_processing(struct Context *ctx, char input) {
    if (input == '\n' || input == '\0') {
        char *endptr;
        long num = strtol(ctx->buffer, &endptr, 10);
        if (endptr != ctx->buffer && *endptr == '\0' && num >= 0 && num <= 1000) {
            ctx->value += (int)num;
            ctx->count++;
            return FINISHED;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, char input) {
    if (input == 'C') {
        return START;
    } else if (input == 'Q') {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, char input) {
    return ERROR;
}

void run_state_machine(struct Context *ctx, const char *input, size_t index) {
    if (index >= strlen(input)) {
        return;
    }
    
    char current_char = input[index];
    enum State next_state = process_input(ctx, current_char);
    ctx->current_state = next_state;
    
    run_state_machine(ctx, input, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    
    char input[512];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    run_state_machine(&ctx, input, 0);
    
    if (ctx.current_state == FINISHED && ctx.count > 0) {
        printf("Success: Processed %d numbers, total: %d, average: %.2f\n", 
               ctx.count, ctx.value, (double)ctx.value / ctx.count);
        return 0;
    } else {
        printf("Error: Invalid input sequence\n");
        return 1;
    }
}