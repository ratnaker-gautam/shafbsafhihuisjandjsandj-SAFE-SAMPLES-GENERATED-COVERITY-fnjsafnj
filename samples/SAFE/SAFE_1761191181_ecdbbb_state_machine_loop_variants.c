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

static void reset_buffer(struct context *ctx) {
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buf_pos = 0;
}

static int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

static void process_buffer(struct context *ctx) {
    if (ctx->buf_pos == 0) return;
    
    int is_number = 1;
    for (size_t i = 0; i < ctx->buf_pos; i++) {
        if (!isdigit((unsigned char)ctx->buffer[i])) {
            is_number = 0;
            break;
        }
    }
    
    if (is_number) {
        ctx->number_count++;
    } else {
        ctx->word_count++;
    }
    reset_buffer(ctx);
}

static enum state handle_start(struct context *ctx, char c) {
    if (isspace((unsigned char)c)) {
        return SKIPPING_SPACE;
    } else if (isdigit((unsigned char)c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    }
    return ERROR;
}

static enum state handle_reading_number(struct context *ctx, char c) {
    if (isspace((unsigned char)c)) {
        process_buffer(ctx);
        return SKIPPING_SPACE;
    } else if (isdigit((unsigned char)c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        return ERROR;
    }
    return ERROR;
}

static enum state handle_reading_word(struct context *ctx, char c) {
    if (isspace((unsigned char)c)) {
        process_buffer(ctx);
        return SKIPPING_SPACE;
    } else if (isalnum((unsigned char)c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    }
    return ERROR;
}

static enum state handle_skipping_space(struct context *ctx, char c) {
    if (isspace((unsigned char)c)) {
        return SKIPPING_SPACE;
    } else if (isdigit((unsigned char)c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    }
    return ERROR;
}

int main(void) {
    struct context ctx;
    ctx.current = START;
    ctx.number_count = 0;
    ctx.word_count = 0;
    reset_buffer(&ctx);
    
    printf("Enter text (max 1023 characters): ");
    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    size_t pos = 0;
    while (pos < len && ctx.current != ERROR && ctx.current != FINISHED) {
        char c = input[pos];
        if (!is_valid_char(c)) {
            ctx.current = ERROR;
            break;
        }
        
        enum state next_state;
        switch (ctx.current) {
            case START:
                next_state = handle_start(&ctx, c);
                break;
            case READING_NUMBER:
                next_state = handle_reading_number(&ctx, c);
                break;
            case READING_WORD:
                next_state = handle_reading_word(&ctx, c);
                break;
            case SKIPPING_SPACE:
                next_state = handle_skipping_space(&ctx, c);
                break;
            default:
                next_state = ERROR;
                break;
        }
        
        ctx.current = next_state;
        pos++;
    }
    
    if (ctx.current == READING_NUMBER || ctx.current == READING_WORD) {
        process_buffer(&ctx);
        ctx.current = FINISHED;
    }
    
    if (ctx.current == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}