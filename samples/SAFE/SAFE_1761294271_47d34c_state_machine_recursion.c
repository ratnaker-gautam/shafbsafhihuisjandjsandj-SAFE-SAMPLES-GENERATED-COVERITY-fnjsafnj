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
    char buffer[256];
    size_t buffer_pos;
};

enum State handle_start(struct Context *ctx, char c) {
    if (c == '\0') return END;
    if (c == ' ' || c == '\t' || c == '\n') return START;
    if (c >= '0' && c <= '9') return PROCESS_DIGIT;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return PROCESS_ALPHA;
    return ERROR;
}

enum State handle_read_char(struct Context *ctx, char c) {
    if (c == '\0') return END;
    if (c == ' ' || c == '\t' || c == '\n') return START;
    if (c >= '0' && c <= '9') return PROCESS_DIGIT;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return PROCESS_ALPHA;
    return ERROR;
}

enum State handle_process_digit(struct Context *ctx, char c) {
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = c;
    }
    ctx->digit_count++;
    return READ_CHAR;
}

enum State handle_process_alpha(struct Context *ctx, char c) {
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = c;
    }
    ctx->alpha_count++;
    return READ_CHAR;
}

void process_state_machine(struct Context *ctx, const char *input, size_t pos) {
    if (ctx->current_state == END || ctx->current_state == ERROR) {
        return;
    }
    
    if (pos >= strlen(input)) {
        ctx->current_state = END;
        return;
    }
    
    char current_char = input[pos];
    enum State next_state;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx, current_char);
            break;
        case READ_CHAR:
            next_state = handle_read_char(ctx, current_char);
            break;
        case PROCESS_DIGIT:
            next_state = handle_process_digit(ctx, current_char);
            break;
        case PROCESS_ALPHA:
            next_state = handle_process_alpha(ctx, current_char);
            break;
        default:
            next_state = ERROR;
            break;
    }
    
    ctx->current_state = next_state;
    process_state_machine(ctx, input, pos + 1);
}

int main(void) {
    char input[512];
    struct Context ctx;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = START;
    ctx.buffer[0] = '\0';
    
    process_state_machine(&ctx, input, 0);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (ctx.buffer_pos < sizeof(ctx.buffer)) {
        ctx.buffer[ctx.buffer_pos] = '\0';
    }
    
    printf("Analysis complete:\n");
    printf("Digits found: %d\n", ctx.digit_count);
    printf("Letters found: %d\n", ctx.alpha_count);
    printf("Processed characters: %s\n", ctx.buffer);
    
    return 0;
}