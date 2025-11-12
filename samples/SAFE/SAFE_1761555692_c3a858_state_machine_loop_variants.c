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
    return isalnum(c) || isspace(c) || c == '.' || c == ',';
}

parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    }
    return STATE_ERROR;
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    }
    return STATE_ERROR;
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (isalpha(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    }
    return STATE_ERROR;
}

int process_input(const char *input, parser_context_t *ctx) {
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
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
                return 0;
            case STATE_END:
                return 1;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return 0;
        }
    }
    
    if (ctx->current_state == STATE_READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == STATE_READING_WORD) {
        ctx->word_count++;
    }
    
    ctx->current_state = STATE_END;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t parser;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_parser(&parser);
    
    if (!process_input(input, &parser)) {
        fprintf(stderr, "Error parsing input\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}