//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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

int is_valid_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (c == '\0') {
        return STATE_END;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (isdigit(c)) {
        *(ctx->buffer + ctx->buf_pos) = c;
        ctx->buf_pos++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        *(ctx->buffer + ctx->buf_pos) = c;
        ctx->buf_pos++;
        return STATE_READING_WORD;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(parser_context_t *ctx, char c) {
    if (c == '\0') {
        ctx->number_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_END;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            *(ctx->buffer + ctx->buf_pos) = c;
            ctx->buf_pos++;
            return STATE_READING_NUMBER;
        } else {
            return STATE_ERROR;
        }
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (c == '\0') {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_END;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            *(ctx->buffer + ctx->buf_pos) = c;
            ctx->buf_pos++;
            return STATE_READING_WORD;
        } else {
            return STATE_ERROR;
        }
    } else {
        return STATE_ERROR;
    }
}

int process_input(parser_context_t *ctx, const char *input) {
    if (input == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = *(input + i);
        
        if (!is_valid_char(current_char)) {
            return -1;
        }
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(ctx, current_char);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx, current_char);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(ctx, current_char);
                break;
            case STATE_ERROR:
                return -1;
            case STATE_END:
                return 0;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    parser_context_t parser;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(input + len - 1) == '\n') {
        *(input + len - 1) = '\0';
    }
    
    init_parser(&parser);
    
    if (process_input(&parser, input) != 0) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}