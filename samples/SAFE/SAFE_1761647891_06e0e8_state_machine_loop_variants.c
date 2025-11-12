//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN];
    size_t buf_pos;
} parser_context_t;

void init_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (c == '\0' || c == '\n') {
        return STATE_END;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (c == '\0' || c == '\n' || c == ' ' || c == '\t') {
        if (ctx->buf_pos > 0) {
            ctx->number_count++;
        }
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0' || c == '\n') {
            return STATE_END;
        }
        return STATE_START;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (c == '\0' || c == '\n' || c == ' ' || c == '\t') {
        if (ctx->buf_pos > 0) {
            ctx->word_count++;
        }
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0' || c == '\n') {
            return STATE_END;
        }
        return STATE_START;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_WORD;
    } else {
        return STATE_ERROR;
    }
}

int parse_input(const char *input, parser_context_t *ctx) {
    size_t i = 0;
    char c;
    
    while (1) {
        c = input[i++];
        
        if (!is_valid_input_char(c)) {
            ctx->current_state = STATE_ERROR;
            return 0;
        }
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(c, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(c, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(c, ctx);
                break;
            case STATE_ERROR:
            case STATE_END:
                break;
        }
        
        if (ctx->current_state == STATE_ERROR || ctx->current_state == STATE_END) {
            break;
        }
        
        if (i >= MAX_INPUT_LEN) {
            ctx->current_state = STATE_ERROR;
            break;
        }
    }
    
    return ctx->current_state == STATE_END;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t ctx;
    int result;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_parser(&ctx);
    result = parse_input(input, &ctx);
    
    if (!result) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}