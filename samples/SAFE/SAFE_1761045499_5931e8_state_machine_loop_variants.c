//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    char buffer[MAX_INPUT_LEN + 1];
    size_t buf_pos;
} parser_context_t;

static void init_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || c == '\0';
}

static void process_char(parser_context_t *ctx, char c) {
    if (!is_valid_input_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (c == '\0') {
                ctx->current_state = STATE_END;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buf_pos < MAX_INPUT_LEN) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace((unsigned char)c) || c == '\0') {
                ctx->number_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\0') {
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
                if (ctx->buf_pos < MAX_INPUT_LEN) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace((unsigned char)c) || c == '\0') {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (c == '\0') {
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

static int read_input_safely(char *buf, size_t buf_size) {
    if (buf_size == 0) return 0;
    
    size_t i = 0;
    int c;
    while (i < buf_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') break;
        buf[i++] = (char)c;
    }
    buf[i] = '\0';
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (!read_input_safely(input, sizeof(input))) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    init_parser(&parser);

    size_t i = 0;
    while (input[i] != '\0' && parser.current_state != STATE_ERROR && parser.current_state != STATE_END) {
        process_char(&parser, input[i]);
        i++;
    }

    if (parser.current_state != STATE_END && parser.current_state != STATE_ERROR) {
        process_char(&parser, '\0');
    }

    if (parser.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);

    return 0;
}