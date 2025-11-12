//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = START;
    
    while (ctx->current_state != END) {
        StateHandler handler = get_handler(ctx->current_state);
        if (handler == NULL) {
            break;
        }
        ctx->current_state = handler(ctx);
    }
}

void initialize_context(struct Context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    
    ctx->current_state = START;
    ctx->position = 0;
    ctx->digit_count = 0;
    ctx->alpha_count = 0;
    ctx->input_len = 0;
    
    size_t len = strlen(input);
    if (len >= sizeof(ctx->input_buffer)) {
        len = sizeof(ctx->input_buffer) - 1;
    }
    
    memcpy(ctx->input_buffer, input, len);
    ctx->input_buffer[len] = '\0';
    ctx->input_len = len;
}

void process_input_recursive(struct Context *ctx, int depth) {
    if (ctx == NULL || depth > 1000) {
        return;
    }
    
    if (ctx->current_state == END) {
        return;
    }
    
    StateHandler handler = get_handler(ctx->current_state);
    if (handler != NULL) {
        ctx->current_state = handler(ctx);
        process_input_recursive(ctx, depth + 1);
    }
}

int main() {
    char user_input[256];
    
    printf("Enter a string (letters and digits only): ");
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
    
    for (size_t i = 0; i < len; i++) {
        char c = user_input[i];
        if (!((c >= '0' && c <= '9') || 
              (c >= 'A' && c <= 'Z') || 
              (c >= 'a' && c <= 'z'))) {
            printf("Invalid character '%c' at position %zu\n", c, i);
            return 1;
        }
    }
    
    struct Context ctx;
    initialize_context(&ctx, user_input);
    
    process_input_recursive(&ctx, 0);
    
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.alpha_count);
    
    return 0;
}