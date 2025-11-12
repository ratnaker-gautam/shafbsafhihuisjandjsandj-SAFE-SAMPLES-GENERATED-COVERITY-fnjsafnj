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
    STATE_READING_NUM,
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

static void reset_context(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

static void process_char(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = STATE_READING_NUM;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (c == ' ' || c == '\t') {
                break;
            } else if (c == '\n') {
                ctx->current_state = STATE_END;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUM:
            if (isdigit((unsigned char)c)) {
                if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->number_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\n') {
                    ctx->current_state = STATE_END;
                } else {
                    ctx->current_state = STATE_START;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\n') {
                    ctx->current_state = STATE_END;
                } else {
                    ctx->current_state = STATE_START;
                }
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
        case STATE_END:
            break;
    }
}

int main(void) {
    parser_context_t ctx;
    char input[MAX_INPUT_LEN + 2];
    int ch;
    size_t i;
    
    printf("Enter text (numbers and words separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    reset_context(&ctx);
    
    for (i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    i = 0;
    while (i < input_len && ctx.current_state != STATE_ERROR && ctx.current_state != STATE_END) {
        process_char(&ctx, input[i]);
        i++;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Parsing error occurred\n");
        return 1;
    }
    
    if (ctx.current_state == STATE_READING_NUM) {
        ctx.number_count++;
    } else if (ctx.current_state == STATE_READING_WORD) {
        ctx.word_count++;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}