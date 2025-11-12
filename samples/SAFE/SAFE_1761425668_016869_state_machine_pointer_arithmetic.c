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

parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (c == '\0' || c == '\n') {
        return STATE_END;
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
    if (c == '\0' || c == '\n') {
        ctx->number_count++;
        ctx->buf_pos = 0;
        return STATE_END;
    } else if (isdigit(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buf_pos = 0;
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (c == '\0' || c == '\n') {
        ctx->word_count++;
        ctx->buf_pos = 0;
        return STATE_END;
    } else if (isalpha(c)) {
        if (ctx->buf_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buf_pos = 0;
        return STATE_START;
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
    
    const char *current = input;
    
    while (*current != '\0' && ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        char c = *current;
        
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
                return -1;
            case STATE_END:
                return 0;
        }
        
        current++;
    }
    
    if (ctx->current_state == STATE_READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == STATE_READING_WORD) {
        ctx->word_count++;
    }
    
    return (ctx->current_state == STATE_ERROR) ? -1 : 0;
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    init_parser(&parser);
    
    if (process_input(&parser, input) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}