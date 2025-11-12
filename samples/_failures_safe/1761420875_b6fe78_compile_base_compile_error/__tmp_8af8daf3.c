//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STATE_COUNT 8
#define MAX_INPUT_LEN 1000

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_READING_WHITESPACE,
    STATE_FOUND_INTEGER,
    STATE_FOUND_FLOAT,
    STATE_FOUND_WORD,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int integer_value;
    double float_value;
    char word_buffer[64];
    int word_len;
    int has_decimal;
    int has_digit;
} parser_context_t;

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->integer_value = 0;
    ctx->float_value = 0.0;
    memset(ctx->word_buffer, 0, sizeof(ctx->word_buffer));
    ctx->word_len = 0;
    ctx->has_decimal = 0;
    ctx->has_digit = 0;
}

parser_state_t process_character(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->integer_value = c - '0';
                ctx->float_value = c - '0';
                ctx->has_digit = 1;
                return STATE_READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->word_buffer[0] = c;
                ctx->word_len = 1;
                return STATE_READING_WORD;
            } else if (isspace(c)) {
                return STATE_READING_WHITESPACE;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                int digit = c - '0';
                ctx->integer_value = ctx->integer_value * 10 + digit;
                ctx->float_value = ctx->float_value * 10.0 + digit;
                ctx->has_digit = 1;
                return STATE_READING_NUMBER;
            } else if (c == '.' && !ctx->has_decimal) {
                ctx->has_decimal = 1;
                return STATE_READING_NUMBER;
            } else if (isspace(c)) {
                if (ctx->has_digit) {
                    return ctx->has_decimal ? STATE_FOUND_FLOAT : STATE_FOUND_INTEGER;
                } else {
                    return STATE_ERROR;
                }
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (isalnum(c)) {
                if (ctx->word_len < 63) {
                    ctx->word_buffer[ctx->word_len++] = c;
                }
                return STATE_READING_WORD;
            } else if (isspace(c)) {
                if (ctx->word_len > 0) {
                    ctx->word_buffer[ctx->word_len] = '\0';
                    return STATE_FOUND_WORD;
                } else {
                    return STATE_ERROR;
                }
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WHITESPACE:
            if (isspace(c)) {
                return STATE_READING_WHITESPACE;
            } else if (isdigit(c)) {
                ctx->integer_value = c - '0';
                ctx->float_value = c - '0';
                ctx->has_digit = 1;
                ctx->has_decimal = 0;
                return STATE_READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->word_buffer[0] = c;
                ctx->word_len = 1;
                return STATE_READING_WORD;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_FOUND_INTEGER:
        case STATE_FOUND_FLOAT:
        case STATE_FOUND_WORD:
            if (isspace(c)) {
                return STATE_READING_WHITESPACE;
            } else if (isdigit(c)) {
                ctx->integer_value = c - '0';
                ctx->float_value = c - '0';
                ctx->has_digit = 1;
                ctx->has_decimal = 0;
                return STATE_READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->word_buffer[0] = c;
                ctx->word_len = 1;
                return STATE_READING_WORD;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_ERROR:
            if (isspace(c)) {
                return STATE_READING_WHITESPACE;
            }
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

void process_input(const char *input) {
    parser_context_t ctx;
    initialize_parser(&ctx);
    
    int input_len = strlen(input);
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    int token_count = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = (i < input_len) ? input[i] : ' ';
        
        parser_state_t old_state = ctx.current_state;
        ctx.current_state = process_character(&ctx, c);
        
        if (old_state != ctx.current_state) {
            switch (old_state) {
                case STATE_FOUND_INTEGER:
                    printf("