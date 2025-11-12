//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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

token_type_t get_token_type(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

void reset_token(parser_context_t *ctx) {
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
    ctx->token_len = 0;
}

void add_to_token(parser_context_t *ctx, char c) {
    if (ctx->token_len < MAX_INPUT_LEN - 1) {
        ctx->current_token[ctx->token_len++] = c;
        ctx->current_token[ctx->token_len] = '\0';
    }
}

void process_token(parser_context_t *ctx) {
    if (ctx->token_len > 0) {
        if (isdigit((unsigned char)ctx->current_token[0])) {
            ctx->number_count++;
            printf("Found number: %s\n", ctx->current_token);
        } else if (isalpha((unsigned char)ctx->current_token[0])) {
            ctx->word_count++;
            printf("Found word: %s\n", ctx->current_token);
        }
        reset_token(ctx);
    }
}

parser_state_t handle_state_transition(parser_context_t *ctx, token_type_t token, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (token == TOKEN_DIGIT) {
                add_to_token(ctx, c);
                return STATE_READING_NUMBER;
            } else if (token == TOKEN_LETTER) {
                add_to_token(ctx, c);
                return STATE_READING_WORD;
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                return STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (token == TOKEN_DIGIT) {
                add_to_token(ctx, c);
                return STATE_READING_NUMBER;
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                process_token(ctx);
                return (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (token == TOKEN_LETTER) {
                add_to_token(ctx, c);
                return STATE_READING_WORD;
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                process_token(ctx);
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

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t ctx;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    for (size_t i = 0; i < strlen(input) + 1; i++) {
        char current_char = input[i];
        token_type_t token = get_token_type(current_char);
        
        ctx.current_state = handle_state_transition(&ctx, token, current_char);
        
        if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "Parse error at position %zu\n", i);
            return 1;
        }
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state != STATE_DONE && ctx.current_state != STATE_START) {
        process_token(&ctx);
    }
    
    printf("\nParsing complete:\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}