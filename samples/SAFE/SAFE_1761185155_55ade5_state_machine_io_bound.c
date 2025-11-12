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
    char buffer[MAX_INPUT_LEN + 1];
} parser_context_t;

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (isspace(c)) {
        return STATE_START;
    } else if (isdigit(c)) {
        ctx->buffer[0] = c;
        ctx->buffer[1] = '\0';
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[0] = c;
        ctx->buffer[1] = '\0';
        return STATE_READING_WORD;
    } else if (c == '\n' || c == '\0') {
        return STATE_DONE;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    size_t len = strlen(ctx->buffer);
    if (len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    if (isdigit(c)) {
        ctx->buffer[len] = c;
        ctx->buffer[len + 1] = '\0';
        return STATE_READING_NUMBER;
    } else if (isspace(c) || c == '\n' || c == '\0') {
        ctx->number_count++;
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    size_t len = strlen(ctx->buffer);
    if (len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    if (isalpha(c)) {
        ctx->buffer[len] = c;
        ctx->buffer[len + 1] = '\0';
        return STATE_READING_WORD;
    } else if (isspace(c) || c == '\n' || c == '\0') {
        ctx->word_count++;
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

int process_input(const char *input, parser_context_t *ctx) {
    size_t input_len = strlen(input);
    if (input_len > MAX_INPUT_LEN) {
        return -1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
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
                return -1;
            case STATE_DONE:
                return 0;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
    }
    
    if (ctx->current_state == STATE_READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == STATE_READING_WORD) {
        ctx->word_count++;
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t parser;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    initialize_parser(&parser);
    
    if (process_input(input, &parser) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}