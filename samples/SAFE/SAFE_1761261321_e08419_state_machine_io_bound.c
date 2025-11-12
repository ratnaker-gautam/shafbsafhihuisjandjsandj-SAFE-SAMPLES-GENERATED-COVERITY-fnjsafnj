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

token_type_t get_token_type(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
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
            return STATE_DONE;
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
            return STATE_DONE;
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
            return STATE_DONE;
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
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

void process_input(const char *input, parser_context_t *ctx) {
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) {
        ctx->error_count++;
        return;
    }

    for (size_t i = 0; i <= len; i++) {
        token_type_t token = get_token_type(input[i]);
        
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
            case STATE_DONE:
                return;
        }
        
        if (ctx->current_state == STATE_DONE) {
            break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t ctx = {
        .current_state = STATE_START,
        .number_count = 0,
        .word_count = 0,
        .error_count = 0
    };

    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    process_input(input, &ctx);

    printf("Parsing results:\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    printf("Errors found: %d\n", ctx.error_count);

    return 0;
}