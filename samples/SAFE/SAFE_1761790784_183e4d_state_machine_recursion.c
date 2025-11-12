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

enum State handle_start(struct Context *ctx);
enum State handle_read_char(struct Context *ctx);
enum State handle_process_digit(struct Context *ctx);
enum State handle_process_alpha(struct Context *ctx);
enum State handle_error(struct Context *ctx);
enum State handle_end(struct Context *ctx);

enum State handle_start(struct Context *ctx) {
    if (ctx == NULL || ctx->position >= ctx->input_len) {
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
    if (ctx == NULL) return ERROR;
    ctx->position++;
    if (ctx->position >= ctx->input_len) {
        return END;
    }
    return handle_start(ctx);
}

enum State handle_process_digit(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    ctx->digit_count++;
    return handle_read_char(ctx);
}

enum State handle_process_alpha(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
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
    
    while (ctx->current_state != END && ctx->current_state != ERROR) {
        StateHandler handler = get_handler(ctx->current_state);
        if (handler == NULL) {
            ctx->current_state = ERROR;
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
    
    size_t input_length = strlen(input);
    if (input_length >= sizeof(ctx->input_buffer)) {
        input_length = sizeof(ctx->input_buffer) - 1;
    }
    
    memcpy(ctx->input_buffer, input, input_length);
    ctx->input_buffer[input_length] = '\0';
    ctx->input_len = input_length;
}

int main(void) {
    struct Context ctx;
    char user_input[256];
    
    printf("Enter a string to analyze (letters and digits only): ");
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!((user_input[i] >= '0' && user_input[i] <= '9') || 
              (user_input[i] >= 'a' && user_input[i] <= 'z') || 
              (user_input[i] >= 'A' && user_input[i] <= 'Z'))) {
            printf("Invalid character found: '%c'\n", user_input[i]);
            return 1;
        }
    }
    
    initialize_context(&ctx, user_input);
    process_state_machine(&ctx);
    
    printf("Analysis complete:\n");
    printf("Letters: %d\n", ctx.alpha_count);
    printf("Digits: %d\n", ctx.digit_count);
    printf("Total characters processed: %zu\n", ctx.input_len);
    
    return 0;
}