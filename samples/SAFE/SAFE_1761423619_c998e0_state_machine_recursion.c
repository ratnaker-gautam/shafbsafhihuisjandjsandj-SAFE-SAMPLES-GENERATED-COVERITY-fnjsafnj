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
    int digit_count;
    int alpha_count;
    char current_char;
};

enum State handle_start(struct Context *ctx) {
    ctx->digit_count = 0;
    ctx->alpha_count = 0;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    int c = getchar();
    if (c == EOF) {
        return END;
    }
    if (c == '\n') {
        return END;
    }
    ctx->current_char = (char)c;
    return PROCESS_DIGIT;
}

enum State handle_process_digit(struct Context *ctx) {
    if (ctx->current_char >= '0' && ctx->current_char <= '9') {
        ctx->digit_count++;
        return READ_CHAR;
    }
    return PROCESS_ALPHA;
}

enum State handle_process_alpha(struct Context *ctx) {
    if ((ctx->current_char >= 'A' && ctx->current_char <= 'Z') || 
        (ctx->current_char >= 'a' && ctx->current_char <= 'z')) {
        ctx->alpha_count++;
        return READ_CHAR;
    }
    return ERROR;
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

void run_state_machine(struct Context *ctx) {
    enum State next_state = ctx->current_state;
    
    if (ctx->current_state == END || ctx->current_state == ERROR) {
        return;
    }
    
    StateHandler handler = get_handler(ctx->current_state);
    if (handler == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    next_state = handler(ctx);
    ctx->current_state = next_state;
    
    run_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Invalid input character encountered\n");
        return 1;
    }
    
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.alpha_count);
    
    return 0;
}