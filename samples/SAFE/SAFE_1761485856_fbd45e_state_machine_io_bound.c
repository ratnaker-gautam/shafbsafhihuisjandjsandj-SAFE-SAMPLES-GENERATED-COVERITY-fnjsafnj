//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN];
    int buf_pos;
} parser_context_t;

void init_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (c == '\n' || c == '\0') {
        return STATE_DONE;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(parser_context_t *ctx, char c) {
    if (isdigit(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (isspace(c) || c == '\n' || c == '\0') {
        ctx->number_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\n' || c == '\0') {
            return STATE_DONE;
        }
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (isalpha(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (isspace(c) || c == '\n' || c == '\0') {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\n' || c == '\0') {
            return STATE_DONE;
        }
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t process_char(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, c);
        case STATE_READING_NUMBER:
            return handle_reading_number(ctx, c);
        case STATE_READING_WORD:
            return handle_reading_word(ctx, c);
        case STATE_ERROR:
            return STATE_ERROR;
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    parser_context_t ctx;
    char input[MAX_INPUT_LEN * 2];
    char *line = NULL;
    size_t line_size = 0;
    
    printf("Enter text (numbers and words separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    init_parser(&ctx);
    
    for (size_t i = 0; i < sizeof(input) && input[i] != '\0'; i++) {
        ctx.current_state = process_char(&ctx, input[i]);
        if (ctx.current_state == STATE_ERROR) {
            printf("Error: Invalid input format\n");
            return 1;
        }
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING_NUMBER) {
        ctx.number_count++;
    } else if (ctx.current_state == STATE_READING_WORD) {
        ctx.word_count++;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}