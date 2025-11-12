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
    char input_buffer[256];
    size_t input_len;
    size_t position;
    int digit_count;
    int word_count;
};

enum State handle_start(struct Context* ctx) {
    if (ctx->input_len == 0) {
        return FINISHED;
    }
    ctx->position = 0;
    ctx->digit_count = 0;
    ctx->word_count = 0;
    return READ_CHAR;
}

enum State handle_read_char(struct Context* ctx) {
    if (ctx->position >= ctx->input_len) {
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->position];
    if (c >= '0' && c <= '9') {
        return COUNT_DIGITS;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return PROCESS_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->position++;
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

enum State handle_count_digits(struct Context* ctx) {
    if (ctx->position >= ctx->input_len) {
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->position++;
        return COUNT_DIGITS;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->position++;
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

enum State handle_process_word(struct Context* ctx) {
    if (ctx->position >= ctx->input_len) {
        ctx->word_count++;
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->position];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->position++;
        return PROCESS_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->word_count++;
        ctx->position++;
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

void process_state_machine(struct Context* ctx) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
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
    }
    
    process_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("Enter text to analyze (max 255 characters): ");
    
    if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    ctx.input_len = strlen(ctx.input_buffer);
    if (ctx.input_len > 0 && ctx.input_buffer[ctx.input_len - 1] == '\n') {
        ctx.input_buffer[ctx.input_len - 1] = '\0';
        ctx.input_len--;
    }
    
    if (ctx.input_len >= sizeof(ctx.input_buffer)) {
        printf("Input too long\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character encountered\n");
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Words found: %d\n", ctx.word_count);
    printf("Total digits: %d\n", ctx.digit_count);
    
    return 0;
}