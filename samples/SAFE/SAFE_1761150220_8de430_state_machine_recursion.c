//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    char input[256];
    size_t position;
    int digit_count;
    int word_count;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        return FINISHED;
    }
    char c = ctx->input[ctx->position];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->position++;
        return START;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return PROCESS_WORD;
    } else if (c >= '0' && c <= '9') {
        ctx->digit_count = 0;
        return COUNT_DIGITS;
    } else {
        return ERROR;
    }
}

enum State handle_process_word(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    while (ctx->position < strlen(ctx->input) && 
           ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
        ctx->position++;
        if (ctx->position < strlen(ctx->input)) {
            c = ctx->input[ctx->position];
        }
    }
    ctx->word_count++;
    return START;
}

enum State handle_count_digits(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    while (ctx->position < strlen(ctx->input) && (c >= '0' && c <= '9')) {
        ctx->digit_count++;
        ctx->position++;
        if (ctx->position < strlen(ctx->input)) {
            c = ctx->input[ctx->position];
        }
    }
    return START;
}

enum State handle_error(struct Context *ctx) {
    return ERROR;
}

enum State handle_finished(struct Context *ctx) {
    return FINISHED;
}

enum State state_machine_step(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case PROCESS_WORD:
            return handle_process_word(ctx);
        case COUNT_DIGITS:
            return handle_count_digits(ctx);
        case ERROR:
            return handle_error(ctx);
        case FINISHED:
            return handle_finished(ctx);
        default:
            return ERROR;
    }
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    ctx->current_state = state_machine_step(ctx);
    
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    
    run_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    printf("Enter text (max 255 characters): ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character encountered\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Total digits: %d\n", ctx.digit_count);
    
    return 0;
}