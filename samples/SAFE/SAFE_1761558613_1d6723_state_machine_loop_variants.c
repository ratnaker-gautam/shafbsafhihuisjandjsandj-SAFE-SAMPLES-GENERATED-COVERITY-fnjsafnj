//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct context {
    enum state current;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buf_pos;
};

void state_machine_init(struct context *ctx) {
    ctx->current = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum state handle_start(struct context *ctx, char c) {
    if (c == '\0' || c == '\n') {
        return FINISHED;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

enum state handle_reading_number(struct context *ctx, char c) {
    if (c == '\0' || c == '\n') {
        ctx->number_count++;
        ctx->buf_pos = 0;
        return FINISHED;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buf_pos = 0;
        return SKIPPING_SPACE;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else {
        return ERROR;
    }
}

enum state handle_reading_word(struct context *ctx, char c) {
    if (c == '\0' || c == '\n') {
        ctx->word_count++;
        ctx->buf_pos = 0;
        return FINISHED;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buf_pos = 0;
        return SKIPPING_SPACE;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

enum state handle_skipping_space(struct context *ctx, char c) {
    if (c == '\0' || c == '\n') {
        return FINISHED;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

void process_input(const char *input) {
    struct context ctx;
    state_machine_init(&ctx);
    
    size_t i = 0;
    char c;
    while (ctx.current != FINISHED && ctx.current != ERROR) {
        c = input[i++];
        switch (ctx.current) {
            case START:
                ctx.current = handle_start(&ctx, c);
                break;
            case READING_NUMBER:
                ctx.current = handle_reading_number(&ctx, c);
                break;
            case READING_WORD:
                ctx.current = handle_reading_word(&ctx, c);
                break;
            case SKIPPING_SPACE:
                ctx.current = handle_skipping_space(&ctx, c);
                break;
            default:
                ctx.current = ERROR;
                break;
        }
    }
    
    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Numbers: %d, Words: %d\n", ctx.number_count, ctx.word_count);
    }
}

int main(void) {
    char input[512];
    
    printf("Enter text (max 511 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}