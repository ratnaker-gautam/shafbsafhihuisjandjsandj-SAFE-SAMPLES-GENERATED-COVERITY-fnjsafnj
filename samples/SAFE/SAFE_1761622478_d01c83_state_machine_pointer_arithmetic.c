//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int word_count;
    int number_count;
    char buffer[MAX_INPUT_LEN];
    char *buffer_ptr;
} parser_context_t;

void init_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    ctx->buffer_ptr = ctx->buffer;
}

parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (isspace(c)) {
        return STATE_START;
    } else if (isalpha(c)) {
        *(ctx->buffer_ptr) = c;
        ctx->buffer_ptr++;
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        *(ctx->buffer_ptr) = c;
        ctx->buffer_ptr++;
        return STATE_READING_NUMBER;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (isalpha(c)) {
        if ((ctx->buffer_ptr - ctx->buffer) < (MAX_INPUT_LEN - 1)) {
            *(ctx->buffer_ptr) = c;
            ctx->buffer_ptr++;
        }
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        ctx->word_count++;
        memset(ctx->buffer, 0, MAX_INPUT_LEN);
        ctx->buffer_ptr = ctx->buffer;
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(parser_context_t *ctx, char c) {
    if (isdigit(c)) {
        if ((ctx->buffer_ptr - ctx->buffer) < (MAX_INPUT_LEN - 1)) {
            *(ctx->buffer_ptr) = c;
            ctx->buffer_ptr++;
        }
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        ctx->number_count++;
        memset(ctx->buffer, 0, MAX_INPUT_LEN);
        ctx->buffer_ptr = ctx->buffer;
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

int process_input(parser_context_t *ctx, const char *input) {
    const char *ptr = input;
    
    while (*ptr != '\0') {
        char current_char = *ptr;
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(ctx, current_char);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(ctx, current_char);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx, current_char);
                break;
            case STATE_ERROR:
                return -1;
            case STATE_END:
                return 0;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
        
        ptr++;
    }
    
    if (ctx->current_state == STATE_READING_WORD) {
        ctx->word_count++;
    } else if (ctx->current_state == STATE_READING_NUMBER) {
        ctx->number_count++;
    }
    
    ctx->current_state = STATE_END;
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
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
    
    init_parser(&parser);
    
    if (process_input(&parser, input) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Words: %d\n", parser.word_count);
    printf("Numbers: %d\n", parser.number_count);
    
    return 0;
}