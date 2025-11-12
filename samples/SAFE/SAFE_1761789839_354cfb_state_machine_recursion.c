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
    enum State current_state;
    char input_buffer[256];
    size_t input_len;
    size_t position;
    int digit_count;
    int alpha_count;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= ctx->input_len) {
        return END;
    }
    char c = ctx->input_buffer[ctx->position];
    if (c >= '0' && c <= '9') {
        return PROCESS_DIGIT;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_ALPHA;
    } else {
        return ERROR;
    }
}

enum State handle_read_char(struct Context *ctx) {
    ctx->position++;
    if (ctx->position >= ctx->input_len) {
        return END;
    }
    char c = ctx->input_buffer[ctx->position];
    if (c >= '0' && c <= '9') {
        return PROCESS_DIGIT;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_ALPHA;
    } else {
        return ERROR;
    }
}

enum State handle_process_digit(struct Context *ctx) {
    ctx->digit_count++;
    return handle_read_char(ctx);
}

enum State handle_process_alpha(struct Context *ctx) {
    ctx->alpha_count++;
    return handle_read_char(ctx);
}

void process_state_machine(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case PROCESS_DIGIT:
            ctx->current_state = handle_process_digit(ctx);
            break;
        case PROCESS_ALPHA:
            ctx->current_state = handle_process_alpha(ctx);
            break;
        case END:
            return;
        case ERROR:
            return;
        default:
            ctx->current_state = ERROR;
            return;
    }
    
    if (ctx->current_state != END && ctx->current_state != ERROR) {
        process_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.alpha_count = 0;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    ctx.input_len = strlen(ctx.input_buffer);
    if (ctx.input_len > 0 && ctx.input_buffer[ctx.input_len - 1] == '\n') {
        ctx.input_buffer[ctx.input_len - 1] = '\0';
        ctx.input_len--;
    }
    
    if (ctx.input_len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (ctx.input_len >= sizeof(ctx.input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character at position %zu\n", ctx.position);
        return 1;
    }
    
    printf("Processing complete:\n");
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.alpha_count);
    
    return 0;
}