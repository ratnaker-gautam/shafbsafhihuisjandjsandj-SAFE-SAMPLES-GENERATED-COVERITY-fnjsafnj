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

int process_state(struct Context *ctx);

int handle_start(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        ctx->current_state = END;
        return 1;
    }
    
    char c = ctx->input[ctx->position];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->position++;
        return 1;
    }
    
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->current_state = READ_CHAR;
        ctx->word_count++;
        return 1;
    }
    
    if (c >= '0' && c <= '9') {
        ctx->current_state = COUNT_DIGITS;
        ctx->digit_count = 0;
        return 1;
    }
    
    ctx->position++;
    return 1;
}

int handle_read_char(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        ctx->current_state = END;
        return 1;
    }
    
    char c = ctx->input[ctx->position];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = START;
        ctx->position++;
        return 1;
    }
    
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->position++;
        return 1;
    }
    
    if (c >= '0' && c <= '9') {
        ctx->current_state = COUNT_DIGITS;
        ctx->digit_count = 0;
        return 1;
    }
    
    ctx->position++;
    ctx->current_state = START;
    return 1;
}

int handle_count_digits(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        ctx->current_state = END;
        return 1;
    }
    
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->position++;
        return 1;
    }
    
    ctx->current_state = PROCESS_WORD;
    return 1;
}

int handle_process_word(struct Context *ctx) {
    printf("Found number with %d digits\n", ctx->digit_count);
    ctx->current_state = START;
    return 1;
}

int handle_end(struct Context *ctx) {
    return 0;
}

int process_state(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case READ_CHAR:
            return handle_read_char(ctx);
        case COUNT_DIGITS:
            return handle_count_digits(ctx);
        case PROCESS_WORD:
            return handle_process_word(ctx);
        case END:
            return handle_end(ctx);
        default:
            return 0;
    }
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    while (process_state(ctx)) {
        if (ctx->current_state == END) break;
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
    if (len > 0 && ctx.input[len-1] == '\n') {
        ctx.input[len-1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    run_state_machine(&ctx);
    
    printf("Total words: %d\n", ctx.word_count);
    
    return 0;
}