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
    char input[256];
    size_t position;
    int digit_sum;
    int alpha_count;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        return END;
    }
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        return PROCESS_DIGIT;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_ALPHA;
    } else if (c == '\0') {
        return END;
    } else {
        return ERROR;
    }
}

enum State handle_process_digit(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    ctx->digit_sum += (c - '0');
    ctx->position++;
    return READ_CHAR;
}

enum State handle_process_alpha(struct Context *ctx) {
    ctx->alpha_count++;
    ctx->position++;
    return READ_CHAR;
}

enum State handle_error(struct Context *ctx) {
    return ERROR;
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
    if (ctx->current_state == END || ctx->current_state == ERROR) {
        return;
    }
    
    StateHandler handler = get_handler(ctx->current_state);
    if (handler == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = handler(ctx);
    ctx->current_state = next_state;
    
    process_state_machine(ctx);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_sum = 0;
    ctx.alpha_count = 0;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character encountered\n");
        return 1;
    }
    
    printf("Digit sum: %d\n", ctx.digit_sum);
    printf("Alpha count: %d\n", ctx.alpha_count);
    
    return 0;
}