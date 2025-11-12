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
    }
    return ERROR;
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
    }
    return ERROR;
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
    }
    return ERROR;
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
    }
    return ERROR;
}

void process_input(struct context *ctx, const char *input) {
    size_t i = 0;
    char c;
    
    while (ctx->current != FINISHED && ctx->current != ERROR) {
        c = input[i++];
        
        switch (ctx->current) {
            case START:
                ctx->current = handle_start(ctx, c);
                break;
            case READING_NUMBER:
                ctx->current = handle_reading_number(ctx, c);
                break;
            case READING_WORD:
                ctx->current = handle_reading_word(ctx, c);
                break;
            case SKIPPING_SPACE:
                ctx->current = handle_skipping_space(ctx, c);
                break;
            case FINISHED:
            case ERROR:
                break;
        }
        
        if (i >= 1024) {
            ctx->current = ERROR;
            break;
        }
    }
}

int main(void) {
    struct context ctx;
    char input[1025];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    state_machine_init(&ctx);
    process_input(&ctx, input);
    
    if (ctx.current == ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}