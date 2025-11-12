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
    if (ctx == NULL) return ERROR;
    
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
        ctx->buffer[0] = '\0';
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (ctx->position >= 255) return ERROR;
    
    if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->position++] = input;
        ctx->buffer[ctx->position] = '\0';
        return READING;
    } else if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        if (ctx->position > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != 'P') return ERROR;
    
    char* endptr;
    long num = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') return ERROR;
    if (num < 0 || num > 10000) return ERROR;
    
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

void process_input_recursive(struct Context* ctx, const char* input, size_t index) {
    if (ctx == NULL || input == NULL) return;
    if (index >= strlen(input)) return;
    
    ctx->current_state = transition(ctx, input[index]);
    
    if (ctx->current_state == ERROR) {
        printf("Invalid input sequence\n");
        return;
    }
    
    process_input_recursive(ctx, input, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.buffer[0] = '\0';
    
    char input[512];
    printf("Enter sequence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_input_recursive(&ctx, input, 0);
    
    if (ctx.current_state != FINISHED) {
        printf("Processing failed\n");
        return 1;
    }
    
    return 0;
}