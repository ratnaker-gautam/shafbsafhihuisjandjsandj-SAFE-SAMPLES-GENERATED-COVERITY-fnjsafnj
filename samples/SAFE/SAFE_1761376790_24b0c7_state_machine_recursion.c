//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    END
};

struct Context {
    enum State current_state;
    char input[256];
    int position;
    int digit_count;
    int word_count;
};

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
}

enum State handle_start(struct Context* ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        return END;
    }
    char current = ctx->input[ctx->position];
    if (is_whitespace(current)) {
        ctx->position++;
        return START;
    }
    return READ_CHAR;
}

enum State handle_read_char(struct Context* ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        return END;
    }
    char current = ctx->input[ctx->position];
    if (is_whitespace(current)) {
        ctx->word_count++;
        return START;
    }
    if (is_digit(current)) {
        ctx->digit_count++;
    }
    ctx->position++;
    return READ_CHAR;
}

void process_state_machine(struct Context* ctx) {
    if (ctx == NULL) {
        return;
    }
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case COUNT_DIGITS:
        case PROCESS_WORD:
        case END:
            return;
    }
    
    if (ctx->current_state != END) {
        process_state_machine(ctx);
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text: ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.word_count > 0) {
        printf("Words processed: %d\n", ctx.word_count);
        printf("Digits found: %d\n", ctx.digit_count);
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}