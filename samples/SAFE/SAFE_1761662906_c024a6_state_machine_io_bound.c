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
    char current_token[MAX_INPUT_LEN];
    int token_len;
} parser_context_t;

static token_type_t get_token_type(char c) {
    if (c == '\0') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

static void reset_token(parser_context_t *ctx) {
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
    ctx->token_len = 0;
}

static void add_to_token(parser_context_t *ctx, char c) {
    if (ctx->token_len < MAX_INPUT_LEN - 1) {
        ctx->current_token[ctx->token_len++] = c;
        ctx->current_token[ctx->token_len] = '\0';
    }
}

static parser_state_t handle_state_transition(parser_context_t *ctx, token_type_t token) {
    switch (ctx->current_state) {
        case STATE_START:
            if (token == TOKEN_DIGIT) {
                reset_token(ctx);
                add_to_token(ctx, '0' + (token - TOKEN_DIGIT));
                return STATE_READING_NUMBER;
            } else if (token == TOKEN_LETTER) {
                reset_token(ctx);
                add_to_token(ctx, 'a' + (token - TOKEN_LETTER));
                return STATE_READING_WORD;
            } else if (token == TOKEN_SPACE) {
                return STATE_START;
            } else if (token == TOKEN_EOF) {
                return STATE_DONE;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (token == TOKEN_DIGIT) {
                add_to_token(ctx, '0' + (token - TOKEN_DIGIT));
                return STATE_READING_NUMBER;
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                ctx->number_count++;
                printf("Found number: %s\n", ctx->current_token);
                reset_token(ctx);
                return (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (token == TOKEN_LETTER) {
                add_to_token(ctx, 'a' + (token - TOKEN_LETTER));
                return STATE_READING_WORD;
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                ctx->word_count++;
                printf("Found word: %s\n", ctx->current_token);
                reset_token(ctx);
                return (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        case STATE_DONE:
            return STATE_DONE;
    }
    return STATE_ERROR;
}

static int process_input(const char *input) {
    parser_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        token_type_t token = get_token_type(c);
        
        ctx.current_state = handle_state_transition(&ctx, token);
        
        if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "Parser error at position %zu\n", i);
            return 1;
        }
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    printf("Parsing completed. Numbers: %d, Words: %d\n", ctx.number_count, ctx.word_count);
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to parse (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    return process_input(input);
}