//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    PROCESS_DIGIT,
    PROCESS_ALPHA,
    END,
    ERROR
};

struct Context {
    const char *input;
    size_t position;
    int digit_sum;
    int alpha_count;
};

enum State handle_start(struct Context *ctx);
enum State handle_read_char(struct Context *ctx);
enum State handle_process_digit(struct Context *ctx, char c);
enum State handle_process_alpha(struct Context *ctx, char c);
enum State handle_end(struct Context *ctx);
enum State handle_error(struct Context *ctx);

enum State process_state(enum State current, struct Context *ctx) {
    switch (current) {
        case START:
            return handle_start(ctx);
        case READ_CHAR:
            return handle_read_char(ctx);
        case PROCESS_DIGIT:
            if (ctx->position >= strlen(ctx->input)) return ERROR;
            return handle_process_digit(ctx, ctx->input[ctx->position]);
        case PROCESS_ALPHA:
            if (ctx->position >= strlen(ctx->input)) return ERROR;
            return handle_process_alpha(ctx, ctx->input[ctx->position]);
        case END:
            return handle_end(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx) {
    if (ctx->input == NULL) return ERROR;
    ctx->position = 0;
    ctx->digit_sum = 0;
    ctx->alpha_count = 0;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) return END;
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') return PROCESS_DIGIT;
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return PROCESS_ALPHA;
    return ERROR;
}

enum State handle_process_digit(struct Context *ctx, char c) {
    if (c < '0' || c > '9') return ERROR;
    ctx->digit_sum += (c - '0');
    ctx->position++;
    return READ_CHAR;
}

enum State handle_process_alpha(struct Context *ctx, char c) {
    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) return ERROR;
    ctx->alpha_count++;
    ctx->position++;
    return READ_CHAR;
}

enum State handle_end(struct Context *ctx) {
    printf("Processing complete.\n");
    printf("Digit sum: %d\n", ctx->digit_sum);
    printf("Alpha count: %d\n", ctx->alpha_count);
    return END;
}

enum State handle_error(struct Context *ctx) {
    printf("Error: Invalid input at position %zu\n", ctx->position);
    return ERROR;
}

void run_state_machine(enum State state, struct Context *ctx) {
    enum State next_state = process_state(state, ctx);
    if (next_state != END && next_state != ERROR) {
        run_state_machine(next_state, ctx);
    }
}

int main(void) {
    char input_buffer[256];
    
    printf("Enter a string (max 255 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.input = input_buffer;
    
    run_state_machine(START, &ctx);
    
    return 0;
}