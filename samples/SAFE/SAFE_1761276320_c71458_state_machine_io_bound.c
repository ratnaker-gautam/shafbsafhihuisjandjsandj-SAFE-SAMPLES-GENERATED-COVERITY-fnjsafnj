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
    if (c == '\n' || c == '\0') {
        ctx->number_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_DONE;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
            return STATE_READING_NUMBER;
        } else {
            return STATE_ERROR;
        }
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (c == '\n' || c == '\0') {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_DONE;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
            return STATE_READING_WORD;
        } else {
            return STATE_ERROR;
        }
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

void process_input(parser_context_t *ctx, const char *input) {
    size_t len = strnlen(input, MAX_INPUT_LEN * 2);
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(ctx, c);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx, c);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(ctx, c);
                break;
            case STATE_ERROR:
                return;
            case STATE_DONE:
                return;
        }
        
        if (ctx->current_state == STATE_ERROR || ctx->current_state == STATE_DONE) {
            break;
        }
    }
}

int main(void) {
    parser_context_t parser;
    char input[MAX_INPUT_LEN * 2];
    
    init_parser(&parser);
    
    printf("Enter text (numbers and words separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    process_input(&parser, input);
    
    if (parser.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}