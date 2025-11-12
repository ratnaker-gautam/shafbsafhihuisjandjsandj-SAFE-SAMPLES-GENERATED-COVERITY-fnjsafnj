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
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
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
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return PROCESS_ALPHA;
    } else {
        return ERROR;
    }
}

enum State handle_process_digit(struct Context *ctx) {
    ctx->digit_count++;
    return READ_CHAR;
}

enum State handle_process_alpha(struct Context *ctx) {
    ctx->alpha_count++;
    return READ_CHAR;
}

enum State handle_error(struct Context *ctx) {
    return ERROR;
}

enum State handle_end(struct Context *ctx) {
    return END;
}

typedef enum State (*StateHandler)(struct Context *);

StateHandler state_handlers[] = {
    handle_start,
    handle_read_char,
    handle_process_digit,
    handle_process_alpha,
    handle_end,
    handle_error
};

void process_state_machine(struct Context *ctx) {
    if (ctx->current_state == END || ctx->current_state == ERROR) {
        return;
    }
    
    if (ctx->current_state < START || ctx->current_state > ERROR) {
        ctx->current_state = ERROR;
        return;
    }
    
    StateHandler handler = state_handlers[ctx->current_state];
    if (handler == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = handler(ctx);
    if (next_state < START || next_state > ERROR) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = next_state;
    process_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.alpha_count = 0;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input_buffer);
    if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
        ctx.input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(ctx.input_buffer)) {
        printf("Input too long\n");
        return 1;
    }
    
    ctx.input_len = len;
    
    if (ctx.input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character encountered\n");
        return 1;
    }
    
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.alpha_count);
    
    return 0;
}