//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int result;
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
    
    if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        return PROCESSING;
    }
    
    if ((input >= '0' && input <= '9') || input == ' ') {
        ctx->buffer[ctx->position] = input;
        ctx->position++;
        return READING;
    }
    
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != 'P') return ERROR;
    
    int sum = 0;
    int num = 0;
    int has_digit = 0;
    
    for (size_t i = 0; i < ctx->position; i++) {
        char c = ctx->buffer[i];
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            has_digit = 1;
        } else if (c == ' ') {
            if (has_digit) {
                sum += num;
                num = 0;
                has_digit = 0;
            }
        }
    }
    
    if (has_digit) {
        sum += num;
    }
    
    ctx->result = sum;
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        printf("Result: %d\n", ctx->result);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void process_input_recursive(struct Context* ctx, const char* input, size_t index) {
    if (ctx == NULL || input == NULL) return;
    
    if (index >= strlen(input)) {
        return;
    }
    
    char current_char = input[index];
    ctx->current_state = transition(ctx, current_char);
    
    if (ctx->current_state == ERROR) {
        printf("Invalid input at position %zu\n", index);
        return;
    }
    
    process_input_recursive(ctx, input, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.result = 0;
    
    char input[512];
    printf("Enter sequence (format: B<numbers and spaces>EPX): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    process_input_recursive(&ctx, input, 0);
    
    if (ctx.current_state != FINISHED) {
        printf("Processing failed - invalid sequence\n");
        return 1;
    }
    
    return 0;
}