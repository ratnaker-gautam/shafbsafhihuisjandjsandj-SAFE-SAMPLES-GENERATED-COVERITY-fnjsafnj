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
    size_t input_pos;
    int digit_count;
    int word_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->input_pos = 0;
    ctx->digit_count = 0;
    ctx->word_count = 0;
}

enum State handle_start(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return ERROR;
    }
    size_t len = strlen(ctx->input_buffer);
    if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
        ctx->input_buffer[len - 1] = '\0';
    }
    ctx->input_pos = 0;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->input_pos >= sizeof(ctx->input_buffer)) return FINISHED;
    char c = ctx->input_buffer[ctx->input_pos];
    if (c == '\0') return FINISHED;
    if (c >= '0' && c <= '9') return COUNT_DIGITS;
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return PROCESS_WORD;
    ctx->input_pos++;
    return READ_CHAR;
}

enum State handle_count_digits(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    char c = ctx->input_buffer[ctx->input_pos];
    while (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->input_pos++;
        if (ctx->input_pos >= sizeof(ctx->input_buffer)) break;
        c = ctx->input_buffer[ctx->input_pos];
    }
    return READ_CHAR;
}

enum State handle_process_word(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    char c = ctx->input_buffer[ctx->input_pos];
    while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        ctx->input_pos++;
        if (ctx->input_pos >= sizeof(ctx->input_buffer)) break;
        c = ctx->input_buffer[ctx->input_pos];
    }
    ctx->word_count++;
    return READ_CHAR;
}

enum State handle_finished(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Words: %d, Digits: %d\n", ctx->word_count, ctx->digit_count);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Error occurred during processing.\n");
    return ERROR;
}

void state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
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
            ctx->current_state = handle_finished(ctx);
            return;
        case ERROR:
            ctx->current_state = handle_error(ctx);
            return;
    }
    
    state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    state_machine(&ctx);
    return 0;
}