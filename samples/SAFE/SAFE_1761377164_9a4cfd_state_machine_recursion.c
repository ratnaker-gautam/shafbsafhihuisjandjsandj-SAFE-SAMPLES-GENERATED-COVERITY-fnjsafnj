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

enum State (*state_handlers[])(struct Context *) = {
    handle_start,
    handle_read_char,
    handle_process_digit,
    handle_process_alpha,
    handle_end,
    handle_error
};

void process_state_machine(struct Context *ctx) {
    while (ctx->current_state != END && ctx->current_state != ERROR) {
        if (ctx->current_state < START || ctx->current_state > ERROR) {
            ctx->current_state = ERROR;
            break;
        }
        enum State next_state = state_handlers[ctx->current_state](ctx);
        ctx->current_state = next_state;
    }
}

void initialize_context(struct Context *ctx, const char *input) {
    ctx->current_state = START;
    ctx->digit_count = 0;
    ctx->alpha_count = 0;
    ctx->position = 0;
    
    if (input == NULL) {
        ctx->input_len = 0;
        ctx->input_buffer[0] = '\0';
        return;
    }
    
    size_t len = strlen(input);
    if (len >= sizeof(ctx->input_buffer)) {
        len = sizeof(ctx->input_buffer) - 1;
    }
    memcpy(ctx->input_buffer, input, len);
    ctx->input_buffer[len] = '\0';
    ctx->input_len = len;
}

int main() {
    struct Context ctx;
    char user_input[256];
    
    printf("Enter a string to analyze (letters and digits only): ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    initialize_context(&ctx, user_input);
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Input contains invalid characters\n");
        return 1;
    }
    
    printf("Analysis results:\n");
    printf("Letters: %d\n", ctx.alpha_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}