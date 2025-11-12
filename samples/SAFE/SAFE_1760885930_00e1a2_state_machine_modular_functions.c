//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int error_count;
} parser_context_t;

token_type_t get_next_token(char input, int *pos, int max_len) {
    if (*pos >= max_len) return TOKEN_EOF;
    
    char c = input;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (c == ' ' || c == '\t') return TOKEN_SPACE;
    
    return TOKEN_INVALID;
}

parser_state_t handle_start_state(token_type_t token, parser_context_t *ctx) {
    switch (token) {
        case TOKEN_DIGIT:
            ctx->number_count++;
            return STATE_READING_NUMBER;
        case TOKEN_LETTER:
            ctx->word_count++;
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            ctx->error_count++;
            return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(token_type_t token, parser_context_t *ctx) {
    switch (token) {
        case TOKEN_DIGIT:
            return STATE_READING_NUMBER;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            ctx->error_count++;
            return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(token_type_t token, parser_context_t *ctx) {
    switch (token) {
        case TOKEN_LETTER:
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            ctx->error_count++;
            return STATE_ERROR;
    }
}

parser_state_t handle_error_state(token_type_t token, parser_context_t *ctx) {
    switch (token) {
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->error_count = 0;
}

int process_input(const char *input, parser_context_t *ctx) {
    if (input == NULL) return -1;
    
    int len = strlen(input);
    if (len >= MAX_INPUT_LEN) return -1;
    
    int pos = 0;
    initialize_parser(ctx);
    
    while (ctx->current_state != STATE_END) {
        token_type_t token = get_next_token(input[pos], &pos, len);
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(token, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(token, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(token, ctx);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_error_state(token, ctx);
                break;
            case STATE_END:
                break;
        }
        
        if (token != TOKEN_EOF) {
            pos++;
        }
        
        if (pos > len) break;
    }
    
    return 0;
}

void print_results(const parser_context_t *ctx) {
    printf("Parser results:\n");
    printf("Numbers found: %d\n", ctx->number_count);
    printf("Words found: %d\n", ctx->word_count);
    printf("Errors encountered: %d\n", ctx->error_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    parser_context_t ctx;
    
    printf("Enter text to parse (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_input(input, &ctx) != 0) {
        printf("Error processing input\n");
        return 1;
    }
    
    print_results(&ctx);
    
    return 0;
}