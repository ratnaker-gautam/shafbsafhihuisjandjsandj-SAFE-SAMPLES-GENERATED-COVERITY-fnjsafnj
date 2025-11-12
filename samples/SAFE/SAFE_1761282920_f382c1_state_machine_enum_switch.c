//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_WORD,
    FINISH,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    unsigned int word_count;
    unsigned int char_count;
};

void state_machine_reset(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum state handle_start(struct context *ctx, int c) {
    if (ctx == NULL) return ERROR;
    if (c == EOF) return FINISH;
    if (isspace(c)) return START;
    if (isalpha(c)) {
        ctx->buffer[ctx->buffer_pos++] = (char)c;
        ctx->char_count++;
        return READ_CHAR;
    }
    return ERROR;
}

enum state handle_read_char(struct context *ctx, int c) {
    if (ctx == NULL) return ERROR;
    if (c == EOF) {
        if (ctx->buffer_pos > 0) return COUNT_WORD;
        return FINISH;
    }
    if (isalpha(c)) {
        if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) return ERROR;
        ctx->buffer[ctx->buffer_pos++] = (char)c;
        ctx->char_count++;
        return READ_CHAR;
    }
    if (isspace(c)) {
        return COUNT_WORD;
    }
    return ERROR;
}

enum state handle_count_word(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        printf("Word %u: '%.*s'\n", ctx->word_count, (int)ctx->buffer_pos, ctx->buffer);
    }
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return START;
}

enum state handle_finish(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Total words: %u\n", ctx->word_count);
    printf("Total characters: %u\n", ctx->char_count);
    return FINISH;
}

enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    fprintf(stderr, "Error: Invalid input character or buffer overflow\n");
    return ERROR;
}

void process_input(struct context *ctx) {
    if (ctx == NULL) return;
    int c;
    while (ctx->current_state != FINISH && ctx->current_state != ERROR) {
        c = getchar();
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx, c);
                break;
            case READ_CHAR:
                ctx->current_state = handle_read_char(ctx, c);
                break;
            case COUNT_WORD:
                ctx->current_state = handle_count_word(ctx);
                break;
            case FINISH:
                ctx->current_state = handle_finish(ctx);
                break;
            case ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
    if (ctx->current_state == READ_CHAR && c == EOF) {
        ctx->current_state = handle_count_word(ctx);
        ctx->current_state = handle_finish(ctx);
    }
}

int main(void) {
    struct context ctx;
    state_machine_reset(&ctx);
    process_input(&ctx);
    return (ctx.current_state == FINISH) ? EXIT_SUCCESS : EXIT_FAILURE;
}