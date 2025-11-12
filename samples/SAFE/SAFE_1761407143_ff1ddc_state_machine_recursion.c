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
        return COUNT_DIGITS;
    } else {
        return ERROR;
    }
}

enum State handle_read_char(struct Context *ctx) {
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
        return COUNT_DIGITS;
    } else {
        return ERROR;
    }
}

enum State handle_count_digits(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        ctx->digit_count++;
        return FINISHED;
    }
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->position++;
        return COUNT_DIGITS;
    } else {
        return handle_read_char(ctx);
    }
}

enum State handle_process_word(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        ctx->word_count++;
        return FINISHED;
    }
    char c = ctx->input[ctx->position];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->position++;
        return PROCESS_WORD;
    } else {
        ctx->word_count++;
        return handle_read_char(ctx);
    }
}

enum State handle_finished(struct Context *ctx) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    return ERROR;
}

enum State process_state(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case READ_CHAR:
            return handle_read_char(ctx);
        case COUNT_DIGITS:
            return handle_count_digits(ctx);
        case PROCESS_WORD:
            return handle_process_word(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) {
        return;
    }
    enum State next_state = process_state(ctx);
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        run_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text to analyze: ");
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
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}