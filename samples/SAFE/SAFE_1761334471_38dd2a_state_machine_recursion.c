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
    return handle_start(ctx);
}

enum State handle_process_digit(struct Context *ctx) {
    ctx->digit_count++;
    return handle_read_char(ctx);
}

enum State handle_process_alpha(struct Context *ctx) {
    ctx->alpha_count++;
    return handle_read_char(ctx);
}

enum State handle_error(struct Context *ctx) {
    return END;
}

enum State handle_end(struct Context *ctx) {
    return END;
}

typedef enum State (*StateHandler)(struct Context *);

StateHandler get_handler(enum State state) {
    switch (state) {
        case START: return handle_start;
        case READ_CHAR: return handle_read_char;
        case PROCESS_DIGIT: return handle_process_digit;
        case PROCESS_ALPHA: return handle_process_alpha;
        case ERROR: return handle_error;
        case END: return handle_end;
        default: return handle_error;
    }
}

void process_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = START;
    
    while (ctx->current_state != END) {
        StateHandler handler = get_handler(ctx->current_state);
        if (handler != NULL) {
            ctx->current_state = handler(ctx);
        } else {
            ctx->current_state = ERROR;
        }
    }
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (ctx.input_len >= sizeof(ctx.input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.alpha_count);
    
    return 0;
}