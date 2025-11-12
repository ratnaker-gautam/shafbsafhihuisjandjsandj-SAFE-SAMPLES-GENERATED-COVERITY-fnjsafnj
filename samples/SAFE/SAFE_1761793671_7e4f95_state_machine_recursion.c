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
    int position;
    int digit_count;
    int word_count;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
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

enum State handle_count_digits(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        return FINISHED;
    }
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->position++;
        return COUNT_DIGITS;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->position++;
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_process_word(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        ctx->word_count++;
        return FINISHED;
    }
    char c = ctx->input[ctx->position];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->position++;
        return PROCESS_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->word_count++;
        ctx->position++;
        return START;
    } else {
        return ERROR;
    }
}

void process_state_machine(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case COUNT_DIGITS:
            ctx->current_state = handle_count_digits(ctx);
            break;
        case PROCESS_WORD:
            ctx->current_state = handle_process_word(ctx);
            break;
        case FINISHED:
            return;
        case ERROR:
            return;
        default:
            ctx->current_state = ERROR;
            return;
    }
    process_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text: ");
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
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digit sequences: %d\n", ctx.digit_count);
    
    return 0;
}