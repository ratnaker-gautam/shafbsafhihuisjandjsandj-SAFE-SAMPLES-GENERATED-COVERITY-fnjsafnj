//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

token_type_t get_next_token(char input, int position) {
    if (position < 0) return TOKEN_INVALID;
    
    if (input == '\0') return TOKEN_EOF;
    if (isspace((unsigned char)input)) return TOKEN_SPACE;
    if (isdigit((unsigned char)input)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)input)) return TOKEN_LETTER;
    
    return TOKEN_INVALID;
}

parser_state_t handle_start_state(token_type_t token, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
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
    if (ctx == NULL) return STATE_ERROR;
    
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
    if (ctx == NULL) return STATE_ERROR;
    
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
    if (ctx == NULL) return STATE_ERROR;
    
    switch (token) {
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

parser_state_t process_token(token_type_t token, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start_state(token, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(token, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(token, ctx);
        case STATE_ERROR:
            return handle_error_state(token, ctx);
        case STATE_END:
            return STATE_END;
        default:
            ctx->error_count++;
            return STATE_ERROR;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && input[i] != '\0') {
            return 0;
        }
    }
    return 1;
}

void process_input(const char *input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }
    
    parser_context_t ctx = {
        .current_state = STATE_START,
        .number_count = 0,
        .word_count = 0,
        .error_count = 0
    };
    
    int position = 0;
    char current_char;
    
    do {
        current_char = input[position];
        token_type_t token = get_next_token(current_char, position);
        ctx.current_state = process_token(token, &ctx);
        position++;
    } while (ctx.current_state != STATE_END && position <= MAX_INPUT_LEN);
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    printf("Errors: %d\n", ctx.error_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(input);
    
    return 0;
}