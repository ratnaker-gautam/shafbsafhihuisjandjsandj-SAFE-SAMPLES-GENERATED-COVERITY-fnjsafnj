//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STATE_COUNT 8
#define MAX_INPUT_LEN 1024

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_READING_WHITESPACE,
    STATE_FOUND_INTEGER,
    STATE_FOUND_FLOAT,
    STATE_FOUND_IDENTIFIER,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int integer_value;
    double float_value;
    int token_count;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->buffer_pos = 0;
    ctx->integer_value = 0;
    ctx->float_value = 0.0;
    ctx->token_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static parser_state_t transition_state(parser_state_t current, char input, parser_context_t *ctx) {
    switch (current) {
        case STATE_START:
            if (isdigit(input)) {
                ctx->buffer[ctx->buffer_pos++] = input;
                ctx->integer_value = input - '0';
                return STATE_READING_NUMBER;
            } else if (isalpha(input)) {
                ctx->buffer[ctx->buffer_pos++] = input;
                return STATE_READING_WORD;
            } else if (isspace(input)) {
                return STATE_READING_WHITESPACE;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (isdigit(input)) {
                if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
                    ctx->buffer[ctx->buffer_pos++] = input;
                    ctx->integer_value = ctx->integer_value * 10 + (input - '0');
                }
                return STATE_READING_NUMBER;
            } else if (input == '.') {
                if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
                    ctx->buffer[ctx->buffer_pos++] = input;
                    ctx->float_value = (double)ctx->integer_value;
                }
                return STATE_READING_NUMBER;
            } else if (isspace(input)) {
                if (strchr(ctx->buffer, '.')) {
                    return STATE_FOUND_FLOAT;
                } else {
                    return STATE_FOUND_INTEGER;
                }
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (isalnum(input)) {
                if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
                    ctx->buffer[ctx->buffer_pos++] = input;
                }
                return STATE_READING_WORD;
            } else if (isspace(input)) {
                return STATE_FOUND_IDENTIFIER;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WHITESPACE:
            if (isspace(input)) {
                return STATE_READING_WHITESPACE;
            } else if (isdigit(input)) {
                ctx->buffer[ctx->buffer_pos++] = input;
                ctx->integer_value = input - '0';
                return STATE_READING_NUMBER;
            } else if (isalpha(input)) {
                ctx->buffer[ctx->buffer_pos++] = input;
                return STATE_READING_WORD;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_FOUND_INTEGER:
        case STATE_FOUND_FLOAT:
        case STATE_FOUND_IDENTIFIER:
            if (isspace(input)) {
                return STATE_READING_WHITESPACE;
            } else if (isdigit(input)) {
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->buffer[ctx->buffer_pos++] = input;
                ctx->integer_value = input - '0';
                return STATE_READING_NUMBER;
            } else if (isalpha(input)) {
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->buffer[ctx->buffer_pos++] = input;
                return STATE_READING_WORD;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

static void process_token(parser_state_t state, parser_context_t *ctx) {
    ctx->token_count++;
    
    switch (state) {
        case STATE_FOUND_INTEGER:
            printf("Token %d: INTEGER '%s' (value: %d)\n", ctx->token_count, ctx->buffer, ctx->integer_value);
            break;
        case STATE_FOUND_FLOAT:
            ctx->float_value = atof(ctx->buffer);
            printf("Token %d: FLOAT '%s' (value: %f)\n", ctx->token_count, ctx->buffer, ctx->float_value);
            break;
        case STATE_FOUND_IDENTIFIER:
            printf("Token %d: IDENTIFIER '%s'\n", ctx->token_count, ctx->buffer);
            break;
        default:
            break;
    }
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->integer_value = 0;
    ctx->float_value