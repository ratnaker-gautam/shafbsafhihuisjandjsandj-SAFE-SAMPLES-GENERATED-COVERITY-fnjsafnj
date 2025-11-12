//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int word_count;
    int number_count;
    char buffer[MAX_INPUT_LEN];
    char *buffer_ptr;
} parser_context_t;

void init_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->buffer_ptr = ctx->buffer;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (isalpha(c)) {
        *(ctx->buffer_ptr) = c;
        ctx->buffer_ptr++;
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        *(ctx->buffer_ptr) = c;
        ctx->buffer_ptr++;
        return STATE_READING_NUMBER;
    } else if (c == '\0' || c == '\n') {
        return STATE_END;
    }
    return STATE_ERROR;
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (isalpha(c)) {
        if ((ctx->buffer_ptr - ctx->buffer) < (MAX_INPUT_LEN - 1)) {
            *(ctx->buffer_ptr) = c;
            ctx->buffer_ptr++;
        }
        return STATE_READING_WORD;
    } else if (isspace(c) || c == '\0' || c == '\n') {
        ctx->word_count++;
        ctx->buffer_ptr = ctx->buffer;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0' || c == '\n') {
            return STATE_END;
        }
        return STATE_START;
    }
    return STATE_ERROR;
}

parser_state_t handle_reading_number(parser_context_t *ctx, char c) {
    if (isdigit(c)) {
        if ((ctx->buffer_ptr - ctx->buffer) < (MAX_INPUT_LEN - 1)) {
            *(ctx->buffer_ptr) = c;
            ctx->buffer_ptr++;
        }
        return STATE_READING_NUMBER;
    } else if (isspace(c) || c == '\0' || c == '\n') {
        ctx->number_count++;
        ctx->buffer_ptr = ctx->buffer;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0' || c == '\n') {
            return STATE_END;
        }
        return STATE_START;
    }
    return STATE_ERROR;
}

parser_state_t process_char(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, c);
        case STATE_READING_WORD:
            return handle_reading_word(ctx, c);
        case STATE_READING_NUMBER:
            return handle_reading_number(ctx, c);
        case STATE_END:
        case STATE_ERROR:
            return ctx->current_state;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    parser_context_t ctx;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_parser(&ctx);
    
    char *ptr = input;
    while (*ptr != '\0' && ctx.current_state != STATE_END && ctx.current_state != STATE_ERROR) {
        ctx.current_state = process_char(&ctx, *ptr);
        ptr++;
    }
    
    if (ctx.current_state == STATE_READING_WORD) {
        ctx.word_count++;
    } else if (ctx.current_state == STATE_READING_NUMBER) {
        ctx.number_count++;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Parser error\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Numbers: %d\n", ctx.number_count);
    
    return 0;
}